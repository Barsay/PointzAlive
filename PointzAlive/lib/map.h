#ifndef PROGRAMNAMEEXAMPLE_MAP_H
#define PROGRAMNAMEEXAMPLE_MAP_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "Point.h"
#include "games/life_game.h"
#include <thread>

enum Games{
    NOGAME,
    BALLPIT,
    LIFE_GAME
};

using namespace std;

class map {
private:
    std::vector<Point> points;
    int height;
    int width;
    vector<int> nColors;
    sf::RenderWindow *graphicwindow;
    bool debug;
    std::vector<sf::Color> colorMap;
    int game;

    void checkbound(Point &point){
        //if left bound touched (x=0)
        if(point.getPosition()[0]<=0 ){
            point.setPosition({0,point.getPosition()[1]});
            point.setVelocity({-point.getVelocity()[0],point.getVelocity()[1]});
        }
        if( point.getPosition()[0]>=width-1 ){
            point.setPosition({(float)width-1,point.getPosition()[1]});
            point.setVelocity({-point.getVelocity()[0],point.getVelocity()[1]});

        }
        //if right bound touched (x=widht)
        if(point.getPosition()[1]<=0 ){
            point.setVelocity({point.getVelocity()[0],-point.getVelocity()[1]});
            point.setPosition({point.getPosition()[0], 0});
        }
        if(point.getPosition()[1]>=height-1 ){
            point.setVelocity({point.getVelocity()[0],-point.getVelocity()[1]});
            point.setPosition({point.getPosition()[0],(float) height-1});
        }

       // cout << "has now velocity "<<point.getVelocity()[0]<<" "<<point.getVelocity()[0]<<endl;
    }

    bool hasColorChanged(){
        for (int i=0; i< COLOR_MAX; i++){
            if(numberOfcolors[i] != nColors[i]){
                return true;
            }
        }
        return false;
    }

public:
    int nthreads;
    std::vector<int>numberOfcolors;

    enum colors{
        BLUE,
        CYAN,
        GREEN,
        MAGENTA,
        WHITE,
        COLOR_MAX
    };

    map(sf::RenderWindow * window, bool debug): graphicwindow(window), debug(debug), nthreads(4){
        height= graphicwindow->getSize().y;
        width= graphicwindow->getSize().x;
        points.reserve(0);

        //init ColorMap
        colorMap.resize(COLOR_MAX);
        colorMap[BLUE] = sf::Color::Blue;
        colorMap[CYAN] = sf::Color::Cyan;
        colorMap[GREEN] = sf::Color::Green;
        colorMap[MAGENTA] = sf::Color::Magenta;
        colorMap[WHITE] = sf::Color::White;

        nColors.resize(COLOR_MAX);
        numberOfcolors.resize(COLOR_MAX);
    }

    void update(){
        /*
        if(height != graphicwindow->getSize().y ||  width!= graphicwindow->getSize().x){
            height= graphicwindow->getSize().y;
            width= graphicwindow->getSize().x;
        }
         */

        if (hasColorChanged()){
            points.clear();
            nColors = numberOfcolors;
            init();
        }
    }

    void init(){
        int totalPoints=0;
        for(int i=0; i<COLOR_MAX;i++){
            totalPoints = totalPoints + nColors[i];
        }
        points.reserve(totalPoints);

        for (int i=0; i<COLOR_MAX;i++){
            for (int j=0;j<nColors[i]; j++){
                points.emplace_back(rand() % width,rand() % height,(int)rand()%5-2 ,(int)rand()%5-2, colorMap[i]);
            }
        }
    }

    void calculate(int game){
        this->game = game;
        if (game == BALLPIT) {
            for (auto &point: points) {
                checkbound(point);
                point.move();
            }


        } else if (game == LIFE_GAME){
            if(nthreads > 1 && points.size() > nthreads){
                std::vector<shared_ptr<std::thread>> threads;

                //divide number of points in equal numbers
                int total = (int ) points.size()/nthreads;
                int resto = points.size()%nthreads;

                //cout << "total points " << points.size() << " total " <<total<< " resto " << resto <<endl;
                threads.resize(nthreads);

                int start=0, stop =total-1;
                for (int i=0; i<nthreads;i++){
                    //last iteration add resto. this will result in a longer last vector but ok for now
                    if (i==nthreads-1){
                        stop = points.size()-1;
                    }

                    threads[i]= make_shared<std::thread>([this](int start, int stop, vector<Point> &pointsVector){

                        //cout << "heh tread" <<endl;

                        for (int i=start; i<=stop;i++){
                            checkbound(points[i]);
                            lifeGame::updateVelocity(points[i], points, points[i].getVelocity()[0], points[i].getVelocity()[1]);
                            points[i].move();

                        }
                    },start, stop, points );

                    start =stop+1;
                    stop +=total;
                }

                int i =0;
                for(auto &thread : threads ){
                    thread->join();
                    //cout <<"stopped thread " << i<<endl;
                    i++;
                }
            } else {
                for (auto &point: points) {
                    checkbound(point);
                    lifeGame::updateVelocity(point, points, point.getVelocity()[0], point.getVelocity()[1]);
                    point.move();
            }


            }
        }
    }

    void show(){
        for(auto & point : points ){
            point.show(graphicwindow);
        }
    }

};
#endif //PROGRAMNAMEEXAMPLE_MAP_H
