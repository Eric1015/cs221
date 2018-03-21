
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

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
	end.first = imIn.width();
	end.second = imIn.height();
	root = buildTree(s, start, end);
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	if (ul.first != lr.first || ul.second != lr.second) {
		pair<int, int> currUL;
		pair<int, int> currLR;
		currUL.first = ul.first;
		currUL.second = ul.second;
		currLR.first = ul.first;
		currLR.second = lr.second;
		long smallest = s.getScore(currUL, currLR);
		pair<int, int> smallestPair;
		smallestPair.first = currLR.first;
		smallestPair.second = currLR.second;
		for (unsigned i = ul.first + 1; i < lr.first; i++) {
			currLR.first = i;
			long temp = s.getScore(currUL, currLR);
			if (temp < smallest) {
				smallest = temp;
				smallestPair.first = currLR.first;
				smallestPair.second = currLR.second;
			}
		}
		for (unsigned i = ul.second; i < lr.second; i++) {
			currLR.second = i;
			long temp = s.getScore(currUL, currLR);
			if (temp < smallest) {
				smallest = temp;
				smallestPair.first = currLR.first;
				smallestPair.second = currLR.second;
			}
		}
		RGBAPixel ave = s.getAvg(ul, lr);
		Node child = Node(ul, lr, ave);
		Node* leftChild = buildTree(s, ul, smallestPair);
		pair<int, int> next;
		if (smallestPair.first == lr.first) {
			next.first = ul.first;
			next.second = leftChild->lowRight.second + 1;
		}
		else if (smallestPair.second == ul.second) {
			next.first = leftChild->lowRight.first + 1;
			next.second = ul.second;
		}
		Node* rightChild = buildTree(s, next, lr);
		child.left = leftChild;
		child.right = rightChild;

		return &child;
	}
	else {
		RGBAPixel ave = s.getAvg(ul, lr);
		Node node = Node(ul, lr, ave);
		return &node;
	}
}

PNG twoDtree::render(){
	int width = root->upLeft.first - root->lowRight.first;
	int height = root->upLeft.second - root->lowRight.second;
	PNG image = PNG((root->upLeft.first - root->lowRight.first), (root->upLeft.second - root->lowRight.second));
	assignPixel(root, image);
}

void twoDtree::assignPixel(Node* subroot, PNG image) {
	if (subroot == NULL) {
		*image.getPixel(subroot->upLeft.first, subroot->upLeft.second) = subroot->avg; 
	}
	else {
		assignPixel(subroot->left, image);
		assignPixel(subroot->right, image);
	}
}

void twoDtree::prune(double pct, int tol){
	/* your code here */
}

void twoDtree::clear() {
	remove(root);
}

void twoDtree::remove(Node* subroot) {
	if (subroot->left != NULL)
		remove(subroot->left);
	if (subroot->right != NULL)
		remove(subroot->right);
	subroot->left = NULL;
	subroot->right = NULL;
	free(subroot);
}


void twoDtree::copy(const twoDtree & orig){
	preOrderCopy(root, orig.root);
	height = orig.height;
	width = orig.width;
}

void twoDtree::preOrderCopy(Node* subroot, Node* otherSubroot) {
	Node newNode = Node(otherSubroot->upLeft, otherSubroot->lowRight, otherSubroot->avg);
	subroot = &newNode;
	if (otherSubroot->left != NULL)
		preOrderCopy(subroot->left, otherSubroot->left);
	if (otherSubroot->right != NULL)
		preOrderCopy(subroot->right, otherSubroot->right);
}



