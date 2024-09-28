project("nogame" CXX)

add_library(${PROJECT_NAME}
        #../../lib/common_include/template_game.h
        #../../lib/common_include/template_game.cpp
        nogame.cpp
        nogame.h)



include_directories(../../lib/common_include/)