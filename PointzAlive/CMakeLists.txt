﻿cmake_minimum_required (VERSION 3.8)

# Packages (Dependencies)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable (${PROJECT_NAME} "main.cpp" lib/Game.h lib/Space.h ./lib/point.h ./lib/games/life_game2.h  ./lib/games/gravitator2.h lib/games/nogame.h lib/games/BallPit.h)

# sub directories
#add_subdirectory("assets")

# Dependencies Targets
target_link_libraries(${PROJECT_NAME} PRIVATE
        ImGui-SFML::ImGui-SFML
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
        )

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)
