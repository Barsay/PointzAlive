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
    int rule(Point &p1, Point &p2){
        if (p1.getColor()==sf::Color::Cyan){
            if (p2.getColor()==sf::Color::Cyan){
                return -1;
            }
            if (p2.getColor()==sf::Color::Blue){
                return +1;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return 1;
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
                return +1;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return 1;
            }
            if (p2.getColor()==sf::Color::Green){
                return -1;
            }

        }
        if (p1.getColor()==sf::Color::Blue){
            if (p2.getColor()==sf::Color::Cyan){
                return +1;
            }
            if (p2.getColor()==sf::Color::Blue){
                return -1;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return -1;
            }
            if (p2.getColor()==sf::Color::Green){
                return +1;
            }
        }
        if (p1.getColor()==sf::Color::Magenta){
            if (p2.getColor()==sf::Color::Cyan){
                return +1;
            }
            if (p2.getColor()==sf::Color::Blue){
                return -1;
            }
            if (p2.getColor()==sf::Color::Magenta){
                return -1;
            }
            if (p2.getColor()==sf::Color::Green){
                return +1;
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

            if(d!=0  &&d<80) {

                //d= rad (dx^2+dy^2)

                F = -lifeGame::rule(P,p)*10/(d*d);
                fx = F*distancex;
                fy = F*distancey;
                vvx = (vvx+fx)*0.5;
                vvy = (vvy+fy)*0.5;
    /*            std::cout << "has distancex " << distancex << " distancey " << distancey
                          << " F " << F << " fx " << fx << " fy " << fy
                          << " vx <<" << vvx << " vy  " << vvy << " with d= " << d << ", F= " << F << std::endl;
*/
                P.setVelocity({static_cast<float>(vvx), static_cast<float>(vvy)});

            }

            /*
            P.setVelocity({distancex != 0 ? (float) P.getVelocity()[0] + 1 / (10*distancex) : (float) P.getVelocity()[0],
                           distancey != 0 ? (float) P.getVelocity()[1] + 1 / (10*distancey) : (float) P.getVelocity()[1]});
            */
        }
        return true;
    }


}
#endif //PROGRAMNAMEEXAMPLE_LIFE_GAME_H
