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
			RGBAPixel* current = im.getPixel(i, j);
			long sum = current->r;
			if (i > 0)
				sum += sumRed[i - 1][j];
			if (j > 0)
				sum += sumRed[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumRed[i - 1][j - 1];
			sumRed[i][j] = sum;

			sum = current->r;
			sum *= sum;
			if (i > 0)
				sum += sumsqRed[i - 1][j];
			if (j > 0)
				sum += sumsqRed[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumsqRed[i - 1][j - 1];
			sumsqRed[i][j] = sum;

			sum = current->b;
			if (i > 0)
				sum += sumBlue[i - 1][j];
			if (j > 0)
				sum += sumBlue[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumBlue[i - 1][j - 1];
			sumBlue[i][j] = sum;

			sum = current->b;
			sum *= sum;
			if (i > 0)
				sum += sumsqBlue[i - 1][j];
			if (j > 0)
				sum += sumsqBlue[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumsqBlue[i - 1][j - 1];
			sumsqBlue[i][j] = sum;

			sum = current->g;
			if (i > 0)
				sum += sumGreen[i - 1][j];
			if (j > 0)
				sum += sumGreen[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumGreen[i - 1][j - 1];
			sumGreen[i][j] = sum;

			sum = current->g;
			sum *= sum;
			if (i > 0)
				sum += sumsqGreen[i - 1][j];
			if (j > 0)
				sum += sumsqGreen[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumsqGreen[i - 1][j - 1];
			sumsqGreen[i][j] = sum;
		}
	}
}