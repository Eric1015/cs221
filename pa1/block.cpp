#include "block.h"
#include <iostream>
#include <vector>

using namespace std;

int Block::width() const{
   return data[0].size();
}
int Block::height() const{
   return data.size();
}

void Block::render(PNG & im, int upLeftX, int upLeftY) const {
    /* your code here */
    for (int i = 0; i < height(); i++){
        for (int j = 0; j < width(); j++){
            HSLAPixel* currentPointer = im.getPixel(j + upLeftX, i + upLeftY);
            *currentPointer = data[i][j];
        }
    }
    
    return;
}

void Block::build(PNG & im, int upLeftX, int upLeftY, int cols, int rows) {
   /* your code here */
    for (int i = 0; i < rows; i++){
        vector<HSLAPixel> vector;
        for (int j = 0; j < cols; j++){
            HSLAPixel *currentPointer = im.getPixel(j + upLeftX, i + upLeftY);
            vector.push_back(*currentPointer);
            //delete currentPointer;
        }
        data.push_back(vector);
    }
   return;
}
