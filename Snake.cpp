//
// Created by andreea on 8/3/23.
//
#include "Defines.cpp"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

class Snake {
private:
    int direction;

public:
    std::vector<cv::Point> body;

    Snake(int x, int y) {
        direction = 0; // start moving to the right
        body.push_back(cv::Point(x, y)); // add the head of the snake
        body.push_back(cv::Point(x-GRID_SIZE, y));
        body.push_back(cv::Point(x-2*GRID_SIZE, y));
    }

    void draw(cv::Mat& image) {
        for (int i = 0; i < body.size(); i++) {
            rectangle(image, cv::Rect(body[i].x, body[i].y, GRID_SIZE, GRID_SIZE), cv::Scalar(0, 255, 0), -1);
        }
    }

    void move() {
        cv::Point head = body[0];
        // right
        if (direction == 0) {
            head.x += GRID_SIZE;
        }
            // down
        else if (direction == 1) {
            head.y += GRID_SIZE;
        }
        // left
        else if (direction == 2) {
            head.x -= GRID_SIZE;
        }
        // up
        else if (direction == 3) {
            head.y -= GRID_SIZE;
        }
        body.insert(body.begin(), head);
        body.pop_back();
    }

    void set_direction(int new_direction) {
        direction = new_direction;
    }

    bool check_collision() {
        cv::Point head = body[0];
        // check collision with walls
        if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
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

};