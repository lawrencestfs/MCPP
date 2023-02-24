#ifndef EXATO_H
#define EXATO_H

enum status{ OPTIMALFOUND = 0, SOLUTIONFOUND = 1, INFEASIBLE = 2};

#include"../problem/Solution.h"
#include"../problem/Instance.h"

#include<ilcplex/ilocplex.h>

#include<utility>
using std::pair;

//ILOSTLBEGIN

#define SMALL_NUMBER 0.000001

using IloNumVarMatrix = IloArray<IloNumVarArray>;
using IloNumVar3Matrix = IloArray<IloNumVarMatrix>;
using IloNumVar4Matrix = IloArray<IloNumVar3Matrix>;

using IloNumMatrix = IloArray<IloNumArray>;
using IloNum3Matrix = IloArray<IloNumMatrix>;

class Exato final{

private:

	const Instance* const p;

	IloConstraintArray constraints;

	IloEnv env;
	IloModel modelo;
	IloCplex CarAcoes;

	IloNumVarArray y;
	IloNumVarMatrix z;

	void addFO( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z) noexcept(0);
	void addConstraint_assignment( IloEnv& env, IloModel& modelo, IloNumVarArray& y) noexcept(0);
	void addConstraint_product( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z, IloNumVarArray& y) noexcept(0);
	void addConstraint_knapsack( IloEnv& env, IloModel& modelo, IloNumVarArray& y) noexcept(0);
	void addConstraint_new_product( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z, IloNumVarArray& y) noexcept(0);
	void addConstraint_hull( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z, IloNumVarArray& y) noexcept(0);

	uint32_t getNumConstraints( IloModel m) noexcept;

public:

	Exato( const Instance* _p) noexcept(0);
	~Exato() noexcept;

	pair<double,int> branch_and_bound( int TimeLimit) noexcept(0);

	const IloEnv& getEnv() noexcept{ return env;}
	status getStatus() noexcept;

	void PrintSol() noexcept;

	void set_primal_bound( double value) noexcept;
	void set_dual_bound( double value) noexcept;
	void set_MIP_start( Solution* s) noexcept;
};

#endif /*EXATO_H*/
