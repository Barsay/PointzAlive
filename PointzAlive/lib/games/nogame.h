//
// Created by Thoma on 30/09/2022.
//

#ifndef POINTZALIVE_NOGAME_H
#define POINTZALIVE_NOGAME_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "../Point.h"

namespace nogame{

    static std::function<std::vector<Point>(map *, int & )> init = [](map *myMap, int &a){
        std::vector<Point> v;
        return v;
    };

}

#endif //POINTZALIVE_NOGAME_H
