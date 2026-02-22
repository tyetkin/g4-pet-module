import matplotlib.pyplot as plt
import matplotlib.patches as patches

def plot_sipms():
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

    # Set axes limits with some margin
    margin = 2.0
    ax.set_xlim(-total_size/2 - margin, total_size/2 + margin)
    ax.set_ylim(-total_size/2 - margin, total_size/2 + margin)
    
    ax.set_aspect('equal')
    ax.set_xlabel('X Position (mm)')
    ax.set_ylabel('Y Position (mm)')
    ax.set_title(f'{grid_size}x{grid_size} SiPM Array Positions')
    
    # Display a background grid
    ax.grid(True, linestyle='--', alpha=0.5)
    
    # Emphasize the (0,0) center axes
    ax.axhline(0, color='black', linewidth=0.8)
    ax.axvline(0, color='black', linewidth=0.8)
    
    plt.tight_layout()
    plt.savefig('sipm_positions.png', dpi=300)
    print("Plot saved as 'sipm_positions.png'")
    plt.show()

if __name__ == "__main__":
    plot_sipms()
