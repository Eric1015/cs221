
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
/**
 * Constructor that builds a twoDtree out of the given PNG.
 * Every leaf in the tree corresponds to a pixel in the PNG.
 * Every non-leaf node corresponds to a rectangle of pixels
 * in the original PNG, represented by an (x,y) pair for the
 * upper left corner of the rectangle and an (x,y) pair for
 * lower right corner of the rectangle. In addition, the Node
 * stores a pixel representing the average color over the
 * rectangle.
 *
 * Every node's left and right children correspond to a partition
 * of the node's rectangle into two smaller rectangles. The node's
 * rectangle is split by the horizontal or vertical line that
 * results in the two smaller rectangles whose sum of squared
 * differences from their mean is as small as possible.
 *
 * The left child of the node will contain the upper left corner
 * of the node's rectangle, and the right child will contain the
 * lower right corner.
 *
* This function will build the stats object used to score the
* splitting lines. It will also call helper function buildTree.
 */

twoDtree::twoDtree(PNG & imIn){
		stats s=stats(imIn);
		pair<int,int> ul=make_pair(0,0);
		pair<int,int> lr=make_pair(imIn.width()-1,imIn.height()-1);
		width=imIn.width();
		height=imIn.height();
		root=buildTree(s,ul,lr);
	//std::cout << "done twoDtree" << '\n';
 /* your code here */
}
/**
* Private helper function for the constructor. Recursively builds
* the tree according to the specification of the constructor.
* @param s Contains the data used to split the rectangles
* @param ul upper left point of current node's rectangle.
* @param lr lower right point of current node's rectangle.
*/
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	Node *ori=new Node(ul,lr,s.getAvg(ul,lr));
	if(lr.first>ul.first && lr.second>ul.second){
		//vertical split
		pair<int,int>vnewlr=make_pair(ul.first,lr.second);
		pair<int,int>vnewul=make_pair(ul.first+1,ul.second);
		long vmin=s.getScore(ul,vnewlr)+s.getScore(vnewul,lr);
		for(int a =ul.first;a<lr.first;a++){
			pair<int,int>tvnewlr=make_pair(a,lr.second);
			pair<int,int>tvnewul=make_pair(a+1,ul.second);
			long totalscore=s.getScore(ul,tvnewlr)+s.getScore(tvnewul,lr);
			if(vmin>totalscore){
				vmin=totalscore;
				vnewlr=tvnewlr;
				vnewul=tvnewul;
			}
		}
		//horizontal split
		pair<int,int>hnewlr=make_pair(lr.first,ul.second);
		pair<int,int>hnewul=make_pair(ul.first,ul.second+1);
		long hmin=s.getScore(ul,hnewlr)+s.getScore(hnewul,lr);
		for(int a =ul.second;a<lr.second;a++){
			pair<int,int>thnewlr=make_pair(lr.first,a);
			pair<int,int>thnewul=make_pair(ul.first,a+1);
			long totalscore=s.getScore(ul,thnewlr)+s.getScore(thnewul,lr);
			if(hmin>totalscore){
				hmin=totalscore;
				hnewlr=thnewlr;
				hnewul=thnewul;
			}
		}
		pair<int,int>newlr;
		pair<int,int>newul;
		if(hmin>vmin){
			newul=vnewul;
			newlr=vnewlr;
		}else{
			newul=hnewul;
			newlr=hnewlr;
		}
		ori->left= buildTree(s,ul,newlr);
		ori->right= buildTree(s,newul,lr);
	 }
	else if(lr.first>ul.first && lr.second==ul.second){     // have the same y
		pair<int,int>vnewlr=make_pair(ul.first,lr.second);
		pair<int,int>vnewul=make_pair(ul.first+1,ul.second);
		long vmin=s.getScore(ul,vnewlr)+s.getScore(vnewul,lr);
		for(int a =ul.first;a<lr.first;a++){
			pair<int,int>tvnewlr=make_pair(a,lr.second);
			pair<int,int>tvnewul=make_pair(a+1,ul.second);
			long totalscore=s.getScore(ul,tvnewlr)+s.getScore(tvnewul,lr);
			if(vmin>totalscore){
				vmin=totalscore;
				vnewlr=tvnewlr;
				vnewul=tvnewul;
			}
		}
		ori->left= buildTree(s,ul,vnewlr);
		ori->right= buildTree(s,vnewul,lr);
	}else if(lr.second>ul.second && lr.first==ul.first){ //have the same x
		pair<int,int>hnewlr=make_pair(lr.first,ul.second);
		pair<int,int>hnewul=make_pair(ul.first,ul.second+1);
		long hmin=s.getScore(ul,hnewlr)+s.getScore(hnewul,lr);
		for(int a =ul.second;a<lr.second;a++){
			pair<int,int>thnewlr=make_pair(lr.first,a);
			pair<int,int>thnewul=make_pair(ul.first,a+1);
			long totalscore=s.getScore(ul,thnewlr)+s.getScore(thnewul,lr);
			if(hmin>totalscore){
				hmin=totalscore;
				hnewlr=thnewlr;
				hnewul=thnewul;
			}
		}
		ori->left= buildTree(s,ul,hnewlr);
		ori->right= buildTree(s,hnewul,lr);
	}
	return ori;
}
/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 */
PNG twoDtree::render(){
	/* your code here */
	PNG mypng= PNG(width,height);
	setPixel(mypng,root);
	return mypng;
}
void twoDtree::setPixel(PNG  &mypng, Node* root){
	if(root!=NULL){
		setPixel(mypng,root->left);
		setPixel(mypng,root->right);
		if(root->left==NULL && root->right==NULL){
			int x1=root->upLeft.first;
			int y1=root->upLeft.second;
			int x2=root->lowRight.first;
			int y2=root->lowRight.second;
			for(int a=x1;a<=x2;a++){
				for(int b=y1;b<=y2;b++){
					RGBAPixel *myRGBAPixel=mypng.getPixel(a,b);
					*myRGBAPixel=(root->avg);
				}
			}
		}
	}
}
/*
 *  Prune function trims subtrees as high as possible in the tree.
 *  A subtree is pruned (cleared) if at least pct of its leaves are within
 *  tol of the average color stored in the root of the subtree.
 *  Pruning criteria should be evaluated on the original tree, not
 *  on a pruned subtree. (we only expect that trees would be pruned once.)
 *
* You may want a recursive helper function for this one.
 */
