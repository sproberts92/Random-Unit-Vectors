import matplotlib.pyplot as plt
import numpy as np
from subprocess import call
import os

def main():
	# Figure 1
	process('box-draw-10000.dat',   'box-discard-z-scatter-10000.png',   discard_z, plt.plot, ',')
	# Figure 2
	process('box-draw-1000.dat',    'box-discard-z-scatter-1000.png',    discard_z, plt.plot, ',')
	# Figure 3
	process('box-draw-1000000.dat', 'box-discard-z-scatter-1000000.png', discard_z, plt.plot, ',')

	# Figure 4
	process('box-draw-2000000.dat', 'box-discard-z-hist2d-200.png',     discard_z, plt.hist2d, 200, range=[[-1,1],[-1,1]])
	# Figure 5
	process('box-draw-2000000.dat', 'box-discard-z-hist2d-20.png',      discard_z, plt.hist2d, 20, range=[[-1,1],[-1,1]])
	# Figure 6
	process('box-draw-2000000.dat', 'box-discard-z-hist2d-2000000.png', discard_z, plt.hist2d, 2000, range=[[-1,1],[-1,1]])

	# Figure 7
	process('box-draw-2000000.dat', 'box-discard-z-hist2d/frame_{0}.png', discard_z, plt.hist2d, 200, range=[[-1,1],[-1,1]], frames=25)

	# Figure 8
	process('box-draw-2000000.dat', 'box-lambert-hist2d-200.png',     lambert, plt.hist2d, 200, range=[[-2,2],[-2,2]])
	# Figure 9
	process('box-draw-2000000.dat', 'box-lambert-hist2d/frame_{0}.png', lambert, plt.hist2d, 200, range=[[-2,2],[-2,2]], frames=25)

	# Figure 10
	process('discard-corners-2000000.dat', 'discard-corners-discard-z-hist2d-200.png', discard_z, plt.hist2d, 50, range=[[-1,1],[-1,1]])
	# Figure 11
	process('discard-corners-2000000.dat', 'discard-corners-lambert-hist2d-200.png',   lambert,   plt.hist2d, 50, range=[[-2,2],[-2,2]])

	# Figure 12
	process('polar-3d-2000000.dat', 'polar-3d-discard-z-hist2d-200.png', discard_z, plt.hist2d, 50, range=[[-1,1],[-1,1]])
	# Figure 13
	process('polar-3d-2000000.dat', 'polar-3d-lambert-hist2d-200.png',   lambert,   plt.hist2d, 50, range=[[-2,2],[-2,2]])

def rotate_data(data, theta):
	if theta != 0:
		rotation = np.array([[np.cos(theta), 0, -np.sin(theta)], [0, 1, 0], [np.sin(theta), 0, np.cos(theta)]])

		return np.array([np.dot(rotation, point) for point in data])
	else:
		# Don't bother trying to rotate if we don't have to.
		return data

def discard_z(data):
	# Just unpack the z coordinate into nothing
	X, Y, _ = data.T
	return X, Y

def lambert(data):
	# First filter out points with z == 1, as Lambert only maps S^2\(0,0,1) -> V^2.
	a = data[np.where(data.T[2] < 1.0)].T

	# Actually do the projection
	return np.array([np.sqrt(2 / (1 - a[2])) * a[0], np.sqrt(2 / (1 - a[2])) * a[1]])

	# Shouldn't have to do this last filtering but I keep ending up with some points
	# outside the disc.
	# return a[np.where(abs(a.T[0]) < 2.0) and np.where(abs(a.T[0]) < 2.0)].T

def process(in_name, out_name, proj, plot_t, pos_arg, frames=1, **kwargs):
	print('Processing {0}...'.format(out_name), end='')

	in_path = "output/data/{0}".format(in_name)
	out_path = "output/plots/{0}".format(out_name)

	# Create output directory, especially important for the animations
	d = os.path.dirname(out_path)
	if not os.path.exists(d):
		os.makedirs(d)

	# Load the data
	data = np.loadtxt(in_path)

	for i in range(frames):
		# 48 is for 24fps video, we want to do 1/4 of one full rotation,
		# which when looped will look like full rotation.
		theta = i * np.pi / (2 * frames)
		X, Y = proj(rotate_data(data, theta))

		# Close any old figures left over from the last animation frame
		plt.close()

		# Create a new figure with fixed size and no frame
		plt.figure(figsize=(4, 4), frameon=False)

		# It would be nice to pass pos_arg
		plot_t(X, Y, pos_arg, **kwargs)

		# A lot of options just to get a borderless image
		ax = plt.axes()
		ax.set_aspect('equal')
		ax.set_xticks([]); ax.set_yticks([])
		plt.axis('off')

		plt.savefig(out_path.format(i), bbox_inches='tight', pad_inches=0)
		plt.close()

	# If we're dealing with multiple frames then encode them into an html5 video.
	if frames > 1:
		call(['ffmpeg', '-loglevel', 'quiet', '-f', 'image2', '-i', out_path.format('%d'), '-c:v', 'libvpx', '-b:v', '64M', '{0}.webm'.format(d)])

	print(' Done.')

if __name__ == '__main__':
	main()
