#ifndef PROGRAMNAMEEXAMPLE_LIFE_GAME_H
#define PROGRAMNAMEEXAMPLE_LIFE_GAME_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <functional>
#include "../Point.h"
#include "../map.h"


namespace lifeGame {


    static std::function<bool(std::vector<int> &a, std::vector<int> &b)> compareColorNumberVectors = [](std::vector<int> &a, std::vector<int> &b){
        for (int i=0; i<a.size(); i++) {
            if (a[i] != b[i]) return true;
        }
        return false;
    };

    static std::function<std::vector<Point>(map *myMap, std::vector<int> &newColors)> newColorsRandomPosRandomVel = [](map *myMap, std::vector<int> &newColors){
        std::vector<Point> points;

        int totalPoints=0;
        for(int i=0; i<newColors.size();i++){
            totalPoints = totalPoints + newColors[i];
        }
        points.reserve(totalPoints);

        for (int i=0; i<newColors.size();i++){
            for (int j=0;j<newColors[i]; j++){
                points.emplace_back(rand() % myMap->getWidth(),rand() % myMap->getHeight(),(int)rand()%5-2 ,(int)rand()%5-2, myMap->colorMap[i]);
            }
        }
        return points;
    };

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

                F = -lifeGame::rule(P, p) * (3) / ((d * d));
                fx = fx + F * distancex;
                fy = fy + F * distancey;
            }
        }
        vvx = (vvx+fx);
        vvy = (vvy+fy);

        P.setVelocity({static_cast<float>(vvx-vvx*0.03), static_cast<float>(vvy-vvy*0.03)});

        return true;
    }

    static std::function<void(map *)> calculate = [](map * myMap) {

        /* TODO: add threads
        if(myMap->nthreads > 1 && points.size() > myMap->nthreads){
            std::vector<shared_ptr<std::thread>> threads;

            //divide number of points in equal numbers
            int total = (int ) points.size()/myMap->nthreads;
            int resto = points.size()%myMap->nthreads;

            //cout << "total points " << points.size() << " total " <<total<< " resto " << resto <<endl;
            threads.resize(myMap->nthreads);

            int start=0, stop =total-1;
            for (int i=0; i<myMap->nthreads;i++){
                //last iteration add resto. this will result in a longer last vector but ok for now
                if (i==myMap->nthreads-1){
                    stop = points.size()-1;
                }

                threads[i]= make_shared<std::thread>([](int start, int stop, vector<Point> &pointsVector, map * myMap){

                    //cout << "heh tread" <<endl;

                    for (int i=start; i<=stop;i++){
                        lifeGame::updateVelocity(const_cast<Point &>(myMap->getPoints()[i]), pointsVector, myMap->getPoints()[i].getVelocity()[0], myMap->getPoints()[i].getVelocity()[1]);

                    }
                },start, stop, points, myMap );

                start =stop+1;
                stop +=total;
            }

            int i =0;
            for(auto &thread : threads ){
                thread->join();
                //cout <<"stopped thread " << i<<endl;
                i++;
            }
        } else{ */
        for (auto &p: myMap->points) {
            lifeGame::updateVelocity(p, myMap->points, p.getVelocity()[0], p.getVelocity()[1]);
        }
        /*} */
    };
};


#endif //PROGRAMNAMEEXAMPLE_LIFE_GAME_H
