# g4-phodet-array

A Geant4 application for a sparse photo-detector readout array.


## Visual Studio Code Settings

Ensure that the include path is correctly set in the .vscode/c_cpp_properties.json file in your workspace. If this file doesn't exist, you might need to create it.

```
{
    "configurations": [
      {
        "name": "Linux",
        "includePath": [
          "/home/tyetkin/sw/geant4-v11.1.2-install/include/Geant4",
          "/home/tyetkin/sw/root_install/include",
          "${workspaceFolder}/**"
        ]
        // Other configurations...
      }
    ],
    "version": 4
  }
  
```