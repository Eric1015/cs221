#include "stack.h"

Point Stack::peek() {
	if (isEmpty())
		return Point(0, 0);
	else
		return st[st.size() - 1];
}

int Stack::size() {
	return st.size();
}

bool Stack::isEmpty() {
	return (st.size() == 0 ? true : false);
}

void Stack::push(Point p) {
	st.push_back(p);
}

Point Stack::pop() {
	if (isEmpty())
		return Point(0, 0);
	else
	{
		Point back = st.back();
		st.pop_back();
		return back;
	}
}
