#ifndef SOLUTION_H_
#define SOLUTION_H_

#include"BitArray.h"
#include"Instance.h"

#include<ostream>
using std::ostream;

#include<cstdint>
using std::size_t;

#include<set>
using std::set;

class Solution final{

private:

	const Instance* const p;
	size_t hash;

	set<int> carteira;
	BitArray* bit_carteira;
	double cost;

	friend ostream& operator<<( ostream&, const Solution&) noexcept;

public:

	Solution( const Instance* _p) noexcept(0);
	~Solution() noexcept;

	double get_cost() const noexcept{ return cost;}
	void comp_cost() noexcept;

	void add_acao( int i) noexcept(0);

	const set<int>& get_carteira() const noexcept{ return carteira;}

	bool operator()( const Solution& a, const Solution& b) const noexcept{

		return a.cost < b.cost;
	}

	bool operator<( const Solution &b) const noexcept;
	bool operator>( const Solution &b) const noexcept;
	bool operator==( const Solution &b) const noexcept;
};

#endif //SOLUTION_H_
