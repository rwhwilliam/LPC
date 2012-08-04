For Villages Compiling: 
- Have SDL 1.2 installed in the 'default' location (http://www.libsdl.org/download-1.2.php)
- Have SDL-image installed - http://www.libsdl.org/projects/SDL_image/
- have SDL-ttf installed - http://www.libsdl.org/projects/SDL_ttf/
- have SDL_mixer installed - http://www.libsdl.org/projects/SDL_mixer/

-Install CMake (http://www.cmake.org/cmake/resources/software.html)

- Extract Villages.zip
- Open CMake
- For the path to source, use the base directory of the villages directory (ie, this directory has the src, assests, data, etc folders)
- For the build directory, choose the build/linux/ directory
- Click the compile button
- Select Unix Makefiles
- Keep 'Use default native compilers' option selected
- click Finish

If you installed SDL in the base directory, all the paths should auto-resolve. If for some reason they do not, then you will have to browse for the proper paths.

- Click Generate

Navigate to the build/linux directory with a command line

*note, as part of the cmake generate command, the data/ folder should have been copied in here*

- use the make command with no additional parameters

* there are a few warnings, but there should be no errors*

- Execute Villages