
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include "Snake.cpp"
#include "Food.cpp"
using namespace cv;
using namespace std;

cv::Mat getChessMap() {
    cv::Mat chessMap(HEIGHT, WIDTH, CV_8UC3, cv::Scalar(10,220,10));

    for(int i=0;i<WIDTH;i++) {
        for(int j=0;j<HEIGHT;j++) {
            //cv::rectangle(chessMap,cv::Point(j*GRID_SIZE*2,i*GRID_SIZE*2), cv::Point(GRID_SIZE*(2*j+1),GRID_SIZE*(2*i+1)), cv::Scalar(255,255,255),cv::FILLED);
            if((i+j)%2 == 0) {
                cv::rectangle(chessMap,cv::Point(i*GRID_SIZE,j*GRID_SIZE), cv::Point((i+1)*GRID_SIZE, (j+1)*GRID_SIZE), cv::Scalar(40,200,60),-1);
            }
        }

    }
    return chessMap;
}

int main() {
    // initialize the game
    Snake snake(WIDTH / 2, HEIGHT / 2);
    Food food;
    bool game_over = false;
    int score = 0;
    Mat menuImg(HEIGHT, WIDTH, CV_8UC3, Scalar(0));
    menuImg = getChessMap();
    cv::putText(menuImg,"Press Y - start game", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2);
    cv::putText(menuImg,"Press Q - quit the game", Point(50, 100), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2);
    imshow("Menu Snake",menuImg);
    char key = waitKey(0);
    switch (key) {
        case 'y':
// create the window
            destroyWindow("Menu Snake");
            namedWindow("Snake Game", WINDOW_NORMAL);
            resizeWindow("Snake Game", WIDTH, HEIGHT);

            // run the game loop
            while (!game_over) {
                // get user input
                int key = waitKey(100);
                if (key == 'w') {
                    snake.set_direction(3);
                }
                else if (key == 'a') {
                    snake.set_direction(2);
                } else if (key == 's') {
                    snake.set_direction(1);
                } else if (key == 'd') {
                    snake.set_direction(0);
                }

                // move the snake
                snake.move();

                // check collision with food
                if (snake.check_collision()) {
                    game_over = true;
                    Mat scoreImage(HEIGHT, WIDTH, CV_8UC3, Scalar(0));
                    scoreImage = getChessMap();
                    cv::putText(scoreImage,"GAME OVER!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2);
                    string s = to_string(score);
                    cv::putText(scoreImage,"Score: " + s, Point(50, 100), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2);
                    destroyWindow("Snake Game");
                    imshow("Game Over", scoreImage);
                    waitKey(0);
                    break;
                } else if (snake.body[0] == food.get_position()) {
                    food = Food();
                    score++;
                    snake.grow();
                }

                // draw the game
                Mat image = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
                image = getChessMap();
                snake.draw(image);
                food.draw(image);
                imshow("Snake Game", image);
            }

            waitKey(0);
            break;

        case 'q':
            cout<<"Game quitting"<<endl;
            break;
    }

    return 0;
}
