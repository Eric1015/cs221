#include "chain.h"
#include "chain_given.cpp"

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){ 
    this->clear();
}

/**
 * Inserts a new node in position one of the Chain,
 * after the sentinel node.
 * This function **SHOULD** create a new Node.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertFront(const Block & ndata){
    Node* newNode = new Node(ndata);
    Node* oldHead = this->head_->next;
    this->head_ ->next= newNode;
    newNode->next = oldHead;
    newNode->prev = this->head_;
    oldHead->prev = newNode;
    length_++;
}

/**
 * Inserts a new node at the back of the Chain,
 * but before the tail sentinel node.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
    //Node node(ndata);
    Node *newNode = new Node(ndata);
    Node* oldTail = this->tail_->prev;
    this->tail_->prev = newNode;
    oldTail->next = newNode;
    newNode->prev = oldTail;
    newNode->next = this->tail_;
    length_++;
}

/**
 * Modifies the Chain by moving a contiguous subset of Nodes to
 * the end of the chain. The subset of len nodes from (and
 * including) startPos are moved so that they
 * occupy the last len positions of the
 * chain (maintaining the sentinel at the end). Their order is
 * not changed in the move.
 */
void Chain::moveToBack(int startPos, int len){
    int currentPos = 1;
    Node* current = this->head_->next;
    if (currentPos < startPos){
        while (currentPos != startPos){
            currentPos++;
            current = current->next;
        }
    }
    
    if (len <= length_){
        for (int i = 0; i < len; i++){
            Node* oldTail = this->tail_->prev;
            Node* beforeCurrent = current->prev;
            Node* afterCurrent = current->next;
            beforeCurrent->next = afterCurrent;
            afterCurrent->prev = beforeCurrent;
            this->tail_->prev = current;
            oldTail->next = current;
            current->prev = oldTail;
            current->next = this->tail_;
            current = afterCurrent;
        }
    }
}

/**
 * Rotates the current Chain by k nodes: removes the first
 * k nodes from the Chain and attaches them, in order, at
 * the end of the chain (maintaining the sentinel at the end).
 */
void Chain::rotate(int k){
    Node* oldHead = this->head_->next;
    Node* oldTail = this->tail_->prev;
    Node* current = this->head_->next;
    for (int i = 1; i < k; i++){
        current = current->next;
    }
    Node* afterK = current->next;
    oldHead->prev = oldTail;
    oldTail->next = oldHead;
    this->head_->next = afterK;
    afterK->prev = this->head_;
    this->tail_->prev = current;
    current->next = this->tail_;
}

/**
 * Modifies the current chain by swapping the Node at pos1
 * with the Node at pos2. the positions are 1-based.
 */
void Chain::swap(int pos1, int pos2){
    if (pos1 < size() && pos2 < size()){
        Node* current = head_->next;
        int currentIndex = 1;
        if (pos1 > pos2){
            for (currentIndex = 1; currentIndex < pos2; currentIndex++){
                current = current->next;
            }
            Node* nodeAtPos2  = current;
            Node* beforePos2  = current->prev;
            Node* afterPos2 = current->next;
            int newCurrentIndex = currentIndex;
            for (currentIndex = newCurrentIndex; currentIndex < pos1; currentIndex++){
                current = current->next;
            }
            Node* nodeAtPos1 = current;
            Node* beforePos1 = current->prev;
            Node* afterPos1 = current->next;
            
            nodeAtPos2->prev = beforePos1;
            nodeAtPos2->next = afterPos1;
            nodeAtPos1->prev = beforePos2;
            nodeAtPos1->next = afterPos2;
            beforePos1->next = nodeAtPos2;
            afterPos1->prev = nodeAtPos2;
            beforePos2->next = nodeAtPos1;
            afterPos2->prev = nodeAtPos1;
        }
        else if (pos1 < pos2){
            for (currentIndex = 1; currentIndex < pos1; currentIndex++){
                current = current->next;
            }
            Node* nodeAtPos1  = current;
            Node* beforePos1  = current->prev;
            Node* afterPos1 = current->next;
            int newCurrentIndex = currentIndex;
            for (currentIndex = newCurrentIndex; currentIndex < pos2; currentIndex++){
                current = current->next;
            }
            Node* nodeAtPos2 = current;
            Node* beforePos2 = current->prev;
            Node* afterPos2 = current->next;
            
            nodeAtPos2->prev = beforePos1;
            nodeAtPos2->next = afterPos1;
            nodeAtPos1->prev = beforePos2;
            nodeAtPos1->next = afterPos2;
            beforePos1->next = nodeAtPos2;
            afterPos1->prev = nodeAtPos2;
            beforePos2->next = nodeAtPos1;
            afterPos2->prev = nodeAtPos1;
        }
    }
}

