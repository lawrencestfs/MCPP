#ifndef EDGE_LIST_H_
#define EDGE_LIST_H_

#include"Edge.h"

#include<vector>
using std::vector;

class EdgeList final{

protected:

	vector<Edge*> graph;

public:

	using const_iterator = vector<Edge*>::const_iterator;
	using iterator = vector<Edge*>::iterator;

	EdgeList() noexcept = default;
	~EdgeList() noexcept = default;

	void push_back( Edge* const edge) noexcept(0){

		graph.push_back(edge);
	}

	// Sort by edge cost
	void sort() noexcept;

	Edge* operator[]( const size_t i) noexcept(0){ return graph[i];}

	const Edge* at( const size_t i) const noexcept(0){ return graph[i];}
	Edge* at( const size_t i) noexcept(0){ return graph[i];}

	size_t size() const noexcept{ return graph.size();}

	iterator begin() noexcept{ return graph.begin();}
	iterator end() noexcept{ return graph.end();}

	const_iterator cbegin() const noexcept{ return graph.cbegin();}
	const_iterator cend() const noexcept{ return graph.cend();}
};

#endif /* EDGE_LIST_H_ */
