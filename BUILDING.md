# Dependencies
There are git submodules for all driver dependencies provided for your convenience, which are by default built with the drivers using cmake. 
If you wish to provide your own build of the dependencies you can set the SUBMODULE_DEPENDENCIES cmake option to OFF in your user-config-project.cmake file.

# Windows / Linux Host build

- Create a file called <code>user-config-project.cmake</code> in the [project](/) directory and
  set the link_directories and include_directories for the driver dependencies. GLM headers must currently be provided for building the xEngine (GLM dependency will be
  removed in the future)
- Enable / Disable the desired [drivers](cmake/drivers.cmake) in the [user-config-project.cmake](user-config-project.cmake) file. (Eg. <code> cmake_policy(SET CMP0077 NEW)</code> <code>set(DRIVER_* ON/OFF)</code> to override the options)
- Open CMakeLists.txt in your favorite IDE or with the cmake command
- Build the desired cmake targets

# Android Cross Compile

# Windows Cross Compile - MinGW

# Notes

## GLSLang
The GLSLang shader compiler driver is written against Version 12.3.1 of glslang.

## ShaderC
Linking shaderc together with spirv-cross in the library causes spirv-cross to segfault when decompiling shaders.

## Box2D
The box2d physics driver is written against Version 2.4.1 of box2d. Make sure to not have conflicting versions of the library on
the include / link paths.