void twoDtree::prune(double pct, int tol){
	prune(root,pct,tol);
}
void twoDtree::prune(Node* parent,double pct,int tol){        //getPruned helper for prune
	int totalLeaves=0;
	int withinLeaves=0;
	if(parent!=NULL){
		getWithIn(parent,parent,tol,totalLeaves,withinLeaves);
		if(((double)withinLeaves)/totalLeaves>=pct){
			freeNode(parent->left);
			freeNode(parent->right);
		}else{
			prune(parent->left,pct,tol);
			prune(parent->right,pct,tol);
		}
	}
}
void twoDtree::getWithIn(Node*root,Node* child,int tol,int & totalLeaves,int &withinLeaves){     //get the percentage
		//root->left;
	if(child->left!=NULL){
		getWithIn(root,child->left,tol,totalLeaves,withinLeaves);
	}
	if(child->right!=NULL){
		getWithIn(root,child->right,tol,totalLeaves,withinLeaves);
	}
	if(child->left==NULL && child->right==NULL){
		totalLeaves++;
		if(checkWithIn(root,child,tol)){
			withinLeaves++;
		}
	}
}
bool twoDtree::checkWithIn(Node* parent,Node* child, int tol){     //check a leaf is withing the tolerancde
	int r1=parent->avg.r;
	int r2=child->avg.r;
	int g1=parent->avg.g;
	int g2=child->avg.g;
	int b1=parent->avg.b;
	int b2=child->avg.b;
	return ((r1-r2)*(r1-r2)+(g1-g2)*(g1-g2)+(b1-b2)*(b1-b2))<=tol;
}
void twoDtree::freeNode(Node *&subRoot){
	if(subRoot!=NULL){
	 	freeNode(subRoot->left);
		freeNode(subRoot->right);
		delete subRoot;
		subRoot=NULL;
	}
}
void twoDtree::clear() {
	freeNode(root);
}
/**
* Copies the parameter other twoDtree into the current twoDtree.
* Does not free any memory. Called by copy constructor and op=.
* You may want a recursive helper function for this one.
* @param other The twoDtree to be copied.
*/
void twoDtree::copy(const twoDtree & orig){
	Node* otherRoot=orig.root;
	width=orig.width;
	height=orig.height;
	copy(this->root,otherRoot);
}
void twoDtree::copy(Node *&thisRoot,Node *otherRoot){
	if(otherRoot!=NULL){
		Node *newRoot=new Node(otherRoot->upLeft,otherRoot->lowRight,otherRoot->avg);
		thisRoot=newRoot;
		copy(thisRoot->left,otherRoot->left);
		copy(thisRoot->right,otherRoot->right);
	}
}
