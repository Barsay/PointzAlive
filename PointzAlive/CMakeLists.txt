cmake_minimum_required (VERSION 3.8)

# Packages (Dependencies)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

include(utils/cmake/Functions.cmake)
include(utils/cmake/colors.cmake)
include(utils/cmake/macros.cmake)


PA_init()

include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_executable (${PROJECT_NAME}
        main.cpp
        lib/pointzalive_core.h
        #lib/Space.h
        #lib/point.h
        #lib/games/life_game2.h
        #lib/games/gravitator2.h
        #games/nogame.h
        #lib/games/BallPit.h
)

add_library(common_include STATIC
        lib/common_include/Point.h
        lib/common_include/Space.h
        lib/common_include/template_game.cpp
        lib/common_include/template_game.h
)

# Aggiungi le directory di inclusione per common_include
target_include_directories(common_include PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${VCPKG_INCLUDE_DIRS})

# Aggiungi le directory di inclusione per il progetto principale
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

PA_add_game(games/nogame)

PA_generateHeaders(${PA_SUBPROJECTS_SELECTION})

# sub directories
#add_subdirectory("assets")

# Dependencies Targets


target_link_libraries(common_include PRIVATE
        ImGui-SFML::ImGui-SFML
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
        glad::glad
        glfw
        imgui::imgui
        implot::implot
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ImGui-SFML::ImGui-SFML
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
        glad::glad
        glfw
        imgui::imgui
        implot::implot
        common_include
)

#target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
#target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
#target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
#target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)


