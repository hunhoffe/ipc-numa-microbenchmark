import argparse
import os

def main():
    # Parse args
    parser = argparse.ArgumentParser()
    parser.add_argument('-r', "--results-dir", help="directory containing results files from the run.sh script", type=str)
    args = parser.parse_args()

    # Check args
    if args.results_dir == None:
        print("Must specify output directory.")
        parser.print_help()
        exit(-1)
    elif not os.path.isdir(args.results_dir):
        print("Results directory does not exist")
        parser.print_help()
        exit(-1)


if __name__ == "__main__":
    main()