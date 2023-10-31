import glob
import re

# These pairs are right on the border of the epsilon.
# This means, depending on floating point precision errors,
# it is valid to either return them as a match or exclude them as not a match.
# These were manually checked to ensure this is actually valid.
randomly_valid_pairs = [ 
    '33267,62663',
    '62663,33267'
]

def read_and_sort(path):
    """Reads all of the lines from the given file and sorts them."""

    # Read all the lines and remove all the newlines.
    lines = open(path).readlines()
    lines = list(map(lambda l: l.replace('\n', ''), lines))

    # Remove any instances of the pairs which are right on the edge of the epsilon value
    # and so may be present or missing randomly.
    for pair in randomly_valid_pairs:
        if pair in lines:
            lines.remove(pair)

    # Sort and return what's left.
    lines.sort()
    return lines

def is_data_valid(expected_data, actual_data):
    """Compares two lists of data to make sure they're the same."""

    # The amount of data must match.
    if len(expected_data) != len(actual_data):
        return False

    # Go through each line and make sure they're the same.
    for expected, actual in zip(expected_data, actual_data):
        if expected != actual:
            print(f'difference: {expected}, {actual}')
            return False
    return True

def main():
    # This file is considered the source of truth (i.e., the correct answer).
    expected_data_file = './outputs/superego-6.5.0.out'
    expected_data = read_and_sort(expected_data_file)

    # Get all the output files except the "truth" one.
    other_files = glob.glob('./outputs/superego-*.out')
    other_files.remove(expected_data_file)

    with open('./results/validity_results.csv', mode='w') as output_file:
        # Write the CSV header.
        output_file.write('Program,Valid\n')

        for i, other_file in enumerate(other_files):
            # Get the name of the program and update the user on the progress.
            program_name = re.sub(r'\.\./results/(.+)\.out', r'\1', other_file)
            print(f'Validating {i+1}/{len(other_files)}: {program_name}')

            # Read the data from the file, check that it's valid, and write the output to the CSV file.
            other_data = read_and_sort(other_file)
            valid = is_data_valid(expected_data, other_data)
            output_file.write(f'{program_name},{valid}\n')

if __name__ == '__main__':
    main()
