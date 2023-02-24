#include"Instance.h"

#include"../FPcomparator.h"
#include"../PCA.h"

#include<stdexcept>
using std::runtime_error;
using std::domain_error;
using std::logic_error;
using std::bad_alloc;

#include<fstream>
using std::ifstream;

#include<iostream>
using std::cout;

Instance::Instance( const char* const file_name, double percent) noexcept try{

	if( percent <= FPcomparator<double>(0.0) || percent >= FPcomparator<double>(1.0) )
		throw domain_error("K percentage is not in ]0,1[");

	ifstream f_inst(file_name);

	if( !f_inst.is_open() )
		throw runtime_error("The file passed could not be open correctly.");

	f_inst >> nvertex;
	nedges =  nvertex*(nvertex-1)/2;

	K = int(nvertex*percent);

	if( nvertex == 0 )
		throw domain_error("N == 0");

	if( K == nvertex )
		throw domain_error("K == N");

	if( K == 0 )
		throw domain_error("K == 0");

	int i,j;
	double cost;

	edges_neighbor = new EdgeList[nvertex];

	for( int id = 0; id < nedges; ++id){

		f_inst >> i;
		f_inst >> j;
		f_inst >> cost;

		Edge* edge = new Edge(i,j,id,cost);
		ij_edge[{i,j}] = id;

		edges.push_back(edge);

		edges_neighbor[edge->get_i()].push_back(edge);
		edges_neighbor[edge->get_j()].push_back(edge);
	}

	if( !f_inst.eof() && ( f_inst.get() > 32 || f_inst.get() > 32 || f_inst.get()> 32 || !f_inst.eof() ) )
		throw runtime_error("The Instance file is not correctly formatted");

	f_inst.close();

	//PrintInstance();

}catch( domain_error& d){

	fail_report(d.what(),ERROR_CODE::_DOMAIN);

}catch( runtime_error& r){

	fail_report(r.what(),ERROR_CODE::RUNTIME);

}catch( bad_alloc& b){

	fail_report(b.what(),ERROR_CODE::BAD_ALLOC);

}catch(...){

	fail_report("Unexpected exception occurred while the Instance were being created.",ERROR_CODE::UNEXPECTED);
}


double Instance::get_corr( const int i, const int j) const noexcept(0){

	return edges.at(ij_edge.at({i,j}))->get_cost();
}


void Instance::PrintInstance() const noexcept{

	cout << nvertex << '\n';

	for( auto it = edges.cbegin(); it != edges.cend(); ++it){

		const Edge* edge = *it;
		cout << edge->get_i() << ' ' << edge->get_j() << ' ' << edge->get_cost() << '\n';
	}
}


Instance::~Instance() noexcept{

//	cout << "Deleting Instance ... ";

	for( auto edge : edges )
		delete edge;

	delete[] edges_neighbor;

//	cout << "done"<<endl;
}
