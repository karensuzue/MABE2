#!/bin/bash

# Target number of files in a folder
target_file_count=5

# Initialize counter for folders with the target number of files
count=0

# Loop through all directories in the current directory
for dir in */ ; do
  if [ -d "$dir" ]; then
    # Count the number of files in the directory
    file_count=$(find "$dir" -type f | wc -l)

    # Check if the file count is greater the target file count
    if [ "$file_count" != "$target_file_count" ]; then
      ((count++))
      echo "$dir"
    fi
  fi
done

# Output the result
echo "Number of folders with files not equal to $target_file_count files: $count"