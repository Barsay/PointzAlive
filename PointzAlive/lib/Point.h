#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>

#ifndef PROGRAMNAMEEXAMPLE_POINT_H
#define PROGRAMNAMEEXAMPLE_POINT_H


class Point {
private:
    sf::CircleShape shape;
    std::vector<float> position;
    std::vector<float> velocity;
    std::pair<int, sf::Color> color;
    float mass;
public:
    //constructor
    Point(float x, float y, float velx, float vely, std::pair<int, sf::Color> color):position({x,y}),velocity({velx, vely}),shape(3.f), color(color){
        shape.setPosition(position[0],position[1]);
        position.resize(5);
        shape.setFillColor(color.second);
};
    void move(){
        position[0]=position[0]+velocity[0];
        position[1]=position[1]+velocity[1];
        shape.setPosition(position[0], position[1]);
        //std::cout <<"moving "<< position[0] << " " <<position[1]<<std::endl;
    }

    void show(sf::RenderWindow *window){
        window->draw(shape);
    }


    const std::vector<float> &getPosition() const {
        return position;
    }

    void setPosition(const std::vector<float> &position) {
        Point::position = position;
    }

    std::vector<float> getVelocity() const {
        return velocity;
    }

    void setVelocity(const std::vector<float> vel) {
        Point::velocity = vel;
    }

    std::pair<int, sf::Color> &getColor() {
        return color;
    }

    const sf::CircleShape &getShape() const {
        return shape;
    }

    void setShape(const sf::CircleShape &shape) {
        Point::shape = shape;
    }

    void setColor(const std::pair<int, sf::Color> &color) {
        Point::color = color;
    }

    float getMass() const {
        return mass;
    }

    void setMass(float mass) {
        Point::mass = mass;
    }

    void setRGBColor(const ImVec4 col){
        //sf::Color c(ImGui::ColorConvertFloat4ToU32(col));
        shape.setFillColor(sf::Color((int)(col.x*255), (int)(col.y*255),(int)(col.z*255),(int)(col.w*255)));
    }

};


#endif //PROGRAMNAMEEXAMPLE_POINT_H
