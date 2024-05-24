#!/bin/bash

# This is a template for generating submissions scripts - 1 per condition set

########## Define Resources Needed with SBATCH ##########

#SBATCH --job-name <<JOB_NAME>>             # you can give your job a name for easier identification (same as -J)

#SBATCH --time=<<TIME_REQUEST>>             # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=<<ARRAY_ID_RANGE>>          # Number of replicates, replace ARRAY_ID_RANGE with the range of array indices e.g. 0-99
# you only need to specify resources required for a single task, not the entire array
#SBATCH --mem=<<MEMORY_REQUEST>>            # memory required per node - amount of memory (in bytes)
#SBATCH --account=<<ACCOUNT_NAME>>

#SBATCH --mail-type=END,FAIL                # Mail if jobs end, or fail
#SBATCH --mail-user=suzuekar@msu.edu

########## Command Lines to Run ##########

ENV=aged_lexicase # Conda environment placeholder

module load Conda/3
conda activate ${ENV}

JOB_SEED_OFFSET=<<JOB_SEED_OFFSET>>
SEED=$((JOB_SEED_OFFSET + SLURM_ARRAY_TASK_ID - 1))

# EXEC=../../build/MABE # Executable
EXEC=../../../../../build/MABE
REPO_DIR=/YYYY_MM_DD_HH_MM
CONFIG_DIR=../${REPO_DIR}
DATA_DIR=${REPO_DIR}/data
RUN_DIR=${DATA_DIR}/Conditions_${SEED}

# Make new run folder
mkdir -p ${RUN_DIR}
cd ${RUN_DIR}

# Copy .mabe files and exec to run folder (current directory)
cp ${CONFIG_DIR}/*.mabe . 
# cp ../../../${EXEC} .
cp ${EXEC} .

######## Run experiment ########
# Ensures the current directory is the run directory
cd ${RUN_DIR}

<<CFG_RUN_COMMANDS>>

<<RUN_COMMANDS>>

######## Clean up ########
rm ${RUN_DIR}/*.mabe
rm ${RUN_DIR}/${EXEC}

