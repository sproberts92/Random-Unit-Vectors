from subprocess import call
import os

input_docs = [
	'volume-dimension',
	'volume-ratio-dimension'
]

output_dir = 'output/plots'
clean_up = ['.aux', '.log', '.pdf']

def main():
	for doc in input_docs:
		print('Processing {0}...'.format(doc), end='')

		call(['pdflatex',
				'-interaction=batchmode',
				'-output-directory={0}'.format(output_dir),
				'{0}.tex'.format(doc)])

		call(['pdf2svg',
				os.path.join(output_dir,
				'{0}.pdf'.format(doc)),
				os.path.join(output_dir,
				'{0}.svg'.format(doc))],
				shell=True)

		print(' Done.')

	print('Cleaning up... ', end='')

	for ext in clean_up:
		[os.remove(os.path.join(output_dir, f)) for f in os.listdir(output_dir) if f.endswith(ext)]
	
	print(' Done.')

if __name__ == '__main__':
	main()
