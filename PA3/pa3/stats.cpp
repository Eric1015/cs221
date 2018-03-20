#include "stats.h"
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <utility>
#include <vector>
using namespace std;
using namespace cs221util;

long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr) {
	long sum = 0;
	int left_x = 0;
	if (ul.first > 0)
		left_x = ul.first - 1;
	else
		left_x = ul.first;
	int right_x = lr.first;
	int top_y = 0;
	if (ul.second > 0)
		top_y = ul.second - 1;
	else
		top_y = ul.second;
	int bottom_y = lr.second;
	if (channel == 'r') {
		sum = sumRed[right_x][bottom_y];
		sum -= sumRed[left_x][bottom_y];
		sum -= sumRed[right_x][top_y];
		sum += sumRed[left_x][top_y];
	}
	else if (channel == 'g') {
		sum = sumGreen[right_x][bottom_y];
		sum -= sumGreen[left_x][bottom_y];
		sum -= sumGreen[right_x][top_y];
		sum += sumGreen[left_x][top_y];
	}
	else if (channel == 'b') {
		sum = sumBlue[right_x][bottom_y];
		sum -= sumBlue[left_x][bottom_y];
		sum -= sumBlue[right_x][top_y];
		sum += sumBlue[left_x][top_y];
	}
	return sum;
}

long stats::getSumSq(char channel, pair<int, int> ul, pair<int, int> lr) {
	long sum = 0;
	int left_x = 0;
	if (ul.first > 0)
		left_x = ul.first - 1;
	else
		left_x = ul.first;
	int right_x = lr.first;
	int top_y = 0;
	if (ul.second > 0)
		top_y = ul.second - 1;
	else
		top_y = ul.second;
	int bottom_y = lr.second;
	if (channel == 'r') {
		sum = sumsqRed[right_x][bottom_y];
		sum -= sumsqRed[left_x][bottom_y];
		sum -= sumsqRed[right_x][top_y];
		sum += sumsqRed[left_x][top_y];
	}
	else if (channel == 'g') {
		sum = sumsqGreen[right_x][bottom_y];
		sum -= sumsqGreen[left_x][bottom_y];
		sum -= sumsqGreen[right_x][top_y];
		sum += sumsqGreen[left_x][top_y];
	}
	else if (channel == 'b') {
		sum = sumsqBlue[right_x][bottom_y];
		sum -= sumsqBlue[left_x][bottom_y];
		sum -= sumsqBlue[right_x][top_y];
		sum += sumsqBlue[left_x][top_y];
	}
	return sum;
}

stats::stats(PNG & im) {
	int width = im.width();
	int height = im.height();
	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < height; j++) {

		}
	}
}