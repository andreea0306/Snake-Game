////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: main.cpp
// description: implements the game logic
///////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// Linux headers
// Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include<opencv2/opencv.hpp>
#include<opencv2/core/mat.hpp>
#include <thread>


#include "Defines.h"
#include "Snake.cpp"
#include "Food.cpp"
#include "Gfx.cpp"


int initUart(const char* port) {
    int serialPort = open(port, O_RDWR /*| O_NOCTTY | O_NONBLOCK*/);
    //  debug
    if (serialPort == -1) {
        printf("Error %i : %s\n", errno, strerror(errno));
        return 1;
    }
    //  std::cout<<serialPort<<std::endl;
    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if (tcgetattr(serialPort, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }


    // Set serial port settings (baud rate, data bits, etc.)
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                     ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in baud rate to be 115200
    cfsetispeed(&tty, B115200);

    return serialPort;
}

void displayGame(Snake snake, Food food, Gfx gfx) {
    // draw the game window
    namedWindow("Game", cv::WINDOW_NORMAL);
    // create a fixed size window
    cv::resizeWindow( "Game", gfx.getWidth(), gfx.getHeight());
    cv::Mat gameImage = gfx.getGreenChessMap();
    // draw snake on game window
    snake.draw(gameImage);
    // draw food on game window
    food.draw(gameImage);
    imshow("Game", gameImage);
    cv::waitKey(1);
    unsigned int microsecond = 100000;
    usleep(3  * microsecond);//sleeps for 3 second
}

int main(int argc, char* argv[]) {
    printf("Program name is: %s", argv[0]);
    if(argc == 1) {
        printf("\nNo extra command line argument passed so the game will start using keyboard");
        // initialize the game variables

        Gfx gfx = Gfx(WIDTH, HEIGHT, GRID_SIZE);
        Snake snake(gfx.getWidth() / 2, gfx.getHeight() / 2, gfx);
        Food food(gfx, snake);
        int score = 0;

        std::string menu_file_path = "/home/andreea/CLionProjects/newSnakeTry/snake_start_menu.png";
        // define the weight of each image in the blend
        double alpha = 0.5;
        double beta = 0.1;
        // add an image to the menu and resize it to fit with the initial image
        cv::Mat snakeImg = cv::imread(menu_file_path);
        cv::Mat resizedSnakeImg;
        cv::resize(snakeImg, resizedSnakeImg,cv::Size(gfx.getWidth(), gfx.getHeight()), cv::INTER_LINEAR);
        // initialize a black image
        cv::Mat menuImg(gfx.getHeight(), gfx.getWidth(), CV_8UC3, cv::Scalar(0));
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
                cv::resizeWindow("Snake Game", gfx.getWidth(), gfx.getHeight());
                // run the game loop
                while (true) {
                    // get user input - snake control
                    key = (char)cv::waitKey(100);
                    if (std::tolower(key) == 'w' && snake.getDirection() != 1) { // going up
                        snake.set_direction(3);
                    } else if (std::tolower(key) == 'a' && snake.getDirection() != 0) { // going left
                        snake.set_direction(2);
                    } else if (std::tolower(key) == 's' && snake.getDirection() != 3) { // going down
                        snake.set_direction(1);
                    } else if (std::tolower(key) == 'd' && snake.getDirection() != 2) { // going right
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
                        food = Food(gfx, snake);
                        score++;
                        //food.setGfx(gfx);
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
    }
    if(argc == 2 && strcmp(argv[1] ,"M5StickCPlus") == 0) {
        printf("the code for snake using m5stick c plus\n");
        printf("the second argument is: %s\n", argv[1]);
        // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)

        int serialPort = initUart("/dev/ttyUSB1");

        Gfx gfx = Gfx(WIDTH, HEIGHT, GRID_SIZE);

        // initialize the game variables
        Snake snake(WIDTH/2, HEIGHT/2, gfx);
        Food food(gfx, snake);
        int score = 0;
        // add an image to the menu and resize it to fit with the initial image
        // run the game loop
        while (true) {
            std::thread displayThread(displayGame, snake, food, gfx);
            displayThread.join();
            //  get user input from M5StickC Plus
            char buffer;
            size_t bytesRead = read(serialPort, &buffer, sizeof(buffer));
            std::cout << "buffer: " << buffer << " and bytes read:" << bytesRead << std::endl;
            int move = buffer;

            if (move == '3' && snake.getDirection() != 1) {
                snake.set_direction(3);
                std::cout << "direction 3 set" << std::endl;
            } else if (move == '2' && snake.getDirection() != 0) {
                snake.set_direction(2);
                std::cout << "direction 2 set" << std::endl;
            } else if (move == '1' && snake.getDirection() != 3) {
                snake.set_direction(1);
                std::cout << "direction 1 set" << std::endl;
            } else if (move == '0' && snake.getDirection() != 2) {
                snake.set_direction(0);
                std::cout << "direction 0 set" << std::endl;
            }
            snake.move();

            // check collision with food and body
            if (snake.check_collision()) {
                std::cout << "final score: " << score << std::endl;
                cv::destroyWindow("Game");
                break;
            } else if (snake.body[0] == food.get_position()) {
                // if snake ate the food, a new apple is generated, score increases and snake grows
                food = Food(gfx, snake);
                score++;
                snake.grow();
            }

        }
        close(serialPort);

    }
    return 0;
}
