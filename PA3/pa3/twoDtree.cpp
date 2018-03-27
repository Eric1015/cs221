
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
	//cout << "From twoDtree constructor, the root's left is x:" << root->left->upLeft.first << " y:" << root->left->upLeft.second << " x:" << root->left->lowRight.first << " y:" << root->left->lowRight.second << endl;
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	//cout << "From buildTree function, ul is x:" << ul.first << " y:" << ul.second << " lr is x:" << lr.first << " y:" << lr.second << endl;
	if (ul.first != lr.first || ul.second != lr.second) {
		pair<int, int> currUL = make_pair(ul.first, ul.second);
		pair<int, int> currLR;
		if (ul.first == lr.first)
			currLR = make_pair(ul.first, ul.second);
		else
			currLR = make_pair(ul.first, lr.second);
		long smallest = s.getScore(currUL, currLR);
		pair<int, int> smallestPair = make_pair(currLR.first, currLR.second);
		for (int i = ul.first + 1; i < lr.first - 1; i++) {
			currLR.first = i;
			long temp = s.getScore(currUL, currLR);
			if (temp < smallest) {
				smallest = temp;
				smallestPair = make_pair(currLR.first, currLR.second);
			}
		}
		currLR = make_pair(lr.first, currLR.second);
		for (int i = ul.second; i < lr.second - 1; i++) {
			currLR = make_pair(currLR.first, i);
			long temp = s.getScore(currUL, currLR);
			if (temp < smallest) {
				smallest = temp;
				smallestPair = make_pair(currLR.first, currLR.second);
			}
		}
		RGBAPixel ave = s.getAvg(ul, lr);
		Node* childptr = (Node*)malloc(sizeof(Node));
		childptr->upLeft = make_pair(ul.first, ul.second);
		childptr->lowRight = make_pair(lr.first, ul.second);
		childptr->avg = ave;
		pair<int, int> next;
		if (smallestPair.first == lr.first) {
			next = make_pair(ul.first, smallestPair.second + 1);
		}
		else if (smallestPair.second == lr.second) {
			next = make_pair(smallestPair.first + 1, ul.second);
		}

		childptr->left = buildTree(s, ul, smallestPair);
		childptr->right = buildTree(s, next, lr);
		//cout << "The current other root is upLeft x:" << childptr->upLeft.first << " y:" << childptr->upLeft.second << " lowRight x:" << childptr->lowRight.first << " y:" << childptr->lowRight.second << " ";
		//cout << "The current other root's left is upLeft x:" << childptr->left->upLeft.first << " y:" << childptr->left->upLeft.second << " lowRight x:" << childptr->left->lowRight.first << " y:" << childptr->left->lowRight.second << endl;
		return childptr;
	}
	else {
		//cout << "ul x:" << ul.first << " y:" << ul.second << " lr x:" << lr.first << " y:" << lr.second << " is added to leaf" << endl;
		RGBAPixel ave = s.getAvg(ul, lr);
		Node* nodeptr = (Node*)malloc(sizeof(Node));
		nodeptr->left = NULL;
		nodeptr -> right = NULL;
		nodeptr->upLeft = ul;
		nodeptr->lowRight = lr;
		nodeptr->avg = ave;
		return nodeptr;
	}
}

void twoDtree::printXY(int lineno, Node* subroot) {
	if (subroot != NULL)
		cout << lineno << ": The current root is ul x:" << subroot->upLeft.first << " y:" << subroot->upLeft.second << " lr is x:" << subroot->lowRight.first << " y:" << subroot->lowRight.second << endl;
	else
		cout << lineno << ": The current root is NULL" << endl;
}
void twoDtree::print() {
	printHelper(root);
}

void twoDtree::printHelper(Node* subroot) {
	if (subroot != NULL) {
		cout << "(" << subroot->upLeft.first << ", " << subroot->upLeft.second << ") (" << subroot->lowRight.first << ", " << subroot->lowRight.second << ") " << endl;
		if (subroot->left != NULL)
			printHelper(subroot->left);
		if (subroot->right != NULL)
			printHelper(subroot->right);
	}
}

PNG twoDtree::render(){
	int width = root->lowRight.first + 1 - root->upLeft.first;
	int height = root->lowRight.second + 1 - root->upLeft.second;
	PNG image = PNG(width, height);
	assignPixel(root, image);
	return image;
}

void twoDtree::assignPixel(Node* subroot, PNG image) {
	if (subroot->left == NULL && subroot->right == NULL) {
		printXY(__LINE__, subroot);
		RGBAPixel *pixel = image.getPixel(subroot->lowRight.first, subroot->lowRight.second);
		*pixel = subroot->avg;
		cout << "succeed" << endl;
	}
	else {
		if (subroot->left != NULL)
			assignPixel(subroot->left, image);
		if (subroot->right != NULL)
			assignPixel(subroot->right, image);
	}
}

void twoDtree::prune(double pct, int tol){
	Node* subtree = getHeighestSub(root, pct, tol);
	cout << "===============================================" << endl;
	if (subtree != NULL)
		remove(subtree);
}

twoDtree::Node* twoDtree::getHeighestSub(Node* subroot, double pct, int tol) {
	printXY(00000000, subroot);
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
	else if (subroot->left != NULL && subroot->right != NULL) {
		Node* left = getHeighestSub(subroot->left, pct, tol);
		Node* right = getHeighestSub(subroot->right, pct, tol);
		printXY(7777777, left);
		printXY(7777777, right);
		if (left != NULL && right != NULL) {
			int leftArea = (left->lowRight.first + 1 - left->upLeft.first) * (left->lowRight.second + 1 - left->upLeft.second);
			int rightArea = (right->lowRight.first + 1 - right->upLeft.first) * (right->lowRight.second + 1 - right->upLeft.second);
			return (leftArea > rightArea) ? left : right;
		}
		else if (right == NULL && left != NULL)
			return left;
		else if (left == NULL && right != NULL)
			return right;
		else
			return NULL;
	}
	else if (subroot->right == NULL && subroot->left != NULL) {
		Node* left = getHeighestSub(subroot->left, pct, tol);
		return left;
	}
	else if (subroot->left == NULL && subroot->right != NULL) {
		Node* right = getHeighestSub(subroot->right, pct, tol);
		return right;
	}
	else
		return NULL;

}

void twoDtree::getLeaves(Node* subroot, vector<Node*> &leaves) {
	if (subroot->left == NULL && subroot->right == NULL) {
		printXY(__LINE__, subroot);
		leaves.push_back(subroot);
	}
	else {
		if (subroot->left != NULL) {
			printXY(77, subroot->left);
			getLeaves(subroot->left, leaves);
		}
		if (subroot->right != NULL) {
			printXY(77, subroot->right);
			getLeaves(subroot->right, leaves);
		}
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
		free(subroot);
		print();
		cout << endl;
	}
}


void twoDtree::copy(const twoDtree & orig){
	root = preOrderCopy(orig.root);
	height = orig.height;
	width = orig.width;
	print();
}

twoDtree::Node* twoDtree::preOrderCopy(Node* otherSubroot) {
	if (otherSubroot != NULL) {
		//cout << "The current other root is upLeft x:" << otherSubroot->upLeft.first << " y:" << otherSubroot->upLeft.second << " lowRight x:" << otherSubroot->lowRight.first << " y:" << otherSubroot->lowRight.second << endl;
		//cout << "The current other root's left is upLeft x:" << otherSubroot->left->upLeft.first << " y:" << otherSubroot->left->upLeft.second << " lowRight x:" << otherSubroot->left->lowRight.first << " y:" << otherSubroot->left->lowRight.second << endl;
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



