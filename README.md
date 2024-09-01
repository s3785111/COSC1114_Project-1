## COSC1114 Operating Systems Principals Assignment 1

 - Author: Matthew Ricci
 - Student Number: s3785111

### Build Instructions
To build all components of the project, navigate to the root directory (A1/) and run one of the following commands:

```bash
make                   # Standard build
make "CFLAGS=-Werror"  # Build with -Werror
make "CFLAGS=-D DEBUG" # Build with debug output
```

### Executing the binaries
Subprojects may be individually built by navigating to their respective directories (mmcopier/ or mscopier/) and invoking ```make```.

After building, the mmcopier executable will be located in the mmcopier/bin directory. To run it, use:

```bash
./mmcopier/bin/mmcopier [thread count] [input dir] [output dir]
```

Similarly, after building, the mscopier executable will be located in the mscopier/bin directory. To run it, use:

```bash
./mscopier/bin/mscopier [thread count] [input file] [output file]
```
##### A note on I/O paths  
Both projects additionally have their binaries symbolically linked within the top level ```/bin``` located in root for convenience.

For either binary, the input and output files/directories *must* be provided as relative to the working directory from which you invoke it. For example, if you choose to invoke ```mscopier``` from the root directory with ```./bin/mscopier```, then the input files/directories must be relative to the root directory.

