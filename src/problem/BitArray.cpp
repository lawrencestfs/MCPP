#include"BitArray.h"

#include<utility>
using std::make_pair;

#include<cmath>
using std::floor;
using std::ceil;

BitArray::BitArray( const size_t _n_elements) noexcept(0) : n_elements(_n_elements){

	bit_array_size = ceil(double(n_elements)/(sizeof(uint32_t)*8.0));
	array = new uint32_t[bit_array_size];

	for( size_t t = 0; t != bit_array_size; ++t)
		array[t] = 0;
}


pair<size_t,size_t> BitArray::get_item_coord( const size_t i) const noexcept{

	const size_t pos = floor(double(i)/(sizeof(uint32_t)*8.0));
	const size_t bit = i-pos*sizeof(uint32_t)*8;
	return make_pair(pos,bit);
}


bool BitArray::is_in_array( const size_t i) const noexcept(0){

	const auto c = get_item_coord(i);
	return array[c.first] & (1<<c.second);
}


void BitArray::add( const size_t i) noexcept(0){

	const auto c = get_item_coord(i);
	array[c.first] = array[c.first] | (1<<c.second);
}


void BitArray::remove( const size_t i) noexcept(0){

	const auto c = get_item_coord(i);
	array[c.first] = array[c.first] & (~(1<<c.second));
}


void BitArray::union_set( const BitArray* const b_array) noexcept(0){

	for( size_t t = 0; t != bit_array_size; ++t)
		this->array[t] = this->array[t] | b_array->array[t];
}


bool BitArray::has_commom_element( const BitArray* const b_array) const noexcept(0){

	uint32_t has = 0;

	for( size_t t = 0; t != bit_array_size && has == 0; ++t)
		has = this->array[t] & b_array->array[t];

	return has > 0;
}


BitArray* BitArray::get_complement( const BitArray* const b_array) const noexcept(0){

	BitArray* const cmp = new BitArray(n_elements);

	for( size_t t = 0; t != bit_array_size; ++t)
		cmp->array[t] = ~this->array[t];
}


bool BitArray::are_equal( const BitArray* const b_array) const noexcept(0){

	uint32_t has = 0;

	for( size_t t = 0; t != bit_array_size; ++t){

		if( this->array[t] == b_array->array[t] )
			++has;
		else
			break;
	}

	return has == bit_array_size;
}


BitArray::~BitArray() noexcept{

	if( array != nullptr )
		delete[] array;
}