/*
 *  Modifies both the current and input chains by trading
 * nodes in even positions between the chains. Length
 * of each chain should stay the same. If the block dimensions
 * are NOT the same, the funtion has no effect and a simple
 * error message is output: cout << "Block sizes differ." << endl;
 */

void Chain::twist(Chain & other){
    if(this->size() == other.size()){
         Node* thisCurrent = this->head_->next->next;
         Node* otherCurrent = other.head_->next->next;
         for (int i = 2; i  < size(); i = i + 2){
              if (thisCurrent->data.width() != otherCurrent->data.width()){
                 std::cout << "Block sizes differ" << std::endl;
                 break;
              }
              Node* beforeThis = thisCurrent->prev;
              Node* afterThis = thisCurrent->next;
              thisCurrent->prev = otherCurrent->prev;
              thisCurrent->next = otherCurrent->next;
              otherCurrent->prev->next = thisCurrent;
              otherCurrent->next->prev = thisCurrent;
              otherCurrent->prev = beforeThis;
              otherCurrent->next = afterThis;
              beforeThis->next = otherCurrent;
              afterThis->prev = otherCurrent;
             otherCurrent = otherCurrent->next->next;
             thisCurrent = thisCurrent->next->next;
         }
         if (thisCurrent->data.width() != otherCurrent->data.width()){
             std::cout << "Block sizes differ" << std::endl;
         }
         else{
             Node* beforeThis = thisCurrent->prev;
             Node* afterThis = thisCurrent->next;
             thisCurrent->prev = otherCurrent->prev;
             thisCurrent->next = otherCurrent->next;
             otherCurrent->prev->next = thisCurrent;
             other.tail_->prev= thisCurrent;
             otherCurrent->prev = beforeThis;
             otherCurrent->next = afterThis;
             beforeThis->next = otherCurrent;
             afterThis->prev = otherCurrent;
         }
    }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */

void Chain::clear() {
    Node* current = this->head_->next;
    for (int currentIndex = 1; currentIndex < this->size(); currentIndex++){
        Node* oldCurrent = current;
        current = current->next;
        delete oldCurrent;
    }
    this->head_->next = this->tail_;
    this->tail_->prev = this->head_;
    length_ = 0;
    height_ = 0;
    width_ = 0;
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */

void Chain::copy(Chain const& other) {
    Node* thisCurrent = new Node;
    Node* otherCurrent = other.head_;
   // thisCurrent->data = otherCurrent->data;
    thisCurrent->prev = NULL;
    this->head_ = thisCurrent;
    otherCurrent = otherCurrent->next;
    for (int i = 1; i <= other.size(); i++){
        Node* newThis = new Node(otherCurrent->data);
        newThis->prev = thisCurrent;
        thisCurrent->next = newThis;
        thisCurrent = thisCurrent->next;
        otherCurrent = otherCurrent->next;
    }
    
    this->tail_ = thisCurrent;
    //this->tail_->data = otherCurrent->data;
    thisCurrent->next = NULL;
    this->length_ = other.length_;
    this->height_ = other.height_;
    this->width_ = other.width_;
}
