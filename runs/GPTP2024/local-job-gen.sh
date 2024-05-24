#!/usr/bin/env bash

# GPTP2024 [CONFIG_DIR]
# ├── config_file1.mabe
# ├── config_file2.mabe
# ├── ... (more .mabe files)
# └── /YYYY_MM_DD [REPO_DIR]
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

REPLICATES=20
ACCOUNT=suzuekar
SEED_OFFSET=10000
JOB_TIME=72:00:00
JOB_MEM=16G

# SCRATCH_EXP_DIR=./test/data/${PROJECT_NAME}
# REPO_DIR=/Users/lalejina/devo_ws/${PROJECT_NAME}
# HOME_EXP_DIR=${REPO_DIR}/experiments

# DATA_DIR=${SCRATCH_EXP_DIR}/${EXP_SLUG}
# JOB_DIR=${SCRATCH_EXP_DIR}/${EXP_SLUG}/jobs
# CONFIG_DIR=${HOME_EXP_DIR}/${EXP_SLUG}/hpc/config

python3 gen-script.py --account ${ACCOUNT} --runs_per_subdir 500 --time_request ${JOB_TIME} --mem ${JOB_MEM} --replicates ${REPLICATES} --account ${ACCOUNT} --seed_offset ${SEED_OFFSET}