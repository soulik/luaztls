cmake_minimum_required (VERSION 3.1.0)
project (luaztls)

list(APPEND CMAKE_MODULE_PATH "${luaztls_SOURCE_DIR}/cmake")

set(LUAZMQ_USE_LIBSODIUM ON CACHE BOOL "Build and use internal copy of libsodium")

# Include ztls

add_subdirectory(dependencies/ztls)
include_directories(dependencies/ztls/src)

set_target_properties(ztls
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR}/bin"
)

# Include Lua 5.1

include(FindLua51)
include_directories(SYSTEM
	${LUA_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}/lua5.1
)

include_directories(dependencies/lutok2/include)

add_subdirectory(src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set_target_properties(luaztls PROPERTIES PREFIX "")
