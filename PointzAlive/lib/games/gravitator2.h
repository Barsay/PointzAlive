#ifndef POINTZALIVE_GRAVITATOR_H
#define POINTZALIVE_GRAVITATOR_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "../Space.h"
#include "../Point.h"
#include <thread>

namespace gravitator{

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

    void updateVelocity(Point &P, double vx,double vy){

        double distancex, distancey, d, F=0, fx=0, fy=0, vvx= vx, vvy=vy;
            //distance on x
        distancex = P.getPosition()[0] - ImGui::GetMousePos().x;
        //distance on yW
        distancey = P.getPosition()[1] -ImGui::GetMousePos().y;

        d = sqrt((distancex*distancex)+(distancey*distancey));

        if(d!=0  && d<250 &&d>2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left,true)) {

            F = -(2) / (d);
            fx = F * distancex;
            fy = F * distancey;
            vvx = (vvx + fx);
            vvy = (vvy + fy);
        }
        if(d!=0  && d<250 &&d>2 && ImGui::IsMouseClicked(ImGuiMouseButton_Right,true)) {
            F = +(2) / (d);
            fx = F * distancex;
            fy = F * distancey;
            vvx = (vvx + fx);
            vvy = (vvy + fy);
        }

        P.setVelocity({static_cast<float>(vvx-0.0003*vvx), static_cast<float>(vvy-0.0003*vvy)});
    }

    static std::function<std::vector<Point>()> init(){
        return nullptr;
    }

    static std::function<void(Space *)> calculate = [](Space *myMap){

        for (auto & point: myMap->points ) {
            gravitator::updateVelocity(point, point.getVelocity()[0],point.getVelocity()[1]);
        }
    };

}


#endif //POINTZALIVE_GRAVITATOR_H
