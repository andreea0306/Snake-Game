////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: main.cpp
// description: implements the game logic
///////////////////////////////////////////////////////////////////////////

#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "Defines.h"
#include "Snake.cpp"
#include "Food.cpp"
#include "Gfx.cpp"

int main() {
    // initialize the game variables
    Snake snake(WIDTH / 2, HEIGHT / 2);
    Food food;
    int score = 0;
    Gfx gfx = Gfx(WIDTH, HEIGHT, GRID_SIZE);
    std::string menu_file_path = "/home/andreea/CLionProjects/newSnakeTry/snake_start_menu.png";
    // define the weight of each image in the blend
    double alpha = 0.5;
    double beta = 0.1;
    // add an image to the menu and resize it to fit with the initial image
    cv::Mat snakeImg = cv::imread(menu_file_path);
    cv::Mat resizedSnakeImg;
    cv::resize(snakeImg, resizedSnakeImg,cv::Size(WIDTH, HEIGHT), cv::INTER_LINEAR);
    // initialize a black image
    cv::Mat menuImg(HEIGHT, WIDTH, CV_8UC3, cv::Scalar(0));
    menuImg = gfx.getGreenChessMap();
    // design the menu window
    menuImg = gfx.blendImages(menuImg, resizedSnakeImg, alpha, beta);
    // add text to the start menu
    cv::putText(menuImg,"START-PRESS SPACEBAR", cv::Point(270, 100), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 0), 1);
    cv::putText(menuImg,"QUIT- PRESS ANY KEY", cv::Point(270, 150), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 0), 1);
    // display window
    cv::imshow("Menu Snake",menuImg);
    // holds the value from user input
    char key = (char)cv::waitKey(0);
    switch (key) {
        // if space bar is pressed the game starts
        case ' ':
            // delete the menu window & create the game window
            cv::destroyWindow("Menu Snake");
            namedWindow("Snake Game", cv::WINDOW_NORMAL);
            // create a fixed size window
            cv::resizeWindow("Snake Game", WIDTH, HEIGHT);
            // run the game loop
            while (true) {
                // get user input - snake control
                key = (char)cv::waitKey(100);
                if (std::tolower(key) == 'w') { // going up
                    snake.set_direction(3);
                } else if (std::tolower(key) == 'a') { // going left
                    snake.set_direction(2);
                } else if (std::tolower(key) == 's') { // going down
                    snake.set_direction(1);
                } else if (std::tolower(key) == 'd') { // going right
                    snake.set_direction(0);
                }
                // move the snake
                snake.move();
                // check collision with food and body
                if (snake.check_collision()) {
                    // create the Game Over window
                    cv::Mat scoreImage = gfx.getGreenChessMap();
                    scoreImage = gfx.blendImages(scoreImage, resizedSnakeImg, alpha, beta);
                    // display final score
                    cv::putText(scoreImage, "GAME OVER!", cv::Point(270, 50), cv::FONT_HERSHEY_DUPLEX, 1.0,
                                cv::Scalar(0, 0, 0), 2);
                    std::string s = std::to_string(score);
                    cv::putText(scoreImage, "Score: " + s, cv::Point(270, 100), cv::FONT_HERSHEY_DUPLEX, 1.0,
                                cv::Scalar(0, 0, 0), 2);
                    cv::destroyWindow("Snake Game");
                    imshow("Game Over", scoreImage);
                    cv::waitKey(0);
                    break;
                } else if (snake.body[0] == food.get_position()) {
                    // if snake ate the food, a new apple is generated, score increases and snake grows
                    food = Food();
                    score++;
                    snake.grow();
                }
                // draw the game window
                cv::Mat gameImage = gfx.getGreenChessMap();
                // draw snake on game window
                snake.draw(gameImage);
                // draw food on game window
                food.draw(gameImage);
                imshow("Snake Game", gameImage);
            }
            break;
        // if any other kay was pressed, the game stops
        default:
            std::cout<<"Game quitting"<<std::endl;
            break;
    }
    return 0;
}
