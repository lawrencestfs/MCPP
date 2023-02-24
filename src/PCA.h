#pragma once

//Start: Global Main definitions
#define NDEBUG
//End: Global Main definitions

//Start: Global debbuging-tools
#include<cassert>
#include<cstdint>

enum ERROR_CODE : uint8_t{

	RUNTIME = 1,
	_DOMAIN = 2,
	LOGIC = 4,
	BAD_ALLOC = 16,
	UNEXPECTED = 32
};

[[noreturn]] void fail_report( const char * const description, const ERROR_CODE) noexcept;	// Defined on main.cpp

#ifndef NDEBUG

	#include<iostream>
	using std::cout;

	#include<iomanip>
	using std::setprecision;
	using std::fixed;

	#define assert_nullptr_excpt(prt,where) if( prt == nullptr ) fail_report("Nullptr exception detected in "#where,ERROR_CODE::LOGIC)
	#define runtime_assert(condition,description,error_id) if( !(condition) ) fail_report(description,error_id)

#else

	#define runtime_assert(condition,description,error_id) (void)0
	#define runtime_assert_nullptr_excpt(prt,where) (void)0

#endif
//End: Global debbuging-tools
