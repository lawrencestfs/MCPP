#include"Edge.h"

bool Edge::operator==( const Edge &e) const noexcept{

	return ( i == e.i && j == e.j ) || ( i == e.j && j == e.i );
}


size_t Edge::hash() const noexcept{

	return (i ^ j >> 1) << 1;
}


bool Edge::operator<( const Edge& e) const noexcept{

	if( i < e.i )
		return true;

	else if( i > e.i )
		return false;

	else
		return j < e.j;
}


bool Edge::operator>( const Edge& e) const noexcept{

	if( i > e.i )
		return true;

	else if( i < e.i )
		return false;

	else
		return j > e.j;
}

ostream& operator<<( ostream &strm, const Edge& e) noexcept{

	return strm << '(' << e.i << ',' << e.j << ") - " << e.cost << '\n';
}
