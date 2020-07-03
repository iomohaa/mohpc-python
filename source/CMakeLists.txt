# CMakeList.txt : CMake project for MOHPCPy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SRCS "*.cpp" "*.h")

find_package(pybind11 CONFIG REQUIRED)
find_package(MOHPC REQUIRED)

# Add source to this project's executable.
pybind11_add_module (MOHPCPy ${SRCS})

add_executable(MOHPCPyexe "MOHPCPy.cpp")
add_dependencies(MOHPCPy MOHPC::MOHPC)

target_include_directories(MOHPCPy PUBLIC ${pybind11_INCLUDE_DIRS} ".")
target_link_libraries(MOHPCPy PRIVATE MOHPC::MOHPC pybind11::module)

install(
	TARGETS MOHPCPy MOHPCPyexe
	LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

