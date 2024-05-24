#!/usr/bin/env bash

REPLICATES=20
ACCOUNT=suzuekar
SEED_OFFSET=10000
JOB_TIME=72:00:00
JOB_MEM=16G

current_date=$(date +"%Y-%m-%d")

# SCRATCH_EXP_DIR=/mnt/scratch/lalejini/data-public/${PROJECT_NAME}
# REPO_DIR=/mnt/home/lalejini/devo_ws/${PROJECT_NAME}
# HOME_EXP_DIR=${REPO_DIR}/experiments

REPO_DIR=/mnt/home/suzuekar/MABE2/runs/GPTP2024
CONFIG_DIR=${REPO_DIR}
EXEC_DIR=${REPO_DIR}

DATA_DIR=${REPO_DIR}/${current_date}
JOB_DIR=${DATA_DIR}/jobs

mkdir -p ${CONFIG_DIR}
cp -R ${CONFIG_DIR_SRC}/* ${CONFIG_DIR}/

python3 gen-script.py --runs_per_subdir 500 --time_request ${JOB_TIME} --mem ${JOB_MEM} --exec_dir ${EXEC_DIR} --data_dir ${DATA_DIR} --config_dir ${CONFIG_DIR} --repo_dir ${REPO_DIR} --replicates ${REPLICATES} --job_dir ${JOB_DIR} --account ${ACCOUNT} --seed_offset ${SEED_OFFSET}