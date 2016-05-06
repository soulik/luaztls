set(luaztls_src
	main.cpp
	main.h
	common.h
)

set(luaztls_sources
	${luaztls_src}
)

include_directories(./)

set(luaztls_version_script luaztls.version)

IF(UNIX)
    IF(CMAKE_COMPILER_IS_GNUCC)
         SET(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_CXX_COMPILE_OPTIONS_VISIBILITY}hidden -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/${luaztls_version_script}")
    ENDIF(CMAKE_COMPILER_IS_GNUCC)

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations")
ENDIF(UNIX)

add_library(luaztls SHARED
	${luaztls_sources}
)

set_property(TARGET luaztls PROPERTY CXX_STANDARD 11)
set_property(TARGET luaztls PROPERTY CXX_STANDARD_REQUIRED ON)

set_target_properties(luaztls
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/bin"
)

configure_file(ztls.lua ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/lib/ztls.lua COPYONLY)

target_link_libraries (luaztls
	ztls
	${LUA_LIBRARY}
)
