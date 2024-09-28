#ifndef PROGRAMNAMEEXAMPLE_MAP_H
#define PROGRAMNAMEEXAMPLE_MAP_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <utility>
#include <vector>
#include "Point.h"
#include <thread>
#include <functional>


using namespace std;

class Space {
private:
    int height;
    int width;
    sf::RenderWindow *graphicwindow;
    bool debug;

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


public:
    int nthreads;
    std::vector<sf::Color> colorMap;
    std::vector<Point> points;

    enum colors{
        FIRST,
        SECOND,
        THIRD,
        FORTH,
        FIFTH,
        SIXTH,
        COLOR_MAX
    };

    Space(sf::RenderWindow * window, bool debug): graphicwindow(window), debug(debug), nthreads(4){
        height= graphicwindow->getSize().y;
        width= graphicwindow->getSize().x;
        points.reserve(0);

        //init ColorMap
        colorMap.resize(COLOR_MAX);

        colorMap[FIRST] = sf::Color::Blue;
        colorMap[SECOND] = sf::Color::Cyan;
        colorMap[THIRD] = sf::Color::Green;
        colorMap[FORTH] = sf::Color::Magenta;
        colorMap[FIFTH] = sf::Color::White;
        colorMap[SIXTH] = sf::Color::Yellow;

    }

    void update( std::function<void(Space *)> fn){
        fn(this);
        for (auto &point: points) {
            checkbound(point);
            point.move();
        }

    }

    /**
     * Inits the points vector inside map.
     * @param fn function that returns the vector of points.
     */
    void init(std::vector<Point> ptV){
        points.clear();
        points = std::move(ptV);
    }

    void calculate(){
    }

    void show(){
        for(auto & point : points ){
            point.show(graphicwindow);
        }
    }

    int getHeight() const {
        return height;
    }

    int getWidth() const {
        return width;
    }


};
#endif //PROGRAMNAMEEXAMPLE_MAP_H
