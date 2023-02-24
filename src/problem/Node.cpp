#include"Node.h"

bool Node::operator==( const Node &e) const noexcept{

	return (i == e.id);
}


size_t Node::hash() const noexcept{

	return (i ^ id >> 1) << 1;
}


bool Node::operator<( const Node& e) const noexcept{

	return id < e.id;
}


bool Node::operator>( const Node& e) const noexcept{

	return id > e.id;
}


ostream& operator<<( ostream &strm, const Node& e) noexcept{

	return strm << '(' << e.i << ',' << e.cost << ')';
}
