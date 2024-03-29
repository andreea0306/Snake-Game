////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: Food.cpp
// description: Snake's food class - generates apple position & draw it
///////////////////////////////////////////////////////////////////////////
#include "Defines.h"
#include "Gfx.cpp"
#include "Snake.cpp"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#pragma once
class Food {
private:
    cv::Point position;
    Gfx graphics;
    Snake snake;
public:
    // generate a random pair fo coordinates for apple
     Food(Gfx g, Snake snk){
        graphics = g;
        snake = snk;
        do {
            position.x = (rand() % (graphics.getWidth() / graphics.getGridSize())) * graphics.getGridSize();
            position.y = (rand() % (graphics.getHeight() / graphics.getGridSize())) * graphics.getGridSize();
        } while (isFoodOnSnake(position,snake));
    }

    Food(int x, int y, Gfx g){
        position.x = x;
        position.y = y;
        graphics = g;
    }

    void setGfx(Gfx g) {
        graphics = g;
    }
    // draw apple on map
    void draw(cv::Mat& image) const {
        rectangle(image, cv::Rect(position.x, position.y, graphics.getGridSize(), graphics.getGridSize()), cv::Scalar(0, 0, 255), -1);
    }
    // get apple's position
    cv::Point get_position() {
        return position;
    }

    bool isFoodOnSnake(const cv::Point& food, const Snake snk) {
        for(auto &it : snk.body) {
            if(it.x == food.x && it.y == food.y) return true;
        }
        return false;
    }

};