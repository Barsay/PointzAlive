#ifndef PROGRAMNAMEEXAMPLE_LIFE_GAME_H
#define PROGRAMNAMEEXAMPLE_LIFE_GAME_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "Point.h"

namespace lifeGame {

    //ciano - Verde attrazione
    //magenta - Blue attrazione
    float rule(Point &p1, Point &p2){
        if (p1.getColor()==sf::Color::Cyan){
            if (p2.getColor()==sf::Color::Cyan){
                return -0.3;
            }
            if (p2.getColor()==sf::Color::Blue){
                return +0.3;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return 0.8;
            }
            if (p2.getColor()==sf::Color::Green){
                return -1;
            }
        }
        if (p1.getColor()==sf::Color::Green){
            if (p2.getColor()==sf::Color::Cyan){
                return -1;
            }
            if (p2.getColor()==sf::Color::Blue){
                return +0.8;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return +0.3;
            }
            if (p2.getColor()==sf::Color::Green){
                return -0.3;
            }

        }
        if (p1.getColor()==sf::Color::Blue){
            if (p2.getColor()==sf::Color::Cyan){
                return +0.8;
            }
            if (p2.getColor()==sf::Color::Blue){
                return -0.3;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return -1;
            }
            if (p2.getColor()==sf::Color::Green){
                return +0.3;
            }
        }
        if (p1.getColor()==sf::Color::Magenta){
            if (p2.getColor()==sf::Color::Cyan){
                return +0.3;
            }
            if (p2.getColor()==sf::Color::Blue){
                return -1;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return -0.3;
            }
            if (p2.getColor()==sf::Color::Green){
                return +0.8;
            }
        }
    }



    bool updateVelocity(Point &P, std::vector<Point> &pvector, double vx, double vy) {
        double vvx = vx, vvy = vy;
        double distancex, distancey, d, F=0, fx=0, fy=0;
        for (auto &p: pvector) {

            //distance on x
            distancex = P.getPosition()[0] - p.getPosition()[0];
            //distance on yW
            distancey = P.getPosition()[1] - p.getPosition()[1];
            d = sqrt((distancex*distancex)+(distancey*distancey));

            if(d!=0  && d<150 &&d>2) {

                //d= rad (dx^2+dy^2);

                F = -lifeGame::rule(P, p) * (3) / ((d * d));
                fx = fx + F * distancex;
                fy = fy + F * distancey;

                /*            std::cout << "has distancex " << distancex << " distancey " << distancey
                                      << " F " << F << " fx " << fx << " fy " << fy
                                      << " vx <<" << vvx << " vy  " << vvy << " with d= " << d << ", F= " << F << std::endl;
            */
            }
        }
        vvx = (vvx+fx);
        vvy = (vvy+fy);

        P.setVelocity({static_cast<float>(vvx-vvx*0.03), static_cast<float>(vvy-vvy*0.03)});

        return true;
    }


}
#endif //PROGRAMNAMEEXAMPLE_LIFE_GAME_H
