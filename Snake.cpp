////////////////////////////////////////////////////////////////////////////////
// author: Andreea Andrei
// file name: Snake.cpp
// description: Snake class - creates the snake & moves & grows & draw on map
///////////////////////////////////////////////////////////////////////////////

#include "Defines.h"
#include "Gfx.cpp"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#pragma once
class Snake {
private:
    int direction;
    Gfx graphics;
public:
    std::vector<cv::Point> body;

    void setGfx(Gfx g) {
        graphics = g;
    }

    Snake(int x, int y, Gfx g) {
        graphics = g;
        direction = 0; // start moving to the right
        body.emplace_back(x, y); // add the head of the snake
        body.emplace_back(x-graphics.getGridSize(), y);
        body.emplace_back(x-2*graphics.getGridSize(), y); // tail
    }

    void draw(cv::Mat& image) {
        for (int i = 1; i < body.size(); i++) {
            rectangle(image, cv::Rect(body[i].x, body[i].y, graphics.getGridSize(), graphics.getGridSize()), cv::Scalar(0, 0, 0), -1);
        }
    }

    void move() {
        cv::Point head = body[0];
        // right
        if (direction == 0) {
            head.x += graphics.getGridSize();
        }
        // down
        else if (direction == 1) {
            head.y += graphics.getGridSize();
        }
        // left
        else if (direction == 2) {
            head.x -= graphics.getGridSize();
        }
        // up
        else if (direction == 3) {
            head.y -= graphics.getGridSize();
        }
        body.insert(body.begin(), head); // move head to the new point
        body.pop_back(); // pop the tail
    }

    void set_direction(int new_direction) {
        direction = new_direction;
    }

    bool check_collision() {
        cv::Point head = body[0];
        // check collision with walls
        if (head.x < 0 || head.x >= graphics.getWidth()+ graphics.getGridSize() || head.y < 0 || head.y >= graphics.getHeight()+graphics.getGridSize()) {
            return true;
        }
        // check collision with body
        for (int i = 1; i < body.size(); i++) {
            if (head == body[i]) {
                return true;
            }
        }
        return false;
    }

    cv::Point get_tail() {
        return body[body.size()-1];
    }

    void grow() {
        body.push_back(get_tail());
    }

    int getDirection() const {
        return direction;
    }
};