#ifndef BIT_ARRAY_H_
#define BIT_ARRAY_H_

#include<cstdint>
using std::size_t;

#include<utility>
using std::pair;

class BitArray final{

private:

	size_t n_elements;
	size_t bit_array_size;
	uint32_t* array;

	pair<size_t,size_t> get_item_coord( size_t i) const noexcept;

public:

	BitArray( size_t _n_elements) noexcept(0);
	~BitArray() noexcept;

	bool is_in_array( size_t i) const noexcept(0);

	void add( size_t i) noexcept(0);
	void remove( size_t i) noexcept(0);

	void union_set( const BitArray* b_array) noexcept(0);
	bool has_commom_element( const BitArray* b_array) const noexcept(0);

	BitArray* get_complement( const BitArray* b_array) const noexcept(0);
	bool are_equal( const BitArray* b_array) const noexcept(0);
};

#endif /* BIT_ARRAY_H_ */
