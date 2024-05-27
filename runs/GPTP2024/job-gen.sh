#!/usr/bin/env bash

REPLICATES=1
# ACCOUNT=default
PARTITION=general-short
SEED_OFFSET=10000
# JOB_TIME=72:00:00
# JOB_MEM=16G
JOB_TIME=03:00:00
JOB_MEM=6G

current_date=$(date +"%Y-%m-%d")

# SCRATCH_EXP_DIR=/mnt/scratch/lalejini/data-public/${PROJECT_NAME}
# REPO_DIR=/mnt/home/lalejini/devo_ws/${PROJECT_NAME}
# HOME_EXP_DIR=${REPO_DIR}/experiments

REPO_DIR=/mnt/home/suzuekar/MABE2/runs/GPTP2024
CONFIG_DIR=${REPO_DIR}
EXEC_DIR=${REPO_DIR}

DATA_DIR=${REPO_DIR}/${current_date}
JOB_DIR=${DATA_DIR}/jobs

# Generate submission scripts in JOB_DIR
python3 gen-script.py --runs_per_subdir 500 --time_request ${JOB_TIME} --mem ${JOB_MEM} --exec_dir ${EXEC_DIR} --data_dir ${DATA_DIR} --config_dir ${CONFIG_DIR} --repo_dir ${REPO_DIR} --replicates ${REPLICATES} --job_dir ${JOB_DIR} --partition ${PARTITION} --seed_offset ${SEED_OFFSET}

# Make sure MABE is executable 
chmod +x "MABE"

# Make all generated submission scripts executable, sbatch
for set_dir in ${JOB_DIR}/set-*; do
  for script in ${set_dir}/*.sh; do
    chmod +x "$script"
    sbatch "$script"
  done
done
