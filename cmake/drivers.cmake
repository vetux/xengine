option(DRIVER_GLFW "Build the glfw display driver" OFF)
option(DRIVER_GLFW_OPENGL "Build the opengl support of the glfw display driver" OFF)
option(DRIVER_OPENGL "Build the OpenGL gpu driver" OFF)
option(DRIVER_BOX2D "Build the box2d physics driver" OFF)
option(DRIVER_BULLET3 "Build the bullet3 physics driver" OFF)
option(DRIVER_OPENAL "Build the OpenAL audio driver" OFF)
option(DRIVER_FREETYPE "Build the FreeType font rendering driver" OFF)
option(DRIVER_ASSIMP "Build the AssImp resource parser driver (For 3D asset file formats)" OFF)
option(DRIVER_SNDFILE "Build the SndFile resource parser driver (For Audio file formats)" OFF)
option(DRIVER_SHADERC "Build the ShaderC shader compiler driver" OFF)
option(DRIVER_SPIRVCROSS "Build the SPIRV-Cross shader decompiler driver" OFF)
option(DRIVER_CRYPTOPP "Build the CryptoPP driver" OFF)
option(DRIVER_ANDROID "Build the android display driver" OFF)
option(DRIVER_ANDROID_OPENGL "Build the opengl support of the android display driver" OFF)

set(DRIVERS_INCLUDE) # The drivers include directories in a list
set(DRIVERS_SRC) # The drivers source files in a list
set(DRIVERS_LINK) # The drivers linked library names in a list
set(Drivers.GLOBEXPR) # The globexpr used to generate DRIVERS_SRC
set(DRIVERS_CLASSES) # The driver class names
set(DRIVERS_INCLUDES) # The driver include paths
set(DRIVERS_NAMES) # The driver names

