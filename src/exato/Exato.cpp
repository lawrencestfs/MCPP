#ifndef EXATO_CPP
#define EXATO_CPP

#include"Exato.h"

#include<sstream>
using std::stringstream;

#include<iostream>
using std::cout;

Exato::Exato( const Instance* const _p) noexcept(0) : p{_p}{

	// Inicia o ambiente Concert
	modelo = IloModel(env);
	constraints = IloRangeArray(env);

	//######################################################################
	// criando as variáveis e adicionando ao modelo
	//######################################################################

	z = IloNumVarMatrix(env, p->get_nvertices()); // considerando a estação base
	y = IloNumVarArray(env, p->get_nvertices(),0,1,ILOBOOL);

	for( uint32_t i = 0; i < p->get_nvertices(); ++i){

		z[i] = IloNumVarArray(env,p->get_nvertices(),0,1,ILOBOOL);

		stringstream vary;
		vary << "y[" << i << ']';
		y[i].setName(vary.str().c_str());
		modelo.add(y[i]);
	}


	for( EdgeList::const_iterator it = p->begin_edges(); it != p->end_edges(); ++it){

		const Edge * edge = *it;

		stringstream varz;
		varz << "z[" << edge->get_i() << "][" << edge->get_j() << ']';
		z[edge->get_i()][edge->get_j()].setName(varz.str().c_str());
		modelo.add(z[edge->get_i()][edge->get_j()]);
	}

	IloNumVarArray ordvar(env,p->get_nvertices());
	IloNumArray ordpri(env,p->get_nvertices());

	//######################################################################
	// definindo a prioridade do branch and bound para as variaveis y
	//######################################################################

	for( uint32_t i = 0; i < p->get_nvertices(); ++i){

     	ordvar[i] = y[i];
     	ordpri[i] = 500.0;
  }

	//######################################################################
	// adicionando restrições + FO
	//######################################################################

	addFO(env,modelo,z);
	addConstraint_product(env,modelo,z,y);
	addConstraint_assignment(env,modelo,y);
	addConstraint_new_product(env,modelo,z,y);
	//addConstraint_hull(env,modelo,z,y);

	CarAcoes = IloCplex(modelo);
}


void Exato::addFO( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z) noexcept(0){

	IloExpr FO(env);

	for( EdgeList::const_iterator it = p->begin_edges(); it != p->end_edges(); ++it){

		const Edge * edge = *it;
		FO += edge->get_cost() * z[edge->get_i()][edge->get_j()];
	}

	//adicionando a fo ao modelo
	modelo.add(IloMinimize(env,FO));
}


void Exato::addConstraint_assignment( IloEnv& env, IloModel& modelo, IloNumVarArray& y) noexcept(0){

	IloExpr assign(env);

	for( uint32_t i = 0; i < p->get_nvertices(); ++i){

		assign += y[i];
	}

	assign -= p->get_K();
	IloRange constr_assignment(env,0,assign,0);
	stringstream name;
	name << "assignment: ";
	constr_assignment.setName(name.str().c_str());
	modelo.add(constr_assignment);
	constraints.add(constr_assignment);
}


void Exato::addConstraint_product( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z, IloNumVarArray& y) noexcept(0){

	for( EdgeList::const_iterator it = p->begin_edges(); it != p->end_edges(); ++it){

		const Edge * edge = *it;
		IloExpr expr0(env);
		IloExpr expr1(env);
		IloExpr expr2(env);

		expr0 = z[edge->get_i()][edge->get_j()]  - y[edge->get_i()];
		expr1 = z[edge->get_i()][edge->get_j()]  - y[edge->get_j()];
		expr2 = z[edge->get_i()][edge->get_j()]  - y[edge->get_i()] - y[edge->get_j()] + 1;

		IloRange constr_product_0(env, -IloInfinity, expr0, 0);
		IloRange constr_product_1(env, -IloInfinity, expr1, 0);
		IloRange constr_product_2(env, 0, expr2, IloInfinity);

		stringstream name0, name1, name2;

		name0 << "product_0[" << edge->get_i() << ',' << edge->get_j() << "]: ";
		constr_product_0.setName(name0.str().c_str());
		modelo.add(constr_product_0);
		constraints.add(constr_product_0);

		name1 << "product_1[" << edge->get_i() << ',' << edge->get_j() << "]: ";
		constr_product_1.setName(name1.str().c_str());
		modelo.add(constr_product_1);
		constraints.add(constr_product_1);

		name2 << "product_2[" << edge->get_i() << ',' << edge->get_j() << "]: ";
		constr_product_2.setName(name2.str().c_str());
		modelo.add(constr_product_2);
		constraints.add(constr_product_2);
	}
}


