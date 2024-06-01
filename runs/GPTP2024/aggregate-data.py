'''
Compile and aggregate (average) data
'''

import os
import pandas as pd
import numpy as np
from datetime import datetime

# Isolate and return conditions from directory path 
def parse_conditions(dir_path):
    # Example of a directory name: /mnt/.../C0_AgeControl_explore_200_500_50_10000
    # First term is condition/treatment ID
    # Last term is array-task-dependent seed, which differs between replicates
    # We want the middle terms
    base_name = dir_path.split('/')[-1]
    conditions = base_name.split('_')[:-1]
    return conditions

# Combine csv files from runs with the same conditions, but different seeds
def combine_csv(data_dir):
    






def main():
    # TODO: Probably not ideal, make sure this matches with the actual data dir
    formatted_date = datetime.now().strftime("%Y-%m-%d")
    # Directory holding all run folders
    data_dir = os.path.join("/mnt/home/suzuekar/MABE2/runs/GPTP2024/", formatted_date)
    # Output folder for aggregated data
    output_dir = os.path.join(data_dir, "compile")


if __name__ == "__main__":
    # NOTE: There should be 72 different csv files corresponding to 72 treatments
    # TODO: not sure what to do with the data after compiling...
        # What metrics are we interested in?
        # How should we present the data?

    # The folders look like this (2 replicates per treatment) in /mnt/home/suzuekar/MABE2/runs/GPTP2024/2024-05-27 (data dir)
    # C0_AgeControl_explore_200_500_50_10000  C0_AgeControl_explore_200_500_50_10001  
    # C9_AgeControl_explore_500_500_200_10018  C9_AgeControl_explore_500_500_200_10019
    # you just have to forget about the two things on the side 






    main()