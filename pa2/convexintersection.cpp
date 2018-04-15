#include "convexintersection.h"
#include "convexhull.h"
#include "stack.h"

bool inside(Point p1, Point sp1, Point sp2) {
	if (ccw(sp1, sp2, p1))
		return true;
	else
		return false;
}

Point computeIntersection(Point s1, Point s2, Point i1, Point i2) {
	if (s1.x != s2.x && i1.x != i2.x) {
		double slope_s = (s2.y - s1.y) / (s2.x - s1.x);
		double intercept_s = s2.y - slope_s * s2.x;
		double slope_i = (i2.y - i1.y) / (i2.x - i1.x);
		double intercept_i = i2.y - slope_i * i2.x;
		if (slope_i != slope_s) {
			double x = (intercept_s - intercept_i) / (slope_i - slope_s);
			double y = slope_s * x + intercept_s;
			if (pointInRange(Point(x, y), s1, s2) && pointInRange(Point(x, y), i1, i2))
				return Point(x, y);
			else
				return Point(0, 0);
		}
		else
			return Point(0, 0);
	}
	else if (s1.x == s2.x && i1.x != i2.x) 
		return verticalIntersection(s1, s2, i1, i2);
	else if (s1.x != s2.x && i1.x == i2.x) 
		return verticalIntersection(i1, i2, s1, s2);
	else
		return Point(0, 0);
}

vector<Point> getConvexIntersection(vector<Point>& poly1, vector<Point>& poly2) {
	vector<Point> intersection;
	addPointsInside(intersection, poly1, poly2);
	addPointsInside(intersection, poly2, poly1);
	for (unsigned i = 0; i < poly1.size() - 1; i++) {
		for (unsigned j = 0; j < poly2.size() - 1; j++) {
			Point p = computeIntersection(poly1[i], poly1[i + 1], poly2[j], poly2[j + 1]);
			if (!(p.x == 0 && p.y == 0))
				addPointsWithoutDuplicate(intersection, p);
		}
		Point p = computeIntersection(poly1[i], poly1[i + 1], poly2[poly2.size() - 1], poly2[0]);
		if (!(p.x == 0 && p.y == 0))
			addPointsWithoutDuplicate(intersection, p);
	}
	for (unsigned j = 0; j < poly2.size() - 1; j++) {
		Point p = computeIntersection(poly1[poly1.size() - 1], poly1[0], poly2[j], poly2[j + 1]);
		if (!(p.x == 0 && p.y == 0))
			addPointsWithoutDuplicate(intersection, p);
	}
	Point p = computeIntersection(poly1[poly1.size() - 1], poly1[0], poly2[poly2.size() - 1], poly2[0]);
	if (!(p.x == 0 && p.y == 0))
		addPointsWithoutDuplicate(intersection, p);

	sortByAngle(intersection);

	return intersection;
}

bool pointInRange(Point p1, Point sp1, Point sp2) {
	if (sp1.y > sp2.y) {
		if (p1.y > sp1.y || p1.y < sp2.y)
			return false;
	}
	else {
		if (p1.y < sp1.y || p1.y > sp2.y)
			return false;
	}
	
	if (sp1.x > sp2.x) {
		if (p1.x > sp1.x || p1.x < sp2.x)
			return false;
	}
	else {
		if (p1.x < sp1.x || p1.x > sp2.x)
			return false;
	}
	return true;
}

void addPointsInside(vector<Point>& intersection, const vector<Point> poly1, const vector<Point> poly2) {
	for (unsigned i = 0; i < poly1.size(); i++) {
		bool isInside = true;
		for (unsigned j = 0; j < poly2.size() - 1; j++) {
			if (!inside(poly1[i], poly2[j], poly2[j + 1])) {
				isInside = false;
				break;
			}
		}
		if (!inside(poly1[i], poly2[poly2.size() - 1], poly2[0]))
			isInside = false;
		if (isInside)
			addPointsWithoutDuplicate(intersection, poly1[i]);
	}
}

Point verticalIntersection(Point ver1, Point ver2, Point p1, Point p2) {
	double slope = (p2.y - p1.y) / (p2.x - p1.x);
	double intercept = p2.y - slope * p2.x;
	double x = ver1.x;
	double y = slope * x + intercept;
	if (pointInRange(Point(x, y), ver1, ver2) && pointInRange(Point(x, y), p1, p2))
		return Point(x, y);
	else
		return Point(0, 0);
}
