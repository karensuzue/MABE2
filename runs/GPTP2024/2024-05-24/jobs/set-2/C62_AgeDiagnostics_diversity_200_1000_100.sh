#!/bin/bash

# This is a template for generating submissions scripts - 1 per condition set

# TODO: Fix this tree
# GPTP2024 [CONFIG_DIR] [REPO_DIR]
# ├── config_file1.mabe
# ├── config_file2.mabe
# MABE
# ├── ... (more .mabe files)
# └── /YYYY_MM_DD_HH_MM [DATA_DIR]
#     ├── jobs [JOB_DIR]
#     │   ├── set1
#     │   │   ├── job1.sh
#     │   │   └── job2.sh
#     │   ├── set2
#     │   └── ... (other subdirectories containing submission scripts)
#     ├── Conditions_SEED1 [RUN_DIR]
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

########## Define Resources Needed with SBATCH ##########

#SBATCH --job-name aged_lexicase             # you can give your job a name for easier identification (same as -J)

#SBATCH --time=72:00:00             # limit of wall clock time - how long the job will run (same as -t)
#SBATCH --array=1-20          # Number of replicates, replace ARRAY_ID_RANGE with the range of array indices e.g. 0-99
# you only need to specify resources required for a single task, not the entire array
#SBATCH --mem=16G            # memory required per node - amount of memory (in bytes)
#SBATCH --account=suzuekar

#SBATCH --mail-type=END,FAIL                # Mail if jobs end, or fail
#SBATCH --mail-user=suzuekar@msu.edu

########## Command Lines to Run ##########

ENV=aged_lexicase # Conda environment placeholder

module load Conda/3
conda activate ${ENV}

JOB_SEED_OFFSET=11240
SEED=$((JOB_SEED_OFFSET + SLURM_ARRAY_TASK_ID - 1))

EXEC=MABE2 
EXEC_DIR=/mnt/c/Users/HP/Documents/GitHub/MABE2/runs/GPTP2024 

CONFIG_DIR=/mnt/c/Users/HP/Documents/GitHub/MABE2/runs/GPTP2024 
REPO_DIR=/mnt/c/Users/HP/Documents/GitHub/MABE2/runs/GPTP2024
DATA_DIR=/mnt/c/Users/HP/Documents/GitHub/MABE2/runs/GPTP2024/2024-05-24

RUN_DIR=/mnt/c/Users/HP/Documents/GitHub/MABE2/runs/GPTP2024/2024-05-24/C62_AgeDiagnostics_diversity_200_1000_100_${SEED}  # run dir is CONFIG_DIR or REPO_DIR / DATA_DIR/

# Make new run folder
mkdir -p ${RUN_DIR}
cd ${RUN_DIR}

# Copy .mabe files to run folder
cp ${CONFIG_DIR}/*.mabe . 
cp ${EXEC_DIR}/${EXEC} .

######## Run experiment ########
# Ensures the current directory is the run directory
cd ${RUN_DIR}

RUN_PARAMS="--SEED ${SEED} --elite_count 0 --filename AgeDiagnostics.mabe --inject_step 100 --num_gens 2000000 --print_step 1000000 -s diagnostics.diagnostic="diversity" -s pop_size=1000 -s inject_count=100 -s num_vals=200"


if [[ ${SLURM_ARRAY_TASK_ID} -eq 1 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 2 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 3 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 4 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 5 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 6 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 7 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 8 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 9 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 10 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 11 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 12 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 13 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 14 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 15 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 16 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 17 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 18 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 19 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi

if [[ ${SLURM_ARRAY_TASK_ID} -eq 20 ]] ; then
echo "${EXEC} ${RUN_PARAMS}" > cmd.log
./${EXEC} ${RUN_PARAMS} > run.log
fi



######## Clean up ########
rm ${RUN_DIR}/*.mabe
rm ${RUN_DIR}/${EXEC}

