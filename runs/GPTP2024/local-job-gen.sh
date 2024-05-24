#!/usr/bin/env bash

REPLICATES=20
ACCOUNT=suzuekar
SEED_OFFSET=10000
JOB_TIME=72:00:00
JOB_MEM=16G

current_date=$(date +"%Y-%m-%d")

# SCRATCH_EXP_DIR=./test/data/${PROJECT_NAME}
# REPO_DIR=/Users/lalejina/devo_ws/${PROJECT_NAME}
# HOME_EXP_DIR=${REPO_DIR}/experiments

REPO_DIR=/mnt/c/Users/HP/Documents/GitHub/MABE2/runs/GPTP2024
CONFIG_DIR=${REPO_DIR}
EXEC_DIR=${REPO_DIR}

DATA_DIR=${REPO_DIR}/${current_date}
JOB_DIR=${DATA_DIR}/jobs

# DATA_DIR=${SCRATCH_EXP_DIR}/${EXP_SLUG}
# JOB_DIR=${SCRATCH_EXP_DIR}/${EXP_SLUG}/jobs
# CONFIG_DIR=${HOME_EXP_DIR}/${EXP_SLUG}/hpc/config

python3 gen-script.py --runs_per_subdir 500 --time_request ${JOB_TIME} --mem ${JOB_MEM} --exec_dir ${EXEC_DIR} --data_dir ${DATA_DIR} --config_dir ${CONFIG_DIR} --repo_dir ${REPO_DIR} --replicates ${REPLICATES} --job_dir ${JOB_DIR} --account ${ACCOUNT} --seed_offset ${SEED_OFFSET}