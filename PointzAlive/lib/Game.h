#ifndef PROGRAMNAMEEXAMPLE_GAMESELECTOR_H
#define PROGRAMNAMEEXAMPLE_GAMESELECTOR_H

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <utility>
#include "Point.h"
#include "map.h"
#include "games/nogame.h"
#include "games/gravitator2.h"
#include "games/life_game2.h"
#include "games/BallPit.h"

#define MAX_POINT 2500

class Game {

private:
    int selection;
    map *myMap;
    vector<int> nColors;
    std::function<bool(int &, int &)> hasSelectionChanged;

public:
    Game(map *map): selection(NOGAME), myMap(map), hasSelectionChanged([](int &a, int &b){return a != b;}){

        nColors.resize(map::COLOR_MAX);

    }

    void ShowGameSettings(){
        ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar);
        ImGui::Separator();
        ImGui::Text("PointzAlive 0.0.2 - Nothing here is truly stable");
        ImGui::Separator();
        showSelectionMenu();
        showGameMenu();
        ImGui::End();
    }

    enum Games{
        NOGAME,
        BALLPIT,
        LIFE_GAME,
        GRAVITATOR
    };

    void showSelectionMenu(){
        int tempSelection = selection;

        if(ImGui::BeginMenuBar() ) {
            if (ImGui::BeginMenu("Select Game")) {
                if (ImGui::MenuItem("NoGame")) {
                    selection = NOGAME;
                    onParameterChanged(selection, tempSelection, myMap, hasSelectionChanged, nogame::init);
                } else if (ImGui::MenuItem("BallPit")) {
                    selection = BALLPIT;
                    onParameterChanged(selection, tempSelection, myMap, hasSelectionChanged, nogame::init);
                }else if (ImGui::MenuItem("Life Game")) {
                    selection = LIFE_GAME;
                    onParameterChanged(selection, tempSelection, myMap, hasSelectionChanged, nogame::init);
                }else if (ImGui::MenuItem("Gravitator")) {
                    selection = GRAVITATOR;
                    onParameterChanged(selection, tempSelection, myMap, hasSelectionChanged, nogame::init);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

    }

    void showGameMenu(){
        if(selection == NOGAME){

        }
        if (selection == BALLPIT){

            std::vector<int> temp = nColors;

            ImGui::SliderInt("Green Pointz", &nColors[map::GREEN], 0, MAX_POINT);
            ImGui::SliderInt("White Balls", &nColors[map::WHITE], 0, MAX_POINT);
            ImGui::SliderInt("Cyan Pointz", &nColors[map::CYAN], 0, MAX_POINT);
            ImGui::SliderInt("Blue Pointz", &nColors[map::BLUE], 0, MAX_POINT);
            ImGui::SliderInt("Magenta Pointz", &nColors[map::MAGENTA], 0, MAX_POINT);
            if(ImGui::Button("Randomize Colors")){
                nColors[map::GREEN] = rand()%MAX_POINT;
                nColors[map::WHITE]  = rand()%MAX_POINT;
                nColors[map::CYAN] = rand()%MAX_POINT;
                nColors[map::BLUE] = rand()%MAX_POINT;
                nColors[map::MAGENTA] = rand()%MAX_POINT;
            };
            onParameterChanged(nColors, temp, myMap ,ballpit::compareColorNumberVectors, ballpit::newColorsRandomPosRandomVel);
        }
        if(selection == LIFE_GAME){
            std::vector<int> temp = nColors;

            ImGui::Separator();
            ImGui::SliderInt("Number of different Point type", &ballpit::numberOfTypes, 1, 6);
            ImGui::Separator();
            for (int i=0;i<map::colors::COLOR_MAX;i++){
                if(ballpit::numberOfTypes>i) {
                    ImGui::SliderInt(to_string(i).c_str(), &nColors[i], 0, MAX_POINT);
                } else {
                   nColors[i] = 0;
                }
            }

            ImGui::Separator();
            ImGui::SliderInt("Number of threads", &myMap->nthreads, 1, 20);
            onParameterChanged(nColors, temp, myMap ,lifeGame::compareColorNumberVectors, lifeGame::newColorsRandomPosRandomVel);

        }
        if(selection == GRAVITATOR){
            std::vector<int> temp = nColors;
            nColors[map::WHITE]  = 0;
            ImGui::SliderInt("Number of Pointz", &nColors[map::GREEN], 0, MAX_POINT);
            if(ImGui::Button("Randomize Colors")){
                nColors[map::GREEN] = rand()%MAX_POINT;
                nColors[map::WHITE]  = rand()%MAX_POINT;
                nColors[map::CYAN] = rand()%MAX_POINT;
                nColors[map::BLUE] = rand()%MAX_POINT;
                nColors[map::MAGENTA] = rand()%MAX_POINT;
            }
            onParameterChanged(nColors, temp, myMap ,gravitator::compareColorNumberVectors, gravitator::newColorsRandomPosRandomVel);
        }
    }


    void step(){
        if (selection == BALLPIT) {
            myMap->update(ballpit::calculate);
        } else if (selection == LIFE_GAME){
            myMap->update(lifeGame::calculate);

        }if(selection == GRAVITATOR){
            myMap->update(gravitator::calculate);
        }
    };

    template<class T>
    void onParameterChanged( T & newParam, T & oldParam, map *someMap, std::function<bool(T &, T & )> compareFn, std::function<std::vector<Point>(map *, T &)> fn){
        if (compareFn(newParam, oldParam)){
            myMap->init(fn(someMap, newParam));
        }
        oldParam = newParam;
    }

    int getSelection(){
        return selection;
    }
};


#endif //PROGRAMNAMEEXAMPLE_GAMESELECTOR_H
