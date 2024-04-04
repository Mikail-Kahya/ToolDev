
cmake_minimum_required(VERSION 3.16)
project(MinecraftForm LANGUAGES CXX)

#TODO: SET CORRECT QT BASE DIR HERE
set(QtBaseDir "C:/Qt/6.6.2/mingw_64")

list(APPEND CMAKE_PREFIX_PATH ${QtBaseDir}/lib/cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED Core)
find_package(Qt6 REQUIRED Gui)
find_package(Qt6 REQUIRED Widgets)

qt_add_executable(MinecraftForm
    minecraftform.cpp 
    minecraftform.ui
    main.cpp
)

set_target_properties(MinecraftForm PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(MinecraftForm PRIVATE MinecraftLib)
target_link_libraries(MinecraftForm PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)


##the following part will generate installation rules.
install(TARGETS MinecraftForm DESTINATION bin)

#TODO2: Use foreach to replace first 3 install commands
set(DLLS "Qt6Core" "Qt6Gui" "Qt6Widgets")

foreach(dll IN LISTS DLLS)
install(FILES "${QtBaseDir}/bin/${dll}.dll" DESTINATION bin)
endforeach()

#install(FILES "${QtBaseDir}/bin/Qt6Core.dll" DESTINATION bin)
#install(FILES "${QtBaseDir}/bin/Qt6Gui.dll" DESTINATION bin)
#install(FILES "${QtBaseDir}/bin/Qt6Widgets.dll" DESTINATION bin)

install(FILES "${QtBaseDir}/plugins/platforms/qwindows.dll" DESTINATION bin/platforms)