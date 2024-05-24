#!/usr/bin/env bash

# GPTP2024 [CONFIG_DIR]
# ├── config_file1.mabe
# ├── config_file2.mabe
# ├── ... (more .mabe files)
# └── /YYYY_MM_DD_HH_MM [REPO_DIR]
#     └── data [DATA_DIR]
#         ├── jobs [JOB_DIR]
#         │   ├── set1
#         │   │   ├── job1.sh
#         │   │   └── job2.sh
#         │   ├── set2
#         │   └── ... (other subdirectories containing submission scripts)
#         ├── Conditions_SEED1 [RUN_DIR]
#         │   ├── config_file1.mabe
#         │   ├── config_file2.mabe
#         │   ├── MABE
#         │   └── ... (other run files)
#         ├── Conditions_SEED2 [RUN_DIR]
#         │   ├── config_file1.mabe
#         │   ├── config_file2.mabe
#         │   ├── MABE
#         │   └── ... (other run files)
#         └── ... (more run directories) [RUN_DIR]

python3 gen-script.py --data_dir 