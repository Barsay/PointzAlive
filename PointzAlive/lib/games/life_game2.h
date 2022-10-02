#ifndef PROGRAMNAMEEXAMPLE_LIFE_GAME_H
#define PROGRAMNAMEEXAMPLE_LIFE_GAME_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <functional>
#include "../Point.h"
#include "../Space.h"
#include <map>


namespace lifeGame {

    static int numberOfTypes = 4;

    //R G B A
    static float rgbwMap[6][4] = {
            {0,0,1,1},
            {0,1,1,1},
            {0,1,0,1},
            {1,1,1,1},
            {1,0,1,1},
            {1,1,0,1}
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
                points.emplace_back(rand() % myMap->getWidth(),rand() % myMap->getHeight(),(int)rand()%5-2 ,(int)rand()%5-2,
                                    std::pair<int, sf::Color>(i,sf::Color(ImGui::ColorConvertFloat4ToU32(ImVec4(lifeGame::rgbwMap[i][0],lifeGame::rgbwMap[i][1],lifeGame::rgbwMap[i][2],lifeGame::rgbwMap[i][3])))));
                                    //std::pair<int, sf::Color>(i, sf::Color(255*lifeGame::rgbwMap[i][0],255*lifeGame::rgbwMap[i][1],255*lifeGame::rgbwMap[i][2], 255*lifeGame::rgbwMap[i][3])));
            }
        }
        return points;
    };
    static bool isGravitator = false;

    const static char* gravitatorOn (bool grav)  {
        return grav ? "gravitator Off" : "gravitator On";
    }

    static std::vector<std::vector<float>> rules = {
            {-0.3,-0.3,0.8,-1,0.0,0.0},
            {-1,-0.3,-0.3,0.8,0.0,0.0},
            {0.8,-1,-0.3,-0.3,0.0,0.0},
            {-0.3,0.8,-1,-0.3,0.0,0.0},
            {0.0,0.0,0.0,0.0,0.0,0.0},
            {0.0,0.0,0.0,0.0,0.0,0.0}
    };

    //ciano - Verde attrazione
    //magenta - Blue attrazione
    /*
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
*/


    bool updateVelocity(Point &P, std::vector<Point> &pvector, double vx, double vy){
        double vvx = vx, vvy = vy;
        double distancex, distancey, d, F=0, fx=0, fy=0;
        for (auto &p: pvector) {

            //distance on x
            distancex = P.getPosition()[0] - p.getPosition()[0];
            //distance on yW
            distancey = P.getPosition()[1] - p.getPosition()[1];
            d = sqrt((distancex*distancex)+(distancey*distancey));

            if(d!=0  && d<150 &&d>2) {

                F = -lifeGame::rules[P.getColor().first][p.getColor().first]* (3) / ((d * d));
                //std::cout << "F = " << F <<endl;
               // F = -lifeGame::rule(P, p) * (3) / ((d * d));
                fx = fx + F * distancex;
                fy = fy + F * distancey;
            }
        }
        //gravitator options
        if(lifeGame::isGravitator && d!=0  && d<250 &&d>2 && ImGui::IsMouseClicked(ImGuiMouseButton_Left,true)) {
            distancex = P.getPosition()[0] - ImGui::GetMousePos().x;
            //distance on yW
            distancey = P.getPosition()[1] -ImGui::GetMousePos().y;

            F = -(2) / (d);
            fx = F * distancex;
            fy = F * distancey;
        }
        if(lifeGame::isGravitator && d!=0  && d<250 &&d>2 && ImGui::IsMouseClicked(ImGuiMouseButton_Right,true)) {
            distancex = P.getPosition()[0] - ImGui::GetMousePos().x;
            //distance on yW
            distancey = P.getPosition()[1] -ImGui::GetMousePos().y;
            F = +(2) / (d);
            fx = F * distancex;
            fy = F * distancey;
        }
        vvx = (vvx+fx);
        vvy = (vvy+fy);

        P.setVelocity({static_cast<float>(vvx-vvx*0.03), static_cast<float>(vvy-vvy*0.03)});

        return true;
    }

    static std::function<void(Space *)> calculate = [](Space * myMap) {

        if(myMap->nthreads > 1 && myMap->points.size() > myMap->nthreads){
            std::vector<shared_ptr<std::thread>> threads;

            //divide number of points in equal numbers
            int total = (int ) myMap->points.size()/myMap->nthreads;
            int resto = myMap->points.size()%myMap->nthreads;

            //cout << "total points " << points.size() << " total " <<total<< " resto " << resto <<endl;
            threads.resize(myMap->nthreads);

            int start=0, stop =total-1;
            for (int i=0; i<myMap->nthreads;i++){
                //last iteration add resto. this will result in a longer last vector but ok for now
                if (i==myMap->nthreads-1){
                    stop = myMap->points.size()-1;
                }

                threads[i]= make_shared<std::thread>([](int start, int stop, vector<Point> &pointsVector, Space * mySpace){

                    //cout << "heh tread" <<endl;

                    for (int j=start; j<=stop;j++){
                        lifeGame::updateVelocity(mySpace->points[j], mySpace->points, mySpace->points[j].getVelocity()[0], mySpace->points[j].getVelocity()[1]);
                    }
                },start, stop, myMap->points, myMap );

                start =stop+1;
                stop +=total;
            }

            int k =0;
            for(auto &thread : threads ){
                thread->join();
                k++;
            }
        } else{
        for (auto &p: myMap->points) {
            lifeGame::updateVelocity(p, myMap->points, p.getVelocity()[0], p.getVelocity()[1]);
        }
        }
    };
};


#endif //PROGRAMNAMEEXAMPLE_LIFE_GAME_H
