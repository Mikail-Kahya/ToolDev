cmake_minimum_required(VERSION 3.16)
project(MinecraftGen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#TODO: include targetss
add_subdirectory(MinecraftLib)
add_subdirectory(MinecraftCmd)
add_subdirectory(MinecraftForm)

#TODO: link library
target_link_libraries(MinecraftCmd PRIVATE MinecraftLib)
target_link_libraries(MinecraftForm PRIVATE MinecraftLib)


#TODO: set install definitions
include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_Minor "0")
set(CPACK_PACKAGE_VERSION_Patch "0")

set(CPACK_NSIS_MODIFTY_PATH on)

include(CPack)