#include"Solution.h"

#include<iterator>
using std::next;

Solution::Solution( const Instance* const _p) noexcept(0) : p{_p}{

	bit_carteira = new BitArray(p->get_nvertices());
	cost = 10000000;
	hash = 0;
}


void Solution::comp_cost() noexcept{

	double w = 0.0;

	for( auto it = carteira.cbegin(); it != carteira.cend(); ++it){

		auto it2 = next(it);

		while( it2 != carteira.cend() ){

			if( *it < *it2 )
				w += p->get_corr(*it,*it2);

			++it2;
		}
	}

	cost = w;
}


void Solution::add_acao( const int i) noexcept(0){

	carteira.insert(i);
	bit_carteira->add(i);
	hash += (i+1)*(i+2);
}


bool Solution::operator<( const Solution &b) const noexcept{

	//cout<<"Testa Menor"<<endl;

	if( this->carteira.size() < b.carteira.size() )
		return true;

	if( this->carteira.size() > b.carteira.size() )
		return false;

	return this->hash < b.hash;
}


bool Solution::operator>( const Solution &b) const noexcept{

	//cout<<"Testa Maior"<<endl;

	if( this->carteira.size() > b.carteira.size() )
		return true;

	if( this->carteira.size() < b.carteira.size() )
		return false;

	return this->hash > b.hash;
}


bool Solution::operator==( const Solution &b) const noexcept{

	if( this->cost != b.cost )
		return false;

	if( this->hash != b.hash )
		return false;

	return this->bit_carteira->are_equal(b.bit_carteira);
}


ostream& operator<<( ostream& strm, const Solution& s) noexcept{

	for( auto it : s.carteira )
		strm << '(' << it << ") ";

	return strm;
}


Solution::~Solution() noexcept{

	if( bit_carteira != nullptr )
		delete bit_carteira;
}
