import matplotlib.pyplot as plt
import matplotlib.patches as patches

def plot_sipms_and_scan():
    fig, ax = plt.subplots(figsize=(8, 8))
    
    sipm_size = 4.0   # mm
    gap = 0.2         # mm (assuming 'pitch' in the prompt refers to the gap between SiPMs)
    pitch = sipm_size + gap # 4.2 mm
    grid_size = 8
    
    # Calculate the total size of the grid to center it around (0, 0)
    # The grid spans 8 SiPMs and 7 gaps between them
    total_size = grid_size * sipm_size + (grid_size - 1) * gap
    
    # Start position of the bottom-left corner of the bottom-left SiPM 
    # to center the array perfectly. This ensures lower-left is -x, -y 
    # and upper-right is +x, +y
    start_pos = -total_size / 2.0
    
    for i in range(grid_size):
        for j in range(grid_size):
            # Coordinates for the bottom-left corner of each SiPM
            x = start_pos + i * pitch
            y = start_pos + j * pitch
            
            # Create a Rectangle patch for SiPM
            rect = patches.Rectangle(
                (x, y), sipm_size, sipm_size, 
                linewidth=1, edgecolor='navy', facecolor='skyblue', alpha=0.7
            )
            ax.add_patch(rect)
            
            # Add center dot
            center_x = x + sipm_size / 2.0
            center_y = y + sipm_size / 2.0
            ax.plot(center_x, center_y, 'r.', markersize=4)

    # Plot the scan positions
    # Start from -16.7 up to 16.7 in 34 steps of 1mm
    scan_x = []
    scan_y = []
    
    for i in range(35):
        # We loop 35 times since 34 steps of 1.0 from -16.7 gives up to 17.3
        # Wait, the script says: start from -16.7, 34 steps, then explicit corner 16.7 
        # Actually 16.7 - (-16.7) = 33.4. So 0 to 33 in 1.0 increments is 34 steps.
        # But let's just use the logic from generate_scan:
        pass
        
    for i in range(34):
        pos = -16.7 + i * 1.0
        scan_x.append(pos)
        scan_y.append(pos)
    
    # Finish on the exact corner
    scan_x.append(16.7)
    scan_y.append(16.7)
    # print positions with 2 decimal places 
    for i in range(len(scan_x)):
        print(f"{scan_x[i]:.2f}, {scan_y[i]:.2f}")
    
    ax.plot(scan_x, scan_y, marker='o', color='purple', linestyle='None', 
            markersize=6, label='Scan Position')


    # Set axes limits with some margin
    margin = 3.0
    ax.set_xlim(-total_size/2 - margin, total_size/2 + margin)
    ax.set_ylim(-total_size/2 - margin, total_size/2 + margin)
    
    ax.set_aspect('equal')
    ax.set_xlabel('X Position (mm)')
    ax.set_ylabel('Y Position (mm)')
    ax.set_title(f'{grid_size}x{grid_size} SiPM Array & Diagonal Scan Trajectory')
    
    # Display a background grid
    ax.grid(True, linestyle='--', alpha=0.5)
    
    # Emphasize the (0,0) center axes
    ax.axhline(0, color='black', linewidth=0.8)
    ax.axvline(0, color='black', linewidth=0.8)
    
    ax.legend(loc='upper left')
    
    plt.tight_layout()
    plt.savefig('sipm_scan_positions.png', dpi=300)
    print("Plot saved as 'sipm_scan_positions.png'")

if __name__ == "__main__":
    plot_sipms_and_scan()