void Exato::addConstraint_knapsack( IloEnv& env, IloModel& modelo, IloNumVarArray& y) noexcept(0){

	IloExpr knapsack(env);

	for( uint32_t i = 0; i < p->get_nvertices(); ++i){

		knapsack += y[i];
	}

	knapsack -= p->get_K();
	IloRange constr_knapsack(env, 0, knapsack, 0);
	stringstream name;
	name << "Knapsack: ";
	constr_knapsack.setName(name.str().c_str());
	modelo.add(constr_knapsack);
	constraints.add(constr_knapsack);
}


void Exato::addConstraint_new_product( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z, IloNumVarArray& y) noexcept(0){

	for( uint32_t i = 0; i < p->get_nvertices(); ++i){

		IloExpr Sum(env);

		for( EdgeList::const_iterator it = p->begin_edges_neighbor(i); it != p->end_edges_neighbor(i); ++it){

			const Edge *edge = *it;

			//cout<<edge->get_i()<<" "<<edge->get_j()<<endl;
			if( edge->get_i() < edge->get_j() ){

				Sum+=z[edge->get_i()][edge->get_j()];
				//cout<<"z["<<edge->get_i()<<","<<edge->get_j()<<"] ";

			}else{

				Sum+=z[edge->get_j()][edge->get_i()];
				//cout<<"z["<<edge->get_j()<<","<<edge->get_i()<<"] ";
			}
		}

		Sum -= (p->get_K()-1)*y[i];
		IloRange constr_new_product(env,0,Sum,0);
		stringstream name;
		name << "new_product[" << i << "]: ";
		constr_new_product.setName(name.str().c_str());
		modelo.add(constr_new_product);
		constraints.add(constr_new_product);
	}
}


void Exato::addConstraint_hull( IloEnv& env, IloModel& modelo, IloNumVarMatrix& z, IloNumVarArray& y) noexcept(0){

	for( uint32_t i = 0; i < p->get_nvertices()-2; ++i){

		for( uint32_t j = i+1; j < p->get_nvertices()-1; ++j){

			for( uint32_t k = j+1; k < p->get_nvertices(); ++k){

				IloExpr hull(env);
				hull = y[i]+y[j]+y[k] - 1 -z[i][j] -z[j][k] -z[i][k];

				IloRange constr_hull(env,-IloInfinity,hull,0);
				stringstream name;
				name << "new_product[" << i << ',' << j << ',' << k << "]: ";
				constr_hull.setName(name.str().c_str());
				modelo.add(constr_hull);
				constraints.add(constr_hull);
			}
		}
	}
}


uint32_t Exato::getNumConstraints( IloModel m) noexcept{

  IloModel::Iterator iter(m);
  uint32_t count = 0;

	while( iter.ok() ){

		if( (*iter).asConstraint().getImpl() )
			++count;

		++iter;
	}

return count;}


