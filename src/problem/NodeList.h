#ifndef NODE_LIST_H_
#define NODE_LIST_H_

#include"Node.h"

#include<vector>
using std::vector;

class NodeList final{

protected:

	vector<Node*> graph;

public:

	using const_iterator = vector<Node*>::const_iterator;
	using iterator = vector<Node*>::iterator;

	NodeList() noexcept = default;
	~NodeList() noexcept = default;

	void push_back( Node* const node) noexcept(0){ graph.push_back(node);}

	// Sort by node ID
	void sort() noexcept;
	
	Node* operator[]( const size_t i) noexcept(0){ return graph[i];}

	const Node* at( const size_t i) const noexcept(0){ return graph[i];}
	Node* at( const size_t i) noexcept(0){ return graph[i];}

	size_t size() const noexcept{ return graph.size();}

	iterator begin() noexcept{ return graph.begin();}
	iterator end() noexcept{ return graph.end();}

	const_iterator cbegin() const noexcept{ return graph.cbegin();}
	const_iterator cend() const noexcept{ return graph.cend();}
};

#endif /* NODE_LIST_H_ */
