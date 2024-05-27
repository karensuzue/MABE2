'''
Compile and aggregate (average) data
'''

import os
import pandas as pd
import numpy as np
from datetime import datetime

def main():
    pass


if __name__ == "__main__":
    # NOTE: There should be 72 different treatments after aggregation

    formatted_date = datetime.now().strftime("%Y-%m-%d")

    # Directory holding all run folders
    data_dir = os.path.join("/mnt/home/suzuekar/MABE2/runs/GPTP2024/", formatted_date)
    # Output folder for aggregated data
    output_dir = os.path.join(data_dir, "compile")

    # 1. Go through all the folders, 
    #   place all the runs with the same conditions and diff seed in a single monster csv
    # 2.  You should have 72 different csvs now
    # 3. average all the data in each of those monster csvs, place them into the final csv

    # The folders look like this (2 replicates per treatment) in /mnt/home/suzuekar/MABE2/runs/GPTP2024/2024-05-27 (data dir)
    # C0_AgeControl_explore_200_500_50_10000  C0_AgeControl_explore_200_500_50_10001  
    # C9_AgeControl_explore_500_500_200_10018  C9_AgeControl_explore_500_500_200_10019
    # you just have to forget about the two things on the side 

    




    main()