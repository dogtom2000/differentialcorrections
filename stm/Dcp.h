#pragma once
#include <vector>
#include "Eigen/Dense"
#include "Ode.h"

class Dcp
{
public:
	// constructor and destructor
	Dcp(Ode&, std::vector<double>);
	~Dcp();

	// variables
	unsigned int neqn;
	unsigned int count{ 0 };
	double t_initial;
	double f_final;
	double error{ 1 };
	std::vector<double> y_initial;
	std::vector<double> y_final;
	std::vector<double> target_position;
	Ode ode;

	// member functions
	void seek_target();
	void integrate();
	void update_initial_conditions();
	void reset();
	void print_solution();
};

