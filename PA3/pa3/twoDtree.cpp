
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

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	//cout << "From buildTree function, ul is x:" << ul.first << " y:" << ul.second << " lr is x:" << lr.first << " y:" << lr.second << endl;
	if (ul.first != lr.first || ul.second != lr.second) {
		pair<int, int> currUL;
		pair<int, int> currLR;
		currUL.first = ul.first;
		currUL.second = ul.second;
		currLR.first = ul.first;
		if (ul.first == lr.first)
			currLR.second = ul.second;
		else
			currLR.second = lr.second;
		long smallest = s.getScore(currUL, currLR);
		pair<int, int> smallestPair;
		smallestPair.first = currLR.first;
		smallestPair.second = currLR.second;
		for (int i = ul.first + 1; i < lr.first - 1; i++) {
			currLR.first = i;
			long temp = s.getScore(currUL, currLR);
			if (temp < smallest) {
				smallest = temp;
				smallestPair.first = currLR.first;
				smallestPair.second = currLR.second;
			}
		}
		currLR.first = lr.first;
		for (int i = ul.second; i < lr.second - 1; i++) {
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
		Node* childptr = &child;
		pair<int, int> next;
		if (smallestPair.first == lr.first) {
			next.first = ul.first;
			next.second = smallestPair.second + 1;
		}
		else if (smallestPair.second == lr.second) {
			next.first = smallestPair.first + 1;
			next.second = ul.second;
		}

		childptr->left = buildTree(s, ul, smallestPair);
		childptr->right = buildTree(s, next, lr);
		cout << "The current other root is upLeft x:" << childptr->upLeft.first << " y:" << childptr->upLeft.second << " lowRight x:" << childptr->lowRight.first << " y:" << childptr->lowRight.second << " ";
		cout << "The current other root's left is upLeft x:" << childptr->left->upLeft.first << " y:" << childptr->left->upLeft.second << " lowRight x:" << childptr->left->lowRight.first << " y:" << childptr->left->lowRight.second << endl;

		return childptr;
	}
	else {
		//cout << "ul x:" << ul.first << " y:" << ul.second << " lr x:" << lr.first << " y:" << lr.second << " is added to leaf" << endl;
		RGBAPixel ave = s.getAvg(ul, lr);
		Node node = Node(ul, lr, ave);
		Node* nodeptr = &node;
		return nodeptr;
	}
}

PNG twoDtree::render(){
	int width = root->upLeft.first - root->lowRight.first;
	int height = root->upLeft.second - root->lowRight.second;
	PNG image = PNG(width, height);
	assignPixel(root, image);
	return image;
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
	Node* subtree = getHeighestSub(root, pct, tol);
	remove(subtree);
}

twoDtree::Node* twoDtree::getHeighestSub(Node* subroot, double pct, int tol) {
	vector<Node*> leaves;
	getLeaves(subroot, leaves);
	int numInRange = 0;
	for (int i = 0; i < (int)leaves.size(); i++) {
		RGBAPixel leavesrgb = leaves[i]->avg;
		RGBAPixel rootrgb = root->avg;
		int distance = pow((rootrgb.r - leavesrgb.r), 2) + pow((rootrgb.g - leavesrgb.g), 2) + pow((rootrgb.b - leavesrgb.b), 2);
		if (distance <= tol) {
			numInRange++;
		}
	}
	if (numInRange / leaves.size() >= pct) {
		 return subroot;
	}
	else {
		Node* left = getHeighestSub(subroot->left, pct, tol);
		Node* right = getHeighestSub(subroot->right, pct, tol);
		int leftArea = (left->lowRight.first - left->upLeft.first) * (left->lowRight.second - left->upLeft.second);
		int rightArea = (right->lowRight.first - right->upLeft.first) * (right->lowRight.second - right->upLeft.second);
		return (leftArea > rightArea) ? left : right;
	}

}

void twoDtree::getLeaves(Node* subroot, vector<Node*> &leaves) {
	if (subroot->left == NULL && subroot->right == NULL) {
		leaves.push_back(subroot);
	}
	else {
		getLeaves(subroot->left, leaves);
		getLeaves(subroot->right, leaves);
	}
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
	root = preOrderCopy(orig.root);
	height = orig.height;
	width = orig.width;
}

twoDtree::Node* twoDtree::preOrderCopy(Node* otherSubroot) {
	if (otherSubroot != NULL) {
		cout << "The current other root is upLeft x:" << otherSubroot->upLeft.first << " y:" << otherSubroot->upLeft.second << " lowRight x:" << otherSubroot->lowRight.first << " y:" << otherSubroot->lowRight.second << endl;
		cout << "The current other root's left is upLeft x:" << otherSubroot->left->upLeft.first << " y:" << otherSubroot->left->upLeft.second << " lowRight x:" << otherSubroot->left->lowRight.first << " y:" << otherSubroot->left->lowRight.second << endl;
		Node node = Node(otherSubroot->upLeft, otherSubroot->lowRight, otherSubroot->avg);
		Node* nodeptr = &node;
		bool a = otherSubroot->left != NULL;
		cout << "The left node is " << a << endl;
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



