//
// Created by Thoma on 27/09/2022.
//

#ifndef POINTZALIVE_GRAVITATOR_H
#define POINTZALIVE_GRAVITATOR_H
#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "../Point.h"
#include <thread>

namespace gravitator{

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
}


#endif //POINTZALIVE_GRAVITATOR_H
