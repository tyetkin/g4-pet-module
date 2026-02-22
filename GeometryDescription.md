# Detector Geometry and Optical Setup

The simulations are performed to study and characterize the properties of monolithic scintillators within PET detectors. The framework relies on Geant4 for a precise optical simulation of the gamma-ray interactions and subsequent scintillation processes. 

## Scintillator and Photodetector
The scintillator investigated is a single monolithic crystal structured to have a surface area of 33.4 × 33.4 mm² and a height of 10 mm. 

The light output is read out by an 8 × 8 array of Silicon Photomultipliers (SiPMs) positioned at the bottom face of the monolith. Each individual SiPM comprises an active sensing area of 3 × 3 mm² enclosed within a physical footprint of 4 × 4 mm² with a thickness of 1.4 mm. The sensors are arranged in a grid with a pitch of 0.2 mm. This configuration precisely blankets the 33.4 × 33.4 mm² bottom surface of the detector block. The coupling between the inner scintillator surface and the SiPM matrix is mediated by a 20 μm thick layer of optical silicone grease to minimize the refractive index mismatch and prevent total internal reflection at the exit face.

## LUT DAVIS Reflection Models
To simulate surface reflections with high physical accuracy, the optical boundaries are modeled natively using the LUT DAVIS model (Stockhoff et al., 2017), replacing standard analytical approximations like the UNIFIED model. The remaining non-readout faces of the crystal (the top and four side faces) are encapsulated by a 0.6 mm thick boundary. Three distinct crystal coating configurations are simulated to investigate their disparate impacts on spatial position estimation and light collection efficiency:
- **Black on all faces (B)**: Black faces are mathematically modelled using the `Rough_LUT` finish under the DAVIS model. A custom material property table enforces 0% reflectivity to ensure total light absorption while strictly preserving the angular scatter distribution at the rough boundary.
- **Reflective on all faces (R)**: Reflective faces are modelled as polished surfaces with optical grease and an Enhanced Specular Reflector (ESR). In Geant4, this is precisely captured by utilizing the `PolishedESRGrease_LUT` finish from the empirical DAVIS datasets.
- **Black sides and Reflective top (BR)**: A hybrid configuration where the side walls act as rough absorbers and the top surface acts as a specular reflector.

## Primary Tracking Reference
To establish a concrete absolute reference (ground truth) for evaluating the intrinsic 3D spatial resolution of the detector system, a 0.5 mm thick primary particle counter is deployed 1.0 mm directly in front of the top coating. Designed as a dedicated sensitive detector filled with air, this volume accurately registers the exact impact coordinates and initial energies of incident primary gamma particles moments before they enter the scintillator interaction matrix.
