import os

output_file = "/Users/tyetkin/Work/g4-pet-module/run.mac"

header = """/gps/energy 140.00 keV
/gps/particle gamma
/gps/direction 0. 0. -1.
/gps/pos/type Beam
/gps/pos/shape Circle
/gps/pos/sigma_r 0.042 cm

# Diagonal scan across the 33.4 x 33.4 mm crystal
# 1mm coordinate steps from lower-left to upper-right.
"""

with open(output_file, "w") as f:
    f.write(header)
    # Start from -16.7 up to 16.7
    # 34 steps of 1mm
    for i in range(34):
        pos = -16.7 + i * 1.0
        f.write(f"/gps/pos/centre {pos:.1f} {pos:.1f} 50.0 mm\n")
        f.write(f"/run/beamOn 1\n")
    # Finish on the exact corner
    f.write(f"/gps/pos/centre 16.7 16.7 50.0 mm\n")
    f.write(f"/run/beamOn 1\n")

print("Created run.mac with diagonal scan.")
