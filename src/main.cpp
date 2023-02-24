#include"problem/Instance.h"
#include"exato/Exato.h"
#include"PCA.h"

#include<chrono>
using ElapsedSeconds = std::chrono::duration<double>;
using std::chrono::system_clock;
using std::chrono::time_point;

#include<iostream>
using std::cout;
using std::cerr;

#include<stdlib.h>
using std::exit;

[[noreturn]] void usage( const char * const argv_0){

	cout<< "Usage:\n"
		<< '\t' << argv_0 << " <input_instance_name> <K> <time_limit>\n\n"
		<< "\t<input_instance_name>: file that contains a Minimum Correlated Portifolio Problem valid instance\n\n"
		<< "\t<K>: a real number in ]0,1[ that represents the number of stocks to be chosen from the total\n\n"
		<< "\t<time_limit>: Maximum time in seconds to run the main algorithm.\n\n";

exit(2);}


[[noreturn]] void fail_report( const char * const description, const ERROR_CODE error) noexcept{

	cerr << "\n\n################################\nA";

	if( error == ERROR_CODE::BAD_ALLOC )
		cerr << " Bad Alloc";

	else if( error == ERROR_CODE::RUNTIME )
		cerr << " Runtime";

	else if( error == ERROR_CODE::_DOMAIN )
		cerr << " Domain";

	else if( error == ERROR_CODE::LOGIC )
		cerr << " Logic";

	else
		cerr << "n Unexpected";

	cerr << " Exception ocurred during the execution";

	if( error == ERROR_CODE::BAD_ALLOC )
		cerr << " in: " << description;
	else
		cerr << ", description: " << description;

	cerr << "\nProgram is shuting down...\n################################\n\n";

exit(error);}


void relatorio( const double value, const double time, const int status){

	cout << "Approach:\t" << value << '\t' << time << '\t' << status << '\n';
}

int main( const int argc, const char* const* const argv){

	if( argc != 4 )
		usage(argv[0]);

	// Start the Instance
	Instance instance(argv[1],atof(argv[2]));

	// Start the Exact Model
	Exato modelo(&instance);

/*
	{
		ILS ils_mh(&instance);

		Solution* s_best = ils_mh.solve(atoi(argv[3]),0,0,0);		// Os zeros são valores que ainda não são usados no ILS, mas que serão necessários na busca local e no shaking.

		modelo.set_MIP_start(s_best);
		delete s_best;
	}
*/

	// Set the model dual bound
	modelo.set_dual_bound(17.0494);

	// Starts counting time after instance reading
	const time_point<system_clock> START = system_clock::now();

	// Run the main algorithm
	pair<double,int> s_best = modelo.branch_and_bound(atoi(argv[3]));

	// Show the execution result
	relatorio(s_best.first,static_cast<float>(size_t(1000000*ElapsedSeconds(system_clock::now()-START).count()))/1000000,s_best.second);

return 0;}
