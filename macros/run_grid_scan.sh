#!/bin/bash

# Create directory for run configs if it doesn't exist
mkdir -p run_nconfigs
mkdir -p ../rootFiles

# Create or clear the log file
> run_logs.txt

# Read positions from text file
while IFS=, read -r x y
do
  # Trim whitespace
  x=$(echo "$x" | tr -d ' ')
  y=$(echo "$y" | tr -d ' ')
  
  # Skip empty lines
  if [ -z "$x" ] || [ -z "$y" ]; then
      continue
  fi

  # Generate a random 5-digit seed
  seed=$(shuf -i 10000-99999 -n 1)
  
  # Log the run parameters
  echo "${x} ${y} ${seed}" >> run_logs.txt
  
  # Generate the macro file by replacing XPOS and YPOS
  # Note: avoiding inline cat for safety, using direct sed input redirection
  sed "s/XPOS/${x}/g; s/YPOS/${y}/g" runTMP.mac > "run_nconfigs/run_${x}_${y}.mac"
  
  # Run the Geant4 application
  echo "Running simulation for position (${x}, ${y}) with seed ${seed}..."
  ../build/myApp "run_nconfigs/run_${x}_${y}.mac" "../rootFiles/GridScan_X${x}_Y${y}_seed${seed}.root" "${seed}"
  
done < "scan_positions.txt"

echo "All simulations completed!"
echo "Log file: run_logs.txt"
echo "Macro files: run_nconfigs/run_X_Y.mac"
