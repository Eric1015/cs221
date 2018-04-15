#include "convexhull.h"
#include "stack.h"
#include <cmath>
#include <iostream>

using namespace std;

void sortByAngle(vector<Point>& v) {
	if (v.size() > 0) {
		swapMinY(v);
		for (unsigned i = 1; i < v.size(); i++) {
			int minIndex = i;
			for (unsigned j = i + 1; j < v.size(); j++) {
				if (radian_X_axis(v[0], v[j]) < radian_X_axis(v[0], v[minIndex])) {
					Point temp = v[minIndex];
					v[minIndex] = v[j];
					v[j] = temp;
				}
				else if (abs(radian_X_axis(v[0], v[j]) - radian_X_axis(v[0], v[minIndex])) < pow(0.1, 6)) {
					if (v[j].x < v[minIndex].x && v[j].y != v[minIndex].y) {
						Point temp = v[minIndex];
						v[minIndex] = v[j];
						v[j] = temp;
					}
					else if (v[j].y == v[minIndex].y && v[j].x < v[minIndex].x) {
						Point temp = v[minIndex];
						v[minIndex] = v[j];
						v[j] = temp;
					}
					else if (v[j].y < v[minIndex].y && v[j].x == v[minIndex].x) {
						Point temp = v[minIndex];
						v[minIndex] = v[j];
						v[j] = temp;
					}
				}
			}
		}
	}
}

bool ccw(Point p1, Point p2, Point p3) {
	if (abs(p2.x - p1.x) > pow(0.1, 10)) {
		double slope = (p2.y - p1.y) / (p2.x - p1.x);
		double intercept = p2.y - slope * p2.x;
		if (isOnLeft(slope, intercept, p3, p1, p2)) 
			return true;
		else
			return false;
	}
	else {
		if (p3.x > p2.x) {
			if (p1.y > p2.y)
				return true;
			else
				return false;
		}
		else if (p3.x < p2.x) {
			if (p2.y > p1.y)
				return true;
			else
				return false;
		}
		else
			return true;
	}
}

vector<Point> getConvexHull(vector<Point>& v) {
	vector<Point> convex;
	Stack temporal;
	sortByAngle(v);
	convex.push_back(v[0]);
	convex.push_back(v[1]);
	for (unsigned i = 2; i < v.size(); i++) {
		if (temporal.isEmpty()) {
			temporal.push(convex.back());
			convex.pop_back();
		}
		while ((!ccw(convex.back(), temporal.peek(), v[i]) || isOnStraightLine(convex.back(), temporal.peek(), v[i]))
			&& convex.size() > 0) {
			temporal.pop();
			temporal.push(convex.back());
			convex.pop_back();
		}
		addPointsWithoutDuplicate(convex, temporal.pop());
		addPointsWithoutDuplicate(convex, v[i]);
	}

	return convex;
}

bool isOnLeft(double slope, double intercept, Point p, Point p1, Point p2) {
	bool isLeft = false;
	bool isY_1Above = false;
	if (p1.y == p2.y) {
		if (p1.x < p2.x && isAbove(slope, intercept, p))
			isLeft = true;
		else if (p1.x > p2.x && isBelow(slope, intercept, p))
			isLeft = true;
	}
	else {
		if (p1.y - p2.y > 0)
			isY_1Above = true;
		if (slope > 0) {
			if (isY_1Above && isBelow(slope, intercept, p))
				isLeft = true;
			else if (!isY_1Above && isAbove(slope, intercept, p))
				isLeft = true;
		}
		else {
			if (isY_1Above && isAbove(slope, intercept, p)) {
				isLeft = true;
			}
			else if (!isY_1Above && isBelow(slope, intercept, p)) {
				isLeft = true;
			}
		}
	}
	return isLeft;
}

bool isAbove(double slope, double intercept, Point p) {
	bool above = false;
	double function_y = slope * p.x + intercept;
	if (p.y >= function_y)
		above = true;
	return above;
}

bool isBelow(double slope, double intercept, Point p) {
	bool below = false;
	double function_y = slope * p.x + intercept;
	if (p.y <= function_y)
		below = true;
	return below;
}

void swapMinY(vector<Point>& v) {
	int minIndex = 0;
	for (unsigned j = 1; j < v.size(); j++) {
		if (v[j].y < v[minIndex].y) {
			minIndex = j;
		}
		else if (v[j].y == v[minIndex].y) {
			if (v[j].x < v[minIndex].x) {
				minIndex = j;
			}
		}
	}
	Point temp = v[0];
	v[0] = v[minIndex];
	v[minIndex] = temp;
}

double radian_X_axis(Point p1, Point p2) {
	double vector_x = p2.x - p1.x;
	double vector_y = p2.y - p1.y;
	double length = sqrt(pow(vector_x, 2) + pow(vector_y, 2));
	return acos(vector_x / length);
}

bool isOnStraightLine(Point p1, Point p2, Point p3) {
	if (p1.x != p2.x) {
		double slope = (p1.y - p2.y) / (p1.x - p2.x);
		double intercept = p2.y - slope * p2.x;
		double function_y = slope * p3.x + intercept;
		if (abs(p3.y - function_y) < pow(0.1, 15))
			return true;
		else
			return false;
	}
	else {
		if (p1.x == p3.x)
			return true;
		else
			return false;
	}
}
void addPointsWithoutDuplicate(vector<Point>& v, Point p) {
	bool pointExists = false;
	for (unsigned i = 0; i < v.size(); i++) {
		if (v[i].x == p.x && v[i].y == p.y) {
			pointExists = true;
		}
	}
	if (!pointExists)
		v.push_back(p);
}
