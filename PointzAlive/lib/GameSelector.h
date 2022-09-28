#ifndef PROGRAMNAMEEXAMPLE_GAMESELECTOR_H
#define PROGRAMNAMEEXAMPLE_GAMESELECTOR_H

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "Point.h"
#include "map.h"

#define MAX_POINT 2500

class GameSelector {

private:
    int selection;
    map *myMap;

public:
    GameSelector(map *map):selection(NOGAME), myMap(map){}

    void ShowGameSettings(){
        ImGui::Begin("Game", nullptr, ImGuiWindowFlags_MenuBar);
        ImGui::Separator();
        ImGui::Text("BallzAlive 0.0.1 - Nothing of this is truly stable");
        ImGui::Separator();
        showSelectionMenu();
        showGameMenu();
        ImGui::End();
    }

    void showSelectionMenu(){

        if(ImGui::BeginMenuBar() ) {
            if (ImGui::BeginMenu("Select Game")) {
                if (ImGui::MenuItem("NoGame")) {
                    selection = NOGAME;
                } else if (ImGui::MenuItem("BallPit")) {
                    selection = BALLPIT;
                }else if (ImGui::MenuItem("Life Game")) {
                    selection = LIFE_GAME;
                }else if (ImGui::MenuItem("Gravitator")) {
                    selection = GRAVITATOR;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void showGameMenu(){
        if(selection == NOGAME){
            myMap->numberOfcolors[map::GREEN]=0;
            myMap->numberOfcolors[map::WHITE]=0;
            myMap->numberOfcolors[map::CYAN]=0;
            myMap->numberOfcolors[map::BLUE]=0;
            myMap->numberOfcolors[map::MAGENTA]=0;
        }
        if (selection == BALLPIT){
            ImGui::SliderInt("Green Balls", &myMap->numberOfcolors[map::GREEN], 0, MAX_POINT);
            ImGui::SliderInt("White Balls", &myMap->numberOfcolors[map::WHITE], 0, MAX_POINT);
            ImGui::SliderInt("Cyan Balls", &myMap->numberOfcolors[map::CYAN], 0, MAX_POINT);
            ImGui::SliderInt("Blue Balls", &myMap->numberOfcolors[map::BLUE], 0, MAX_POINT);
            ImGui::SliderInt("Magenta Balls", &myMap->numberOfcolors[map::MAGENTA], 0, MAX_POINT);
            if(ImGui::Button("Randomize Colors")){
                myMap->numberOfcolors[map::GREEN] = rand()%MAX_POINT;
                myMap->numberOfcolors[map::WHITE]  = rand()%MAX_POINT;
                myMap->numberOfcolors[map::CYAN] = rand()%MAX_POINT;
                myMap->numberOfcolors[map::BLUE] = rand()%MAX_POINT;
                myMap->numberOfcolors[map::MAGENTA] = rand()%MAX_POINT;
            };
        }
        if(selection == LIFE_GAME){
            myMap->numberOfcolors[map::WHITE]  = 0;
            ImGui::SliderInt("Green Balls", &myMap->numberOfcolors[map::GREEN], 0, MAX_POINT);
            ImGui::SliderInt("Cyan Balls", &myMap->numberOfcolors[map::CYAN], 0, MAX_POINT);
            ImGui::SliderInt("Blue Balls", &myMap->numberOfcolors[map::BLUE], 0, MAX_POINT);
            ImGui::SliderInt("Magenta Balls", &myMap->numberOfcolors[map::MAGENTA], 0, MAX_POINT);
            ImGui::Separator();
            ImGui::SliderInt("Number of threads", &myMap->nthreads, 1, 20);
        }
        if(selection == GRAVITATOR){
            myMap->numberOfcolors[map::WHITE]  = 0;
            ImGui::SliderInt("Number of balls", &myMap->numberOfcolors[map::GREEN], 0, MAX_POINT);
            if(ImGui::Button("Randomize Colors")){
                myMap->numberOfcolors[map::GREEN] = rand()%MAX_POINT;
                myMap->numberOfcolors[map::WHITE]  = rand()%MAX_POINT;
                myMap->numberOfcolors[map::CYAN] = rand()%MAX_POINT;
                myMap->numberOfcolors[map::BLUE] = rand()%MAX_POINT;
                myMap->numberOfcolors[map::MAGENTA] = rand()%MAX_POINT;
            };
        }
    }

    int getSelection(){
        return selection;
    }

};


#endif //PROGRAMNAMEEXAMPLE_GAMESELECTOR_H