# @COMPILE_DEFS = Compile definitions
# @DIR = The directory in drivers/ which contains the driver source
# @CLASS = The class name that this driver defines
# @INCLUDE = The include path of the driver class
# @DRIVER_LINK = The library name/s which the driver links to. There can be multiple DRIVER_LINK arguments.
function(CompileDriver COMPILE_DEFS DIR CLASS INCLUDE)
    set(Drivers.GLOBEXPR ${Drivers.GLOBEXPR} ${BASE_SOURCE_DIR}/drivers/${DIR}/*.cpp ${BASE_SOURCE_DIR}/drivers/${DIR}/*.c PARENT_SCOPE)
    add_compile_definitions(${COMPILE_DEFS})
    set(DRIVERS_INCLUDE ${DRIVERS_INCLUDE} ${BASE_SOURCE_DIR}/drivers/${DIR}/ PARENT_SCOPE)
    set(DRIVERS_CLASSES "${DRIVERS_CLASSES}${CLASS};" PARENT_SCOPE)
    set(DRIVERS_INCLUDES "${DRIVERS_INCLUDES}${INCLUDE};" PARENT_SCOPE)
    set(DRIVERS_NAMES "${DRIVERS_NAMES}${DIR};" PARENT_SCOPE)
    if (${ARGC} GREATER 4)
        # Each additional argument is treated as a library name
        set(MAXINDEX ${ARGC})
        MATH(EXPR MAXINDEX "${MAXINDEX}-1")
        foreach (index RANGE 4 ${MAXINDEX})
            list(GET ARGV ${index} LIBNAME)
            set(DRIVER_LINK ${DRIVER_LINK} ${LIBNAME})
        endforeach ()
        set(DRIVERS_LINK ${DRIVERS_LINK} ${DRIVER_LINK} PARENT_SCOPE)
    endif ()
endfunction()

### --  Start Driver Definitions -- ###

if (DRIVER_GLFW)
    CompileDriver(DRIVER_GLFW
            glfw
            glfw::GLFWDisplayDriver
            display/glfw/glfwdisplaydriver.hpp
            glfw)
endif ()

if (DRIVER_GLFW_OPENGL)
    add_compile_definitions(DRIVER_GLFW_OPENGL)
endif ()

if (DRIVER_ANDROID)
    CompileDriver(DRIVER_ANDROID
            android
            android::AndroidDisplayDriver
            display/android/androiddisplaydriver.hpp
            android
            EGL)
endif ()

if (DRIVER_ANDROID_OPENGL)
    add_compile_definitions(DRIVER_ANDROID_OPENGL)
endif ()

if (DRIVER_OPENGL)
    if (ANDROID)
        set(GL_LIBNAME GLESv3)
    elseif (WIN32)
        set(GL_LIBNAME OpenGL32)
    else ()
        set(GL_LIBNAME GL)
    endif ()
    CompileDriver(DRIVER_OPENGL
            opengl
            opengl::OGLGpuDriver
            gpu/opengl/oglgpudriver.hpp
            ${GL_LIBNAME})
endif ()

if (DRIVER_BOX2D)
    CompileDriver(DRIVER_BOX2D
            box2d
            PhysicsDriverBox2D
            physics/box2d/physicsdriverbox2d.hpp
            box2d)
endif ()

if (DRIVER_BULLET3)
    CompileDriver(DRIVER_BULLET3
            bullet3
            PhysicsDriverBt3
            physics/bullet3/physicsdriverbt3.hpp)
endif ()

if (DRIVER_OPENAL)
    if (WIN32)
        set(AL_LIBNAME OpenAL32)
    else ()
        set(AL_LIBNAME openal)
    endif ()
    CompileDriver(DRIVER_OPENAL
            openal-soft
            OALAudioDriver
            audio/openal/oalaudiodriver.hpp
            ${AL_LIBNAME})
endif ()

if (DRIVER_FREETYPE)
    CompileDriver(DRIVER_FREETYPE
            freetype
            FtFontDriver
            text/freetype/ftfontdriver.hpp
            freetype)
endif ()

if (DRIVER_ASSIMP)
    CompileDriver(DRIVER_ASSIMP
            assimp
            AssImpParser
            resource/parsers/assimpparser.hpp
            assimp)
endif ()

if (DRIVER_SNDFILE)
    CompileDriver(DRIVER_SNDFILE
            sndfile
            SndFileParser
            resource/parsers/sndfileparser.hpp
            sndfile)
endif ()

if (DRIVER_SHADERC)
    CompileDriver(DRIVER_SHADERC
            shaderc
            ShaderCCompiler
            shader/shaderccompiler.hpp
            shaderc_combined)
endif ()

if (DRIVER_SPIRVCROSS)
    CompileDriver(DRIVER_SPIRVCROSS
            spirv-cross
            SpirvCrossDecompiler
            shader/spirvcrossdecompiler.hpp
            spirv-cross-core spirv-cross-glsl spirv-cross-hlsl)
endif ()

if (DRIVER_CRYPTOPP)
    CompileDriver(DRIVER_CRYPTOPP
            cryptopp
            CryptoPPDriver
            crypto/cryptoppdriver.hpp
            cryptopp)
endif ()

### --  Stop Driver Definitions -- ###

set(DRIVERS_REGISTRATION_HEADER)
set(DRIVERS_REGISTRATION)

list(LENGTH DRIVERS_CLASSES DRIVERS_CLASSES_LEN)
if (${DRIVERS_CLASSES_LEN} GREATER 0)
    foreach (index RANGE 0 ${DRIVERS_CLASSES_LEN})
        list(GET DRIVERS_CLASSES ${index} CLASS)
        list(GET DRIVERS_INCLUDES ${index} INCLUDE)
        list(GET DRIVERS_NAMES ${index} NAME)
        if (CLASS STREQUAL "")
            break()
        endif ()
        set(DRIVERS_REGISTRATION_HEADER "${DRIVERS_REGISTRATION_HEADER}#include \"${INCLUDE}\"\n")
        set(DRIVERS_REGISTRATION "${DRIVERS_REGISTRATION}{\"${NAME}\", [](){ return std::make_unique<${CLASS}>()\; }},")
    endforeach ()
endif ()

set(DRIVERS_GENERATOR "${DRIVERS_REGISTRATION_HEADER}\nnamespace xng::DriverGenerator { inline const std::map<std::string, Driver::Creator> &getDrivers() { static const std::map<std::string, Driver::Creator> creators = {${DRIVERS_REGISTRATION}}\; return creators\; } }")

# Write the header file included by engine/src/driver/driver.cpp
file(WRITE ${BASE_SOURCE_DIR}/engine/src/compiled_drivers.h ${DRIVERS_GENERATOR})

set(STR_LIBRARIES "")
foreach (val IN LISTS DRIVERS_LINK)
    set(STR_LIBRARIES "${STR_LIBRARIES}${val} ")
endforeach ()

list(LENGTH DRIVERS_INCLUDE LEN_DRIVERS_INCLUDE)

message("${LEN_DRIVERS_INCLUDE} Drivers Loaded")
foreach (val IN LISTS DRIVERS_INCLUDE)
    message("   ${val}")
endforeach ()

if (${LEN_DRIVERS_INCLUDE} GREATER 0)
    message("Linked Libraries: ${STR_LIBRARIES}")
endif ()

file(GLOB_RECURSE DRIVERS_SRC ${Drivers.GLOBEXPR})
