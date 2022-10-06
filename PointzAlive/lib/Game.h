#ifndef PROGRAMNAMEEXAMPLE_GAMESELECTOR_H
#define PROGRAMNAMEEXAMPLE_GAMESELECTOR_H

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h
#include "imgui_internal.h"
#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <utility>
#include "Point.h"
#include "Space.h"
#include "games/nogame.h"
#include "games/gravitator2.h"
#include "games/life_game2.h"
#include "games/BallPit.h"

#define MAX_POINT 2500

class Game {

private:
    int selection;
    Space *myMap;
    vector<int> nColors;
    std::function<bool(int &, int &)> hasSelectionChanged;

public:
    Game(Space *map): selection(NOGAME), myMap(map), hasSelectionChanged([](int &a, int &b){return a != b;}){

        nColors.resize(Space::COLOR_MAX);

    }

    void ShowGameSettings(){
        ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar);
        ImGui::Separator();
        ImGui::Text("PointzAlive 0.0.3 - Nothing here is truly stable");
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

            ImGui::SliderInt("Green Pointz", &nColors[Space::THIRD], 0, MAX_POINT);
            ImGui::SliderInt("White Balls", &nColors[Space::FIFTH], 0, MAX_POINT);
            ImGui::SliderInt("Cyan Pointz", &nColors[Space::SECOND], 0, MAX_POINT);
            ImGui::SliderInt("Blue Pointz", &nColors[Space::FIRST], 0, MAX_POINT);
            ImGui::SliderInt("Magenta Pointz", &nColors[Space::FORTH], 0, MAX_POINT);
            if(ImGui::Button("Randomize Colors")){
                nColors[Space::THIRD] = rand() % MAX_POINT;
                nColors[Space::FIFTH]  = rand() % MAX_POINT;
                nColors[Space::SECOND] = rand() % MAX_POINT;
                nColors[Space::FIRST] = rand() % MAX_POINT;
                nColors[Space::FORTH] = rand() % MAX_POINT;
            };
            onParameterChanged(nColors, temp, myMap ,ballpit::compareColorNumberVectors, ballpit::newColorsRandomPosRandomVel);
        }
        if(selection == LIFE_GAME){
            std::vector<int> temp = nColors;

            ImGui::Separator();
            ImGui::SliderInt("Number of different Point type", &lifeGame::numberOfTypes, 1, 6);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImGui::Separator();

            lifeGame::graphics::showRule(lifeGame::numberOfTypes, draw_list, 400, 400);

            ImGui::Separator();


            static float sz = 36.0f;
            float x = 1, y=1;

            for (int type=0; type < Space::colors::COLOR_MAX; type++){

                if(lifeGame::numberOfTypes>type) {
                    std::string buf;
                    ImGui::Separator();
                    ImGui::Text(std::string("Pointz" +to_string(type)).c_str(),type);
                    ImGui::SliderInt(std::string("Amount " +to_string(type)).c_str(), &nColors[type], 0, MAX_POINT);

                    //-----COLOR SETTINGS -----//
                    if(ImGui::ColorEdit3(std::string("Color " +to_string(type)).c_str(), lifeGame::rgbwMap[type],1)){
                        for (auto &point: myMap->points) {
                            if(point.getColor().first==type){
                                point.setRGBColor(ImVec4(lifeGame::rgbwMap[type][0],lifeGame::rgbwMap[type][1],lifeGame::rgbwMap[type][2],lifeGame::rgbwMap[type][3]));
                            }
                        }
                    };

                    for(int j=0;j<lifeGame::numberOfTypes;j++){
                        ImColor col = lifeGame::ruleToColor(lifeGame::rules[type][j]);
                        ImGui::SliderFloat(string(to_string(type) + " -> " +to_string(j)).c_str(), &lifeGame::rules[type][j],lifeGame::rule_MaxAttraction, lifeGame::rule_MaxRepulsion, "%.3f");
                        ImGui::SameLine();

                        draw_list->AddCircleFilled(ImVec2(ImGui::GetCursorScreenPos().x+5, ImGui::GetCursorScreenPos().y+10), 7, ImColor(lifeGame::rgbwMap[type][0],lifeGame::rgbwMap[type][1],lifeGame::rgbwMap[type][2],lifeGame::rgbwMap[type][3]), 20);

                        draw_list->AddLine(ImVec2(ImGui::GetCursorScreenPos().x+15, ImGui::GetCursorScreenPos().y+10), ImVec2(ImGui::GetCursorScreenPos().x+80, ImGui::GetCursorScreenPos().y+10), col,5 );
                        draw_list->AddTriangleFilled(ImVec2(ImGui::GetCursorScreenPos().x+80, ImGui::GetCursorScreenPos().y+15),ImVec2(ImGui::GetCursorScreenPos().x+80, ImGui::GetCursorScreenPos().y+5),ImVec2(ImGui::GetCursorScreenPos().x+90, ImGui::GetCursorScreenPos().y+10),col);
                        draw_list->AddCircleFilled(ImVec2(ImGui::GetCursorScreenPos().x+100, ImGui::GetCursorScreenPos().y+10), 7, ImColor(lifeGame::rgbwMap[j][0],lifeGame::rgbwMap[j][1],lifeGame::rgbwMap[j][2],lifeGame::rgbwMap[j][3]), 20);
                        ImGui::NewLine();
                    }


                } else {
                   nColors[type] = 0;
                }
            }


            if(ImGui::Button("Randomize forces")){
                for(int i=0;i<lifeGame::numberOfTypes;i++){
                    for(int j=0;j<lifeGame::numberOfTypes;j++){
                        lifeGame::rules[i][j] = rand()%11-5+rand()%9*0.1 +rand()%9*0.01+rand()%9*0.001;
                    }
                }
            };

            if(ImGui::Button(lifeGame::gravitatorOn(lifeGame::isGravitator))){
                lifeGame::isGravitator = !lifeGame::isGravitator;
            }

            ImGui::SliderInt("Number of threads", &myMap->nthreads, 1, 20);
            onParameterChanged(nColors, temp, myMap ,lifeGame::compareColorNumberVectors, lifeGame::newColorsRandomPosRandomVel);

        }
        if(selection == GRAVITATOR){
            std::vector<int> temp = nColors;
            nColors[Space::FIFTH]  = 0;
            ImGui::SliderInt("Number of Pointz", &nColors[Space::THIRD], 0, MAX_POINT);
            if(ImGui::Button("Randomize Colors")){
                nColors[Space::THIRD] = rand() % MAX_POINT;
                nColors[Space::FIFTH]  = rand() % MAX_POINT;
                nColors[Space::SECOND] = rand() % MAX_POINT;
                nColors[Space::FIRST] = rand() % MAX_POINT;
                nColors[Space::FORTH] = rand() % MAX_POINT;
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
    void onParameterChanged(T & newParam, T & oldParam, Space *someMap, std::function<bool(T &, T & )> compareFn, std::function<std::vector<Point>(Space *, T &)> fn){
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
