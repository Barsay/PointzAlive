#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "lib/Point.h"
#include "lib/Space.h"
#include "lib/pointzalive_core.h"

int main(){
    //create window
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "PointzAlive");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    Space demoMap(&window, true);
    pointzalive_core myGame(&demoMap);

    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear();
        myGame.ShowGameSettings();
        myGame.step();
        demoMap.show();

        //------------------//

        ImGui::SFML::Render(window);
        window.display();

    }


}
