#include"NodeList.h"

#include<algorithm>
//using std::sort;

struct node_list_item_cmp final{

	bool operator()( const Node* const a, const Node* const b) const noexcept{

		if( a->get_id() < b->get_id() )
			return true;

		else if( a->get_id() > b->get_id() )
			return false;

		else
			return *a < *b;
	}
};

void NodeList::sort() noexcept{

	std::sort(graph.begin(),graph.end(),node_list_item_cmp());
}
