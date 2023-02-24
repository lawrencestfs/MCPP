#include"EdgeList.h"

#include<algorithm>
//using std::sort;

struct edge_list_item_cmp final{

	bool operator()( const Edge* const a, const Edge* const b) const noexcept{

		if( a->get_cost() < b->get_cost() )
			return true;

		else if( a->get_cost() > b->get_cost() )
			return false;

		else
			return *a < *b;
	}
};


void EdgeList::sort() noexcept{

	std::sort(graph.begin(),graph.end(),edge_list_item_cmp());
}
