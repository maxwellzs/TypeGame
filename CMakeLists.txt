cmake_minimum_required(VERSION 3.27)
project(TypeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:\\Qt\\6.6.1\\mingw_64")

include_directories(VNet)
include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(TypeGame main.cpp
        UI/MainWindow.cpp
        UI/MainWindow.h
        Game/GameEngine.cpp
        Game/GameEngine.h
        Game/Packets/GameCreatePacket.cpp
        Game/Packets/GameCreatePacket.h
        Game/Handlers/GameCreateListener.cpp
        Game/Handlers/GameCreateListener.h
        Game/GameTick.cpp
        Game/GameTick.h
        UI/GameWindow.cpp
        UI/GameWindow.h
        Game/GameLogics.cpp
        Game/GameLogics.h
        Game/Packets/GameTickPacket.cpp
        Game/Packets/GameTickPacket.h
        Game/Handlers/GameTickListener.cpp
        Game/Handlers/GameTickListener.h
        Game/Handlers/IntervalTickSendHandler.cpp
        Game/Handlers/IntervalTickSendHandler.h
        UI/SinglePlayerWIndow.cpp
        UI/SinglePlayerWIndow.h
)
target_link_libraries(TypeGame
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Ws2_32.dll
        libVNetBase.dll
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
