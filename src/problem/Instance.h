#ifndef INSTANCE_H
#define INSTANCE_H

#include"EdgeList.h"
#include"NodeList.h"
#include"Node.h"
#include"Edge.h"

#include<utility>
using std::pair;

#include<map>
using std::map;

class Instance final{

private:

	int nvertex;
	int nedges;
	int K;

	map<pair<int,int>,int> ij_edge;

	NodeList nodes;
	EdgeList edges;

	EdgeList* edges_neighbor;

public:

	Instance( const char* const file_name, double percent) noexcept;
	~Instance() noexcept;

	int get_nvertices() const noexcept{ return nvertex;}
	int get_nedges() const noexcept{ return nedges;}
	int get_K() const noexcept{ return K;}

	double get_corr( int i, int j) const noexcept(0);

	EdgeList::const_iterator begin_edges() const noexcept{ return edges.cbegin();}
	EdgeList::const_iterator end_edges() const noexcept{ return edges.cend();}

	NodeList::const_iterator begin_nodes() const noexcept{ return nodes.cbegin();}
	NodeList::const_iterator end_nodes() const noexcept{ return nodes.cend();}

	EdgeList::const_iterator begin_edges_neighbor( const int i) const noexcept(0){ return edges_neighbor[i].cbegin();}
	EdgeList::const_iterator end_edges_neighbor( const int i) const noexcept(0){ return edges_neighbor[i].cend();}

	size_t get_size_neighbor( const int i) const noexcept(0){ return edges_neighbor[i].size();}
	void PrintInstance() const noexcept;
};

#endif
