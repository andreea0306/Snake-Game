//
// Created by andreea on 8/3/23.
//
#include "Defines.cpp"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Food {
private:
    cv::Point position;
public:

    Food(){
        position.x = (rand() % (WIDTH/GRID_SIZE)) * GRID_SIZE;
        position.y = (rand() % (HEIGHT/GRID_SIZE)) * GRID_SIZE;
    }

    void draw(cv::Mat& image) {
        rectangle(image, cv::Rect(position.x, position.y, GRID_SIZE, GRID_SIZE), cv::Scalar(0, 0, 255), -1);
    }

    cv::Point get_position() {
        return position;
    }
};