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
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <map>


namespace lifeGame {

    static int numberOfTypes = 4;
    static float rule_MaxAttraction = -5;
    static float rule_MaxRepulsion = +5;
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
                                    std::pair<int, sf::Color>(i,sf::Color((int)(255*lifeGame::rgbwMap[i][0]),(int)(255*lifeGame::rgbwMap[i][1]),(int)(255*lifeGame::rgbwMap[i][2]),(int)(255*lifeGame::rgbwMap[i][3]))));
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


    bool updateVelocity(Point &P, std::vector<Point> &pvector, double vx, double vy){
        double vvx = vx, vvy = vy;
        double distancex, distancey, d, F=0, fx=0, fy=0;
        for (auto &p: pvector) {

            //distance on x
            distancex = P.getPosition()[0] - p.getPosition()[0];
            //distance on y
            distancey = P.getPosition()[1] - p.getPosition()[1];
            d = sqrt((distancex*distancex)+(distancey*distancey));

            if(d!=0  && d<150 &&d>2) {

                F = lifeGame::rules[P.getColor().first][p.getColor().first]* (3) / ((d * d));
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

    //Green: Attraction (negative)
    //Red: Repulsion (Positive)
    ImColor ruleToColor(float rule){
        int red, green, blue=0, alpha=255;
        float tot = ((rule_MaxRepulsion>0? rule_MaxRepulsion : -rule_MaxRepulsion) + (rule_MaxAttraction >0?rule_MaxAttraction : -rule_MaxAttraction ) );
        red = (rule*rule_MaxRepulsion  > 0 ? (rule)*255/(rule_MaxRepulsion) : 0);
        green = (rule*rule_MaxAttraction  > 0 ? rule*255/(rule_MaxAttraction): 0);
        alpha = rule>0 ? 255/(rule) : -255/(rule);
        blue = rule != 0 ? (rule>0 ? 1/(rule*30) : -1/(rule*30) ) : 0 ;
        return ImColor(red, green, blue, alpha);
    }

    //namespace for drawing applications
    namespace graphics {

        void arrow(std::pair<int, int> start, std::pair<int, int> end, ImColor col, ImDrawList *draw_list) {

            draw_list->AddLine(ImVec2(start.first, start.second), ImVec2(end.first, end.second), col, 5);
            draw_list->AddTriangleFilled(ImVec2(end.first, end.second + 10), ImVec2(end.first, end.second - 10),
                                         ImVec2(end.first + 10, end.second), col);

        }
        typedef struct circle{
            ImVec2 center;
            int radius;
        }circle;
        /**
         * @brief creates an arrow between two circles
         * @param draw_list
         * @param startCircle
         * @param endCircle
         * @param offsetX number of pixels between the beginning-end of the circle and the arrow
         * @param col
         * @param offsetY number of pixels for y offset
         *
         */
        void arrow(ImDrawList *draw_list, circle startCircle, circle endCircle, ImColor col, float offsetAngle, float offsetDistance) {
            float dx= endCircle.center.x - startCircle.center.x , dy= endCircle.center.y - startCircle.center.y ;
            //Arrow lenght is 1/10 of the line
            float Arrowlength = (1/10)*sqrt(dx*dx + dy*dy);

            //in radians, counterclockwise, starting from horizontal with direction right (this: -> )
            float angle = atan2(dy,dx);
            angle = (angle < 0 ?  2*3.14+angle : angle);
            float totAngle = angle-offsetAngle;
            cout << "Angle is "<< angle * 180/M_PI <<endl;
            cout << "totangle is "<< totAngle* 180/M_PI <<endl;


            ImVec2 start(startCircle.center.x+(startCircle.radius+offsetDistance)*cos(totAngle), startCircle.center.y+(startCircle.radius+offsetDistance)*sin(totAngle));
            //ImVec2 end(endCircle.center.x-(endCircle.radius+offsetDistance)*sin(totAngle), endCircle.center.y+(endCircle.radius+offsetDistance)*cos(totAngle));
            ImVec2 end(endCircle.center.x+(endCircle.radius+offsetDistance)*cos(M_PI+offsetAngle+angle), endCircle.center.y+(endCircle.radius+offsetDistance)*sin(M_PI+offsetAngle+angle));
            draw_list->AddLine(start, end, col, startCircle.radius/4);
            /*
            draw_list->AddTriangleFilled(ImVec2(end.first, end.second + 10), ImVec2(end.first, end.second - 10),
                                         ImVec2(end.first + 10, end.second), col);

            */
        }


        /**
         * @brief Draw a point with a circular arrow pointing to itself
         * @param draw_list drawlist pointer
         * @param circleCenter Coordinates for Point
         * @param CircleRadius Point radius
         * @param CircleColor Point Color
         * @param angle radians, starting from up (12:00), and going clockwise
         * @param ForceColor Color for force arrow
         */
        void pointWithForce(ImDrawList *draw_list, ImVec2 circleCenter, float CircleRadius, ImColor CircleColor, float angle, ImColor ForceColor){
            draw_list->AddCircle(ImVec2 (circleCenter[0]+CircleRadius*sin(angle), circleCenter[1]-CircleRadius*cos(angle)) , CircleRadius*80/100,ForceColor,20, CircleRadius/4);
            draw_list->AddCircleFilled(circleCenter, 20,CircleColor,20);
        }

        void showRule(int n, ImDrawList *draw_list, int width, int height) {
            //ImGui::SameLine();
            //ImGui::SetCursorScreenPos(ImVec2 {ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y-height });

            //X0, Y0 of the rectangle.
            ImVec2 StartingPoint(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
            //X1, Y1 of the rectangle
            ImVec2 EndPoint(StartingPoint[0]+width, StartingPoint[1]+height);
            //Coordinates of the center

            draw_list->AddRect(StartingPoint,
                               EndPoint,
                               ImColor(0, 0, 255));
            if (n == 1) {
                ImVec2 center((StartingPoint[0]+EndPoint[0])/2, (StartingPoint[1]+EndPoint[1])/2);
                lifeGame::graphics::pointWithForce(draw_list,center,20,ImColor(lifeGame::rgbwMap[0][0], lifeGame::rgbwMap[0][1],lifeGame::rgbwMap[0][2], lifeGame::rgbwMap[0][3]),3.14*7/4, lifeGame::ruleToColor(lifeGame::rules[0][0]));
            }
            if (n==2){
                circle c1, c2;
                ImVec2 center((StartingPoint[0]+EndPoint[0])/2, (StartingPoint[1]+EndPoint[1])/2);

                c1.center = ImVec2(StartingPoint[0]+width*1/4 ,center.y);
                c2.center = ImVec2(StartingPoint[0]+width*3/4 , center.y);

                c1.radius = 20;
                c2.radius = 20;
                //pointz
                lifeGame::graphics::pointWithForce(draw_list,c1.center,c1.radius,ImColor(lifeGame::rgbwMap[0][0], lifeGame::rgbwMap[0][1],lifeGame::rgbwMap[0][2], lifeGame::rgbwMap[0][3]),3.14*3/2, lifeGame::ruleToColor(lifeGame::rules[0][0]));
                lifeGame::graphics::pointWithForce(draw_list,c2.center,c2.radius,ImColor(lifeGame::rgbwMap[1][0], lifeGame::rgbwMap[1][1],lifeGame::rgbwMap[1][2], lifeGame::rgbwMap[1][3]),3.14*1/2, lifeGame::ruleToColor(lifeGame::rules[1][1]));
                //Arrowz
                //0->1
                arrow(draw_list,c1,c2, lifeGame::ruleToColor(lifeGame::rules[0][1]),M_PI*1/12,+5);
                //1->0
                arrow(draw_list,c2,c1, lifeGame::ruleToColor(lifeGame::rules[1][0]),M_PI*1/12 ,+5);

            }
            if (n==3){
                circle c0, c1, c2;

                c0.center = ImVec2(StartingPoint[0]+width*1/3,StartingPoint[1]+height*1/3 );
                c1.center = ImVec2(StartingPoint[0]+width*2/3,StartingPoint[1]+height*1/3 );
                c2.center = ImVec2(StartingPoint[0]+width*1/2,StartingPoint[1]+height*2/3 );

                c0.radius = 20;
                c1.radius = 20;
                c2.radius = 20;

                //Pointz
                lifeGame::graphics::pointWithForce(draw_list,c0.center,c0.radius,ImColor(lifeGame::rgbwMap[0][0], lifeGame::rgbwMap[0][1],lifeGame::rgbwMap[0][2], lifeGame::rgbwMap[0][3]),2*3.14*3/4, lifeGame::ruleToColor(lifeGame::rules[0][0]));
                lifeGame::graphics::pointWithForce(draw_list,c1.center,c1.radius,ImColor(lifeGame::rgbwMap[1][0], lifeGame::rgbwMap[1][1],lifeGame::rgbwMap[1][2], lifeGame::rgbwMap[1][3]),2*3.14*1/4, lifeGame::ruleToColor(lifeGame::rules[1][1]));
                lifeGame::graphics::pointWithForce(draw_list,c2.center,c2.radius,ImColor(lifeGame::rgbwMap[2][0], lifeGame::rgbwMap[2][1],lifeGame::rgbwMap[2][2], lifeGame::rgbwMap[2][3]),2*3.14*1/2, lifeGame::ruleToColor(lifeGame::rules[2][2]));


                //Arrowz
                //0->1
                arrow(draw_list,c0,c1, lifeGame::ruleToColor(lifeGame::rules[0][1]), M_PI*1/12,+5);
                //0->2
                arrow(draw_list,c0,c2, lifeGame::ruleToColor(lifeGame::rules[0][2]),M_PI*1/12,5);
                //1->0
                arrow(draw_list,c1,c0, lifeGame::ruleToColor(lifeGame::rules[1][0]),M_PI*1/12, 5);
                //1->2
                arrow(draw_list,c1,c2, lifeGame::ruleToColor(lifeGame::rules[1][2]),M_PI*1/12, 5);
                //2->0
                arrow(draw_list,c2,c0, lifeGame::ruleToColor(lifeGame::rules[2][0]),M_PI*1/12, 5);
                //2->1
                arrow(draw_list,c2,c1, lifeGame::ruleToColor(lifeGame::rules[2][1]),M_PI*1/12, 5);
            }
            if(n==4){
                circle c0, c1, c2, c3;
                c0.center = ImVec2(StartingPoint[0]+width*1/4,StartingPoint[1]+height*1/4 );
                c1.center = ImVec2(StartingPoint[0]+width*3/4,StartingPoint[1]+height*1/4 );
                c2.center = ImVec2(StartingPoint[0]+width*1/4,StartingPoint[1]+height*3/4 );
                c3.center = ImVec2(StartingPoint[0]+width*3/4,StartingPoint[1]+height*3/4 );

                c0.radius = 20;
                c1.radius = 20;
                c2.radius = 20;
                c3.radius = 20;

                //Pointz
                lifeGame::graphics::pointWithForce(draw_list,c0.center,c0.radius,ImColor(lifeGame::rgbwMap[0][0], lifeGame::rgbwMap[0][1],lifeGame::rgbwMap[0][2], lifeGame::rgbwMap[0][3]),-M_PI*1/4, lifeGame::ruleToColor(lifeGame::rules[0][0]));
                lifeGame::graphics::pointWithForce(draw_list,c1.center,c1.radius,ImColor(lifeGame::rgbwMap[1][0], lifeGame::rgbwMap[1][1],lifeGame::rgbwMap[1][2], lifeGame::rgbwMap[1][3]),M_PI*1/4, lifeGame::ruleToColor(lifeGame::rules[1][1]));
                lifeGame::graphics::pointWithForce(draw_list,c2.center,c2.radius,ImColor(lifeGame::rgbwMap[2][0], lifeGame::rgbwMap[2][1],lifeGame::rgbwMap[2][2], lifeGame::rgbwMap[2][3]),M_PI*5/4, lifeGame::ruleToColor(lifeGame::rules[2][2]));
                lifeGame::graphics::pointWithForce(draw_list,c3.center,c3.radius,ImColor(lifeGame::rgbwMap[3][0], lifeGame::rgbwMap[3][1],lifeGame::rgbwMap[3][2], lifeGame::rgbwMap[3][3]),M_PI*3/4, lifeGame::ruleToColor(lifeGame::rules[3][3]));


                //Arrowz

                //0->1
                arrow(draw_list,c0,c1, lifeGame::ruleToColor(lifeGame::rules[0][1]), M_PI*1/12,+5);
                //0->2
                arrow(draw_list,c0,c2, lifeGame::ruleToColor(lifeGame::rules[0][2]),M_PI*1/12,5);
                //0->3
                arrow(draw_list,c0,c3, lifeGame::ruleToColor(lifeGame::rules[0][3]),M_PI*1/12,5);

                //1->0
                arrow(draw_list,c1,c0, lifeGame::ruleToColor(lifeGame::rules[1][0]),M_PI*1/12, 5);
                //1->2
                arrow(draw_list,c1,c2, lifeGame::ruleToColor(lifeGame::rules[1][2]),M_PI*1/12, 5);
                //1->3
                arrow(draw_list,c1,c3, lifeGame::ruleToColor(lifeGame::rules[1][3]),M_PI*1/12,5);

                //2->0
                arrow(draw_list,c2,c0, lifeGame::ruleToColor(lifeGame::rules[2][0]),M_PI*1/12, 5);
                //2->1
                arrow(draw_list,c2,c1, lifeGame::ruleToColor(lifeGame::rules[2][1]),M_PI*1/12, 5);
                //2->3
                arrow(draw_list,c2,c3, lifeGame::ruleToColor(lifeGame::rules[2][3]),M_PI*1/12,5);
                //3->0
                arrow(draw_list,c3,c0, lifeGame::ruleToColor(lifeGame::rules[3][0]),M_PI*1/12,5);
                //3->1
                arrow(draw_list,c3,c1, lifeGame::ruleToColor(lifeGame::rules[3][1]),M_PI*1/12,5);
                //3->2
                arrow(draw_list,c3,c2, lifeGame::ruleToColor(lifeGame::rules[3][2]),M_PI*1/12,5);

            }
            if (n==5){

                circle c0, c1, c2, c3, c4;
                c0.center = ImVec2(StartingPoint[0]+width*1/2,StartingPoint[1]+height*1/4 );
                c1.center = ImVec2(StartingPoint[0]+width*2/3,StartingPoint[1]+height*2/3 );
                c2.center = ImVec2(StartingPoint[0]+width*2/3,StartingPoint[1]+height*1/3);
                c3.center = ImVec2(StartingPoint[0]+width*1/3,StartingPoint[1]+height*2/3 );
                c4.center = ImVec2(StartingPoint[0]+width*1/3,StartingPoint[1]+height*1/3 );

                c0.radius = 20;
                c1.radius = 20;
                c2.radius = 20;
                c3.radius = 20;
                c4.radius = 20;

                //Pointz
                lifeGame::graphics::pointWithForce(draw_list,c0.center,c0.radius,ImColor(lifeGame::rgbwMap[0][0], lifeGame::rgbwMap[0][1],lifeGame::rgbwMap[0][2], lifeGame::rgbwMap[0][3]),2*M_PI*0/5, lifeGame::ruleToColor(lifeGame::rules[0][0]));
                lifeGame::graphics::pointWithForce(draw_list,c1.center,c1.radius,ImColor(lifeGame::rgbwMap[1][0], lifeGame::rgbwMap[1][1],lifeGame::rgbwMap[1][2], lifeGame::rgbwMap[1][3]),2*M_PI*1/5, lifeGame::ruleToColor(lifeGame::rules[1][1]));
                lifeGame::graphics::pointWithForce(draw_list,c2.center,c2.radius,ImColor(lifeGame::rgbwMap[2][0], lifeGame::rgbwMap[2][1],lifeGame::rgbwMap[2][2], lifeGame::rgbwMap[2][3]),2*M_PI*2/5, lifeGame::ruleToColor(lifeGame::rules[2][2]));
                lifeGame::graphics::pointWithForce(draw_list,c3.center,c3.radius,ImColor(lifeGame::rgbwMap[3][0], lifeGame::rgbwMap[3][1],lifeGame::rgbwMap[3][2], lifeGame::rgbwMap[3][3]),2*M_PI*3/5, lifeGame::ruleToColor(lifeGame::rules[3][3]));
                lifeGame::graphics::pointWithForce(draw_list,c4.center,c4.radius,ImColor(lifeGame::rgbwMap[4][0], lifeGame::rgbwMap[4][1],lifeGame::rgbwMap[4][2], lifeGame::rgbwMap[4][3]),2*M_PI*4/5, lifeGame::ruleToColor(lifeGame::rules[4][4]));

                //Arrowz
                //0->1
                arrow(draw_list,c0,c1, lifeGame::ruleToColor(lifeGame::rules[0][1]), M_PI*1/12,+5);
                //0->2
                arrow(draw_list,c0,c2, lifeGame::ruleToColor(lifeGame::rules[0][2]),M_PI*1/12,5);
                //0->3
                arrow(draw_list,c0,c3, lifeGame::ruleToColor(lifeGame::rules[0][3]),M_PI*1/12,5);
                //0->4
                arrow(draw_list,c0,c4, lifeGame::ruleToColor(lifeGame::rules[0][4]),M_PI*1/12,5);
                //1->0
                arrow(draw_list,c1,c0, lifeGame::ruleToColor(lifeGame::rules[1][0]),M_PI*1/12, 5);
                //1->2
                arrow(draw_list,c1,c2, lifeGame::ruleToColor(lifeGame::rules[1][2]),M_PI*1/12, 5);
                //1->3
                arrow(draw_list,c1,c3, lifeGame::ruleToColor(lifeGame::rules[1][3]),M_PI*1/12,5);
                //1->4
                arrow(draw_list,c1,c4, lifeGame::ruleToColor(lifeGame::rules[1][4]),M_PI*1/12,5);

                //2->0
                arrow(draw_list,c2,c0, lifeGame::ruleToColor(lifeGame::rules[2][0]),M_PI*1/12, 5);
                //2->1
                arrow(draw_list,c2,c1, lifeGame::ruleToColor(lifeGame::rules[2][1]),M_PI*1/12, 5);
                //2->3
                arrow(draw_list,c2,c3, lifeGame::ruleToColor(lifeGame::rules[2][3]),M_PI*1/12,5);
                //2->4
                arrow(draw_list,c2,c4, lifeGame::ruleToColor(lifeGame::rules[2][4]),M_PI*1/12,5);
                //3->0
                arrow(draw_list,c3,c0, lifeGame::ruleToColor(lifeGame::rules[3][0]),M_PI*1/12,5);
                //3->1
                arrow(draw_list,c3,c1, lifeGame::ruleToColor(lifeGame::rules[3][1]),M_PI*1/12,5);
                //3->2
                arrow(draw_list,c3,c2, lifeGame::ruleToColor(lifeGame::rules[3][2]),M_PI*1/12,5);
                //3->4
                arrow(draw_list,c3,c4, lifeGame::ruleToColor(lifeGame::rules[3][4]),M_PI*1/12,5);
                //4->0
                arrow(draw_list,c4,c0, lifeGame::ruleToColor(lifeGame::rules[4][0]),M_PI*1/12,5);
                //4>1
                arrow(draw_list,c4,c1, lifeGame::ruleToColor(lifeGame::rules[4][1]),M_PI*1/12,5);
                //4->2
                arrow(draw_list,c4,c2, lifeGame::ruleToColor(lifeGame::rules[4][2]),M_PI*1/12,5);
                //4>3
                arrow(draw_list,c3,c4, lifeGame::ruleToColor(lifeGame::rules[4][3]),M_PI*1/12,5);



            }

            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + height));
        }
    };
};


#endif //PROGRAMNAMEEXAMPLE_LIFE_GAME_H
