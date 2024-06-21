'''
Compile and aggregate (average) data
'''

import os
import pandas as pd
import numpy as np
from datetime import datetime

SEED_OFFSET = 10000
REPLICATES = 20
SET = "0"

# Isolate and return conditions from directory path 
def parse_conditions(dir_path):
    # Example of a directory name: /mnt/.../C0_AgeControl_explore_200_500_50_10000
    # First term is condition/treatment ID
    # Last term is array-task-dependent seed, which differs between replicates
    base_name = dir_path.split('/')[-1]
    conditions = base_name.split('_')
    condition_id = conditions[0].replace("C","")
    array_id = int(conditions[-1]) - SEED_OFFSET - int(condition_id)*20 + 1
    return (conditions, array_id)

def aggregate_data(condition_folders, output_file):
    # Takes in a list of paths from the same condition and a path to the output file
    # Averages all replicates from the same condition

    aggregated_data = None

    # loop through list of condition folders
    for folder in condition_folders:
        conditions, array_id = parse_conditions(folder)

        # Locate run data in the current condition folder
        run_file = os.path.join(folder, f"run_{array_id}.csv")

        if os.path.exists(run_file):
            run_data = pd.read_csv(run_file)
            # Add "Generation" column
            run_data['Generations'] = [(i*1000 + 1000) for i in range(len(run_data))]
            # Add "Replicate" column
            run_data['Replicate'] = [array_id for i in range(len(run_data))]

            if aggregated_data is None:
                aggregated_data = run_data
            else:
                aggregated_data += run_data              
            
        else:
            print(f"Run file {run_file} not found.")     
        
    if aggregated_data is not None:
        aggregated_data /= len(condition_folders)
        aggregated_data.to_csv(output_file)


def main():
    # TODO: For now, average across all conditions

    # Directory holding all run folders
    data_dir = os.path.join("/mnt/scratch/suzuekar/GPTP2024/", SET)

    # Output folder for aggregated data
    output_dir = os.path.join(data_dir, "compile")
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    condition_folders = [os.path.join(data_dir, entry) for entry in sorted(os.listdir(data_dir))
                         if os.path.isdir(os.path.join(data_dir, entry))
                         and entry not in ["jobs", "compile"]]

    # Group condition folders by experimental configuration
    condition_groups = {} # {"conditions", [folders]}
    for folder in condition_folders:
        conditions, array_id = parse_conditions(folder)
        conditions_str = "_".join(conditions[:-1])
        if conditions_str not in condition_groups:
            condition_groups[conditions_str] = []
        condition_groups[conditions_str].append(folder)

    for cond, folders in condition_groups.items():
        output_file = os.path.join(output_dir, f"{cond}.csv")
        aggregate_data(folders, output_file)    


if __name__ == "__main__":
    main()