pair<double,int> Exato::branch_and_bound( int TimeLimit) noexcept(0){

	CarAcoes.exportModel("lp-linear.lp");

	// CarAcoes.setPriorities (ordvar, ordpri);
	CarAcoes.setParam(IloCplex::TiLim, TimeLimit);

	// Avoids verbose output
	CarAcoes.setOut(env.getNullStream());

	//Resolver direto pelo Concert
	//CarAcoes.setParam(IloCplex::PreLinear, 0);
	//CarAcoes.setParam(IloCplex::AdvInd, 1); //AdvInd = 1 ou 2
/*
	CarAcoes.setParam(IloCplex::PreLinear, 0);
	CarAcoes.setParam(IloCplex::Param::Preprocessing::Aggregator, 0);
	CarAcoes.setParam(IloCplex::Param::Preprocessing::Presolve, 0);

	CarAcoes.setParam(IloCplex::AdvInd, 0); //AdvInd = 1 ou 2

	CarAcoes.setParam(IloCplex::Threads, 1);

	CarAcoes.setParam(IloCplex::PreInd, IloFalse);


	CarAcoes.setParam(IloCplex::HeurFreq, -1);
	CarAcoes.setParam(IloCplex::Cliques, -1);
	CarAcoes.setParam(IloCplex::Covers, -1);
	CarAcoes.setParam(IloCplex::DisjCuts, -1);
	CarAcoes.setParam(IloCplex::FlowCovers, -1);
	CarAcoes.setParam(IloCplex::FlowPaths, -1);
	CarAcoes.setParam(IloCplex::FracCuts, -1);
	CarAcoes.setParam(IloCplex::GUBCovers, -1);
	CarAcoes.setParam(IloCplex::ImplBd, -1);
	CarAcoes.setParam(IloCplex::MIRCuts, -1);
	CarAcoes.setParam(IloCplex::ZeroHalfCuts, -1);
	CarAcoes.setParam(IloCplex::MCFCuts, -1);
	CarAcoes.setParam(IloCplex::Param::MIP::Cuts::LiftProj, -1);
/*
//#ifdef PRINT
	CarAcoes.setParam(IloCplex::NetDisplay, 0);
	CarAcoes.setParam(IloCplex::SiftDisplay, 0);
	CarAcoes.setParam(IloCplex::SimDisplay, 0);
	CarAcoes.setParam(IloCplex::BarDisplay, 0);
	CarAcoes.setParam(IloCplex::MIPDisplay, 0);
//#endif*/

	CarAcoes.solve();

// DEBUG

/*
	PrintSol();
	Solution* sol = new Solution (p);
	for(int i = 0; i<p->get_nvertices(); i++){
		if(CarAcoes.getValue(y[i])>1E-5){
			sol->add_acao(i);
		}
	}
	sol->comp_cost();

	if(getStatus()==0){
		cout<<"Optimum Found"<<endl;
		cout<<"BestSol: "<<CarAcoes.getObjValue()<<endl;
		cout<<"GAP: "<<CarAcoes.getMIPRelativeGap()<<endl;
	}else if(getStatus() == 1){
		cout<<"Solution Found"<<endl;
		cout<<"BestSol: "<<CarAcoes.getObjValue()<<endl;
		cout<<"GAP: "<<CarAcoes.getMIPRelativeGap()<<endl;
	}else if(getStatus() == 2){
		cout<<"Infeasible"<<endl;
	}else{
		cout<<"Unknown"<<endl;
	}
*/

	//chkConflicts(CarAcoes);
	//crono.stop();

	return {CarAcoes.getObjValue(),getStatus()};
}


status Exato::getStatus() noexcept{

	if ( CarAcoes.getStatus() == IloAlgorithm::Infeasible )
		return INFEASIBLE;

	else if( CarAcoes.getStatus() == IloAlgorithm::Optimal )
		return OPTIMALFOUND;

	else
		return SOLUTIONFOUND;
}


void Exato::PrintSol() noexcept{

	for( uint32_t i = 0; i < p->get_nvertices(); ++i)
		if( CarAcoes.getValue(y[i]) > 1E-5 )
			cout << "y[" << i << "] = " << CarAcoes.getValue(y[i]) << '\n';
}


void Exato::set_primal_bound( const double value) noexcept{

	CarAcoes.setParam(IloCplex::CutUp,value);
}


void Exato::set_dual_bound( const double value) noexcept{

	CarAcoes.setParam(IloCplex::CutLo,value);
}


void Exato::set_MIP_start( Solution* s) noexcept{

	const set<int>& acoes = s->get_carteira();
	IloNumVarArray startVar(getEnv());
	IloNumArray startVal(getEnv());

	for( uint32_t i = 0; i < p->get_nvertices(); ++i){

		bool flag = 0;

		for( set<int>::const_iterator it = acoes.cbegin(); it != acoes.cend(); ++it){

			if( i == *it ){

				startVar.add(y[i]);
				startVal.add(1);
				flag = 1;
				break;
			}
		}

		if( flag == 0 ){

			startVar.add(y[i]);
			startVal.add(0);
		}
	}

	CarAcoes.addMIPStart(startVar,startVal,IloCplex::MIPStartRepair);
	CarAcoes.writeMIPStarts("mipstart.mst");
}

Exato::~Exato() noexcept{

//	cout << "Deleting Exato ... ";

	CarAcoes.end();
	env.end();

//	cout << "done." <<endl;
}

#endif /*EXATO_CPP*/
