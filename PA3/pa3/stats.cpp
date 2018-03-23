#include "stats.h"
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <utility>
#include <vector>
using namespace std;
using namespace cs221util;

long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr) {
	//TODO::consider the case that channel is not any of r, g, b
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
	//TODO::consider the case that channel is not any of r, g, b
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
	//TODO::consider the case im is NULL
	int width = im.width();
	int height = im.height();
	sumRed = vector<vector<long>>(height, vector<long>(width, 0));
	sumsqRed = vector<vector<long>>(height, vector<long>(width, 0));
	sumBlue = vector<vector<long>>(height, vector<long>(width, 0));
	sumsqBlue = vector<vector<long>>(height, vector<long>(width, 0));
	sumGreen = vector<vector<long>>(height, vector<long>(width, 0));
	sumsqGreen = vector<vector<long>>(height, vector<long>(width, 0));

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			RGBAPixel* current = im.getPixel(i, j);
			long sum = current->r;
			if (i > 0)
				sum += sumRed[i - 1][j];
			if (j > 0)
				sum += sumRed[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumRed[i - 1][j - 1];
			sumRed[i].push_back(sum);
			sum = current->r;
			sum *= sum;
			if (i > 0)
				sum += sumsqRed[i - 1][j];
			if (j > 0)
				sum += sumsqRed[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumsqRed[i - 1][j - 1];
			sumsqRed[i].push_back(sum);

			sum = current->b;
			if (i > 0)
				sum += sumBlue[i - 1][j];
			if (j > 0)
				sum += sumBlue[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumBlue[i - 1][j - 1];
			sumBlue[i].push_back(sum);

			sum = current->b;
			sum *= sum;
			if (i > 0)
				sum += sumsqBlue[i - 1][j];
			if (j > 0)
				sum += sumsqBlue[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumsqBlue[i - 1][j - 1];
			sumsqBlue[i].push_back(sum);

			sum = current->g;
			if (i > 0)
				sum += sumGreen[i - 1][j];
			if (j > 0)
				sum += sumGreen[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumGreen[i - 1][j - 1];
			sumGreen[i].push_back(sum);

			sum = current->g;
			sum *= sum;
			if (i > 0)
				sum += sumsqGreen[i - 1][j];
			if (j > 0)
				sum += sumsqGreen[i][j - 1];
			if (i > 0 && j > 0)
				sum -= sumsqGreen[i - 1][j - 1];
			sumsqGreen[i].push_back(sum);
		}
	}
}

long stats::getScore(pair<int, int> ul, pair<int, int> lr) {
	return getSumSq('r', ul, lr) - getSum('r', ul, lr) / rectArea(ul, lr)
		+ getSumSq('b', ul, lr) - getSum('b', ul, lr) / rectArea(ul, lr)
		+ getSumSq('g', ul, lr) - getSum('g', ul, lr) / rectArea(ul, lr);
}

RGBAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr) {
	int red = getSum('r', ul, lr) / rectArea(ul, lr);
	int blue = getSum('b', ul, lr) / rectArea(ul, lr);
	int green = getSum('g', ul, lr) / rectArea(ul, lr);
	RGBAPixel ave = RGBAPixel(red, green, blue);
	return ave;
}

long stats::rectArea(pair<int, int> ul, pair<int, int> lr) {
	long height = lr.second - ul.second + 1;
	long width = lr.first - ul.first + 1;
	return height * width;
}