#!/usr/bin/env bash

REPLICATES=20
# ACCOUNT=default
PARTITION=general-short
SEED_OFFSET=10000
# JOB_TIME=72:00:00
# JOB_MEM=16G
JOB_TIME=03:00:00
JOB_MEM=8G

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

# Function to wait until squeue is empty
wait_for_set() { 
  # Take all arguments and form a comma-separated string
  job_ids_str=$(IFS=,; echo "$*")
  
  # Check if any of the jobs are still in the queue
  squeue -j $job_ids_str > /dev/null 2>&1 
  # (Use squeue -j to check for multiple jobs at once: 'squeue -j jobid1,jobid2,jobid3')
  
  # While jobs are still in squeue, exist status ('$?') is equal to 0
  while [ $? -eq 0 ]; do
    echo "Waiting for jobs $job_ids_str to complete..."
    sleep 1800 # wait for 30 minutes before checking again 
    squeue -j $job_ids_str > /dev/null 2>&1 # discard both stdout and stderr
    # if no jobs match the given ids (queue is empty), squeue has non-zero exit status 
    # exist status will be automatically store in variable '$?'
  done
}


# Make all generated submission scripts executable, sbatch
for set_dir in ${JOB_DIR}/set-*; do
  # Empty array, holds submitted job IDs of current set (not individual task IDs)
  job_ids=() 
  for script in ${set_dir}/*.sh; do
    chmod +x "$script"
    id=$(sbatch "$script" | awk '{print $4}')
    job_ids+=($id)
  done
  # Wait until one set is complete
  wait_for_set "${job_ids[@]}" # pass each ID in array as a series of arguments?
done
