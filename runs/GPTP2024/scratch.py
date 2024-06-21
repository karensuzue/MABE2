

    # Iterate through names of experiment condition (replicate) folders
    for entry in sorted(os.listdir(data_dir)):
        # Only consider condition folders
        if entry == "jobs" or entry == "compile":
            continue

        # Locate the condition folder
        entry_path = os.path.join(data_dir, entry)
        if os.path.isdir(entry_path):
            # Get conditions and array task ID of this folder
            conditions, array_id = parse_conditions(entry_path)

            # Locate the corresponding final CSV file 
            conditions_str = "_".join(conditions[:-1])
            target_path = os.path.join(output_dir, f"{conditions_str}.csv")

            # Locate run data in the current condition folder
            run_file = os.path.join(entry_path, f"run_{array_id}.csv")
            if os.path.exists(run_file):
                run_data = pd.read_csv(run_file)

                # Add "Generation" column
                run_data['Generations'] = [(i*1000 + 1000) for i in range(len(run_data))]

                # Add "Replicate" column
                run_data['Replicate'] = [array_id for i in range(len(run_data))]              

                # Append or create the target .csv file
                if os.path.exists(target_path):
                    run_data.to_csv(target_path, mode='a', header=False, index=False)
                else:
                    run_data.to_csv(target_path, index=False)
            
            else:
                print(f"Run file {run_file} not found.")