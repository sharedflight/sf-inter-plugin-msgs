# Sample Cmake...

cmake_minimum_required(VERSION 3.9)
project(sharedflightmessagingexample)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)

if(WIN32)
	set(PLAT_SHORT "win64")
	set(PLAT_LONG "win-64")
	set(PLAT_ONLY "win")
	set(PLUGIN_BIN_OUTDIR "win_x64")
elseif(APPLE)
	set(PLAT_SHORT "mac64")
	set(PLAT_LONG "mac-64")
	set(PLAT_ONLY "mac")
	set(PLUGIN_BIN_OUTDIR "mac_x64")
else()
	set(PLAT_SHORT "lin64")
	set(PLAT_LONG "linux-64")
	set(PLAT_ONLY "linux")
	set(PLUGIN_BIN_OUTDIR "lin_x64")
endif()

set(XPSDK "./sdk")

set(ALL_SRC
    example_plugin.cpp
    ../../includes/SharedFlightMessaging.h)
LIST(SORT ALL_SRC)

add_library(sharedflightmessagingexample SHARED ${ALL_SRC})

include_directories(sharedflightmessagingexample PUBLIC
    "${XPSDK}/CHeaders/XPLM"
    "${XPSDK}/CHeaders/Widgets"
    "${XPSDK}/CHeaders/Wrappers"
    "../../includes"
)

#compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wextra \
    --std=c11 -Wno-missing-field-initializers -fvisibility=hidden")
if(UNIX AND NOT APPLE)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format-truncation \
	    -fno-gnu-unique")
endif()

add_definitions(-D_GNU_SOURCE -D_FILE_OFFSET_BITS=64)
# Needed for C11's gmtime_s
add_definitions(-D__STDC_WANT_LIB_EXT1__=1)

add_definitions(-DXPLM200=1 -DXPLM210=1 -DXPLM300=1 -DXPLM301=1 -DXPLM302=1)
add_definitions(-DXPLM_DEPRECATED)

if(WIN32)
	add_definitions(-DAPL=0 -DIBM=1 -DLIN=0)
elseif(APPLE)
	add_definitions(-DAPL=1 -DIBM=0 -DLIN=0)
else()
	add_definitions(-DAPL=0 -DIBM=0 -DLIN=1)
endif()

set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} -DDEBUG -g \
    -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -O0 -g")

# linking
# Careful: order matters in target_link_libraries. Dependencies must be
# ordered as first symbol callers and then as symbol providers (i.e.
# libpng16 depends on libz, so linking must be first: libpng, then libz).
#link_libraries(m)

if(WIN32)
	find_library(XPLM_LIBRARY XPLM_64 "${XPSDK}/Libraries/Win")
elseif(APPLE)
	find_library(XPLM_LIBRARY XPLM "${XPSDK}/Libraries/Mac")
else()
	set(XPLM_LIBRARY "")
endif()

if(APPLE)
	set(MATH_LIBRARY "")
	set(LIBC_NONSHARED "")
elseif(UNIX)
	find_library(MATH_LIBRARY "m")
	find_library(LIBC_NONSHARED c_nonshared)
endif()

if(APPLE)
	find_library(OPENGL_FRAMEWORK OpenGL)
	find_library(IOSURFACE_FRAMEWORK IOSurface)
	set(EXTRA_PLATFORM_LIBS
	    ${OPENGL_FRAMEWORK}
	    ${IOSURFACE_FRAMEWORK})
else()
	set(EXTRA_PLATFORM_LIBS "")
endif()

target_link_libraries(sharedflightmessagingexample
    ${XPLM_LIBRARY}
    ${EXTRA_PLATFORM_LIBS}
    ${MATH_LIBRARY}
    ${LIBC_NONSHARED}
    )

set_target_properties(sharedflightmessagingexample PROPERTIES PREFIX "")
set_target_properties(sharedflightmessagingexample PROPERTIES SUFFIX "")
set_target_properties(sharedflightmessagingexample PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/bin/${PLUGIN_BIN_OUTDIR}" )
set_target_properties(sharedflightmessagingexample PROPERTIES LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/bin/${PLUGIN_BIN_OUTDIR}" )
set_target_properties(sharedflightmessagingexample PROPERTIES OUTPUT_NAME "sharedflightmessagingexample.xpl")

if(UNIX AND NOT APPLE)
	set_target_properties(sharedflightmessagingexample PROPERTIES LINK_FLAGS
	    "${CMAKE_SHARED_LINKER_FLAGS} -rdynamic -nodefaultlibs \
	    -undefined_warning -fPIC -fvisibility=hidden -fno-gnu-unique")
endif()