#ifndef POINTZALIVE_BALLPIT_H
#define POINTZALIVE_BALLPIT_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "../Point.h"
#include "../Space.h"

namespace ballpit {

    static int numberOfTypes = 4;

    static std::function<std::vector<Point>()> init(){
        return nullptr;
    };

    static std::function<bool(std::vector<int> &a, std::vector<int> &b)> compareColorNumberVectors = [](std::vector<int> &a, std::vector<int> &b){
       for (int i=0; i<a.size(); i++) {
           if (a[i] != b[i]) return true;
       }
        return false;
    };

    static std::function<std::vector<Point>(Space *myMap, std::vector<int> &newColors)> newColorsRandomPosRandomVel = [](Space *myMap, std::vector<int> &newColors){
        std::vector<Point> points;

        int totalPoints=0;
        for(int i=0; i<newColors.size();i++){
            totalPoints = totalPoints + newColors[i];
        }
        points.reserve(totalPoints);

        for (int i=0; i<newColors.size();i++){
            for (int j=0;j<newColors[i]; j++){
                points.emplace_back(rand() % myMap->getWidth(),rand() % myMap->getHeight(),(int)rand()%5-2 ,(int)rand()%5-2, std::pair<int, sf::Color>(i, myMap->colorMap[i]));
            }
        }
        return points;
    };

    static std::function<void(Space *)> calculate = [](Space *){
        return;
    };


}
#endif //POINTZALIVE_BALLPIT_H
