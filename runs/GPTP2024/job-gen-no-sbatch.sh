#!/usr/bin/env bash

# This script generates submission scripts

REPLICATES=20
# ACCOUNT=default
PARTITION=general-short
SEED_OFFSET=10000
# JOB_TIME=3:50:00
JOB_MEM=16G

current_date=$(date +"%Y-%m-%d")

# You might want to copy things over to scratch to run

REPO_DIR=/mnt/home/suzuekar/MABE2/runs/GPTP2024
CONFIG_DIR=${REPO_DIR}
EXEC_DIR=${REPO_DIR}

SCRATCH_DIR=/mnt/scratch/suzuekar/GPTP2024
DATA_DIR=${SCRATCH_DIR}/${current_date}
JOB_DIR=${DATA_DIR}/jobs

# Generate submission scripts in JOB_DIR
# TODO: remove time_request - will have to vary between population sizes
python3 gen-script.py --runs_per_subdir 500 --mem ${JOB_MEM} --exec_dir ${EXEC_DIR} --data_dir ${DATA_DIR} --config_dir ${CONFIG_DIR} --repo_dir ${REPO_DIR} --replicates ${REPLICATES} --job_dir ${JOB_DIR} --partition ${PARTITION} --seed_offset ${SEED_OFFSET}

# Make sure MABE is executable 
chmod +x "MABE"