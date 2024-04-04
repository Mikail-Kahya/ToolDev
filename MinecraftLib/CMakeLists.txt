set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MinecraftLib STATIC 
	"MinecraftParser.cpp"
)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
	json
	GIT_REPOSITORY "https://github.com/nlohmann/json.git"
	GIT_TAG v3.11.3 #version
	GIT_PROGRESS TRUE
	GIT_SHALLOW  TRUE
)

FetchContent_MakeAvailable(json)


#include all information when included. Includes libs, .h, etc..)
target_include_directories(MinecraftLib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(MinecraftLib PRIVATE nlohmann_json::nlohmann_json)