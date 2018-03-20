
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

twoDtree::twoDtree(PNG & imIn){ /* your code here */ }

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	/* your code here */
}

PNG twoDtree::render(){
	/* your code here */
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



