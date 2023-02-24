#ifndef EDGE_H_
#define EDGE_H_

#include<ostream>
using std::ostream;

class Edge final{

private:

	int i,j;		// Edge id
	int id;
	double cost;	// The cost for choose this edge to be in the solution

	friend ostream& operator<<( ostream&, const Edge&) noexcept;

public:

	constexpr Edge( const int _i, const int _j, const int _id, const double _cost) noexcept : i{_i}, j{_j}, id{_id}, cost{_cost} {};
	~Edge() noexcept = default;

	int get_i() const noexcept{ return i;}
	int get_j() const noexcept{ return j;}
	int get_id() const noexcept{ return id;}
	double get_cost() const noexcept{ return cost;}

	bool operator<( const Edge& e) const noexcept;
	bool operator>( const Edge& e) const noexcept;
	bool operator==( const Edge &e) const noexcept;
	size_t hash() const noexcept;
};

#endif /* EDGE_H_ */
