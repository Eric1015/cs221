
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include <cmath>
#include <vector>

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ 
	stats s = stats(imIn); 
	pair<int, int> start;
	pair<int, int> end;
	start.first = 0;
	start.second = 0;
	end.first = imIn.width() - 1;
	end.second = imIn.height() - 1;
	root = buildTree(s, start, end);
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int, int> ul, pair<int, int> lr) {
	RGBAPixel ave = s.getAvg(ul, lr);
	Node* subroot = new Node(ul, lr, ave);

	pair<int, int> min;
	pair<int, int> next;

	if (ul.first != lr.first || ul.second != lr.second) {
		int left_x = ul.first;
		int top_y = ul.second;
		int right_x = lr.first;
		int bottom_y = lr.second;

		long minScore1 = LONG_MAX;
		pair<int, int> min1;
		for (int x = left_x; x < right_x; x++) {
			pair<int, int> p1 = { x, bottom_y };
			pair<int, int> p2 = { x + 1, top_y };
			long score1 = s.getScore(ul, p1);
			long score2 = s.getScore(p2, lr);
			long sum = score1 + score2;
			if (sum < minScore1) {
				minScore1 = sum;
				min1 = p1;
			}
		}

		long minScore2 = LONG_MAX;
		pair<int, int> min2;
		for (int y = top_y; y < bottom_y; y++) {
			pair<int, int> p1 = { right_x, y };
			pair<int, int> p2 = { left_x, y + 1 };
			long score1 = s.getScore(ul, p1);
			long score2 = s.getScore(p2, lr);
			long sum = score1 + score2;
			if (sum < minScore2) {
				minScore2 = sum;
				min2 = p1;

			}
		}

		if (minScore1 < minScore2) {
			min = min1;
			next = { min1.first + 1, top_y };
		}
		else {
			min = min2;
			next = { left_x, min.second + 1 };
		}

		subroot->left = buildTree(s, ul, min);
		subroot->right = buildTree(s, next, lr);

	}
	else {
		return subroot;
	}
	return subroot;
}

PNG twoDtree::render(){
	int width = root->lowRight.first + 1 - root->upLeft.first;
	int height = root->lowRight.second + 1 - root->upLeft.second;
	PNG* image = new PNG(width, height);
	assignPixel(root, *image);
	return *image;
}

void twoDtree::assignPixel(Node* subroot, PNG &image) {
	if (subroot->left == NULL && subroot->right == NULL) {
		int left_x = subroot->upLeft.first;
		int top_y = subroot->upLeft.second;
		int right_x = subroot->lowRight.first;
		int bottom_y = subroot->lowRight.second;
		for (int x = left_x; x <= right_x; x++) {
			for (int y = top_y; y <= bottom_y; y++) {
				RGBAPixel *pixel = image.getPixel(x, y);
				*pixel = subroot->avg;
			}
		}
	}
	else {
		if (subroot->left != NULL)
			assignPixel(subroot->left, image);
		if (subroot->right != NULL)
			assignPixel(subroot->right, image);
	}
}

void twoDtree::prune(double pct, int tol){
	getHeighestSub(root, pct, tol);
}

void twoDtree::getHeighestSub(Node* subroot, double pct, int tol) {
	int numInRange = 0;
	int totalLeaves = 0;

	getNumIn(subroot, subroot, totalLeaves, numInRange, tol);

	if (numInRange >= (pct * totalLeaves)) {
		remove(subroot->left);
		remove(subroot->right);
		subroot->left = NULL;
		subroot->right = NULL;
	}
	else {
		if (subroot->left != NULL)
			getHeighestSub(subroot->left, pct, tol);
		if (subroot->right != NULL)
			getHeighestSub(subroot->right, pct, tol);
	}
}

void twoDtree::getNumIn(Node* subroot, Node* child, int &totalLeaves, int &numInRange, int tol) {
	if (child->left != NULL)
		getNumIn(subroot, child->left, totalLeaves, numInRange, tol);
	if (child->right != NULL)
		getNumIn(subroot, child->right, totalLeaves, numInRange, tol);
	if (child->left == NULL && child->right == NULL) {
		RGBAPixel leafrgb = child->avg;
		RGBAPixel rootrgb = subroot->avg;
		int distance = pow((rootrgb.r - leafrgb.r), 2) + pow((rootrgb.g - leafrgb.g), 2) + pow((rootrgb.b - leafrgb.b), 2);
		if (distance <= tol) {
			numInRange++;
		}
		totalLeaves++;
	}
}

void twoDtree::clear() {
	remove(root);
}

void twoDtree::remove(Node* subroot) {
	if (subroot != NULL) {
		if (subroot->left != NULL)
			remove(subroot->left);
		if (subroot->right != NULL)
			remove(subroot->right);
		subroot->left = NULL;
		subroot->right = NULL;
		delete subroot;
	}
}


void twoDtree::copy(const twoDtree & orig){
	root = preOrderCopy(orig.root);
	height = orig.height;
	width = orig.width;
}

twoDtree::Node* twoDtree::preOrderCopy(Node* otherSubroot) {
	if (otherSubroot != NULL) {
		Node* nodeptr = (Node*)malloc(sizeof(Node));
		nodeptr->upLeft = make_pair(otherSubroot->upLeft.first, otherSubroot->upLeft.second);
		nodeptr->lowRight = make_pair(otherSubroot->lowRight.first, otherSubroot->lowRight.second);
		nodeptr->avg = otherSubroot->avg;
		if (otherSubroot->left != NULL)
			nodeptr->left = preOrderCopy(otherSubroot->left);
		else
			nodeptr->left = NULL;
		if (otherSubroot->right != NULL)
			nodeptr->right = preOrderCopy(otherSubroot->right);
		else
			nodeptr->right = NULL;
		return nodeptr;
	}
	else
		return NULL;
}



