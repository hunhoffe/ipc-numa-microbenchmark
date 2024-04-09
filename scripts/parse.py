import argparse
from collections import defaultdict
import os

def parse_data(results_dir):
    all_test_data = []

    for file in os.listdir(results_dir):
        filename = os.fsdecode(file)
        if filename.endswith(".log") and filename.startswith("client"):
            test_tokens = filename.split(".")[0].split("-")
            test_type = test_tokens[1]
            test_case = test_tokens[2]
            bytes_per_op = float(test_tokens[3])

            with open(os.path.join(results_dir, filename), 'r') as f:
                iterations = f.read().split("====")[1:] # iteration 0 is not a real iteration since file starts with ====
            
            num_iters = len(iterations)
            ops_per_second = 0
            for i in iterations:
                lines = i.split('\n')
                for l in lines:
                    if l.startswith('Average operations per second is: '):
                        ops_per_second += float(l.split(": ")[1])
            ops_per_second = ops_per_second / float(num_iters)
            bytes_per_second = ops_per_second * bytes_per_op
        
            print(f"Type={test_type}, Case={test_case}, bytes_per_op={bytes_per_op}, ops_per_second={ops_per_second}, bytes_per_second={bytes_per_second}")

    return all_test_data


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

    data = parse_data(args.results_dir)


if __name__ == "__main__":
    main()