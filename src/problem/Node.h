#ifndef NODE_H_
#define NODE_H_

#include<ostream>
using std::ostream;

class Node final{

private:

	int i;
	int id;			// Node id
	double cost;	// Preço da Ação

	friend ostream& operator<<( ostream&, const Node&) noexcept;

public:

	constexpr Node( const int _i, const int _id, const double _cost) noexcept : i{_i}, id{_id}, cost{_cost} {};
	~Node() noexcept = default;

	int get_i() const noexcept{ return i;}
	int get_id() const noexcept{ return id;}
	double get_cost() const noexcept{ return cost;}


	bool operator<( const Node& i) const noexcept;
	bool operator>( const Node& i) const noexcept;
	bool operator==( const Node &i) const noexcept;
	size_t hash() const noexcept;
};

#endif /* NODE_H_ */
