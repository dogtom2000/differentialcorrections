#include "Dcp.h"
#include <iostream>
#include <iomanip>
#include <fstream>


Dcp::Dcp(Ode& ode, std::vector<double> target_position) : ode(ode), target_position(target_position)
{
	neqn = ode.neqn;
	t_initial = ode.t;
	f_final = ode.tout;
	ode.maxnum = 100000;
	
	for (size_t i = 0; i < neqn; i++)
	{
		y_initial.push_back(ode.y[i]);
	}
}


Dcp::~Dcp()
{
}

void Dcp::seek_target()
{
	while (true)
	{
		integrate();
		update_initial_conditions();
		if (count > 10 || error < 1e-8) { break; }		
		reset();
		count++;
	}
}

void Dcp::print_solution()
{
	std::cout << std::fixed;
	std::cout << std::setprecision(5);
	std::ofstream output_file;
	output_file.open("output.txt");
	ode.iflag = 1;
	ode.t = t_initial;
	for (size_t i = 0; i < ode.neqn; i++)
	{
		ode.y[i] = y_initial[i];
	}
	for (size_t i = 0; i < 1000; i++)
	{
		ode.tout = i + 1;
		ode.step();
		output_file << ode.y[0 + 0] << "," << ode.y[0 + 1] << ",";
		output_file << ode.y[6 + 0] << "," << ode.y[6 + 1];
		output_file << '\n';
	}
	output_file.close();
}

void Dcp::integrate()
{
	while (true)
	{
		ode.step();
		if (ode.iflag == 2)
		{
			return;
		}
	}
}

void Dcp::update_initial_conditions()
{
	int n = target_position.size();

	Eigen::VectorXd target(n);
	Eigen::VectorXd current(n);
	Eigen::VectorXd velocity(n);

	Eigen::VectorXd f(n);
	Eigen::MatrixXd df(n, n);
	Eigen::VectorXd x_vector(n);
	Eigen::VectorXd b_vector(n);

	target << target_position[0], target_position[1], target_position[2];
	current << ode.y[6], ode.y[7], ode.y[8];
	velocity << y_initial[9], y_initial[10], y_initial[11];
	
	f = current - target;

	error = f.norm();

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			df(i, j) = ode.y[i * 6 + j + 15];
		}
	}

	b_vector = df * velocity - f;
	x_vector = df.fullPivLu().solve(b_vector);

	y_initial[9] = x_vector(0);
	y_initial[10] = x_vector(1);
	y_initial[11] = x_vector(2);
}

void Dcp::reset()
{
	ode.iflag = 1;
	ode.t = t_initial;
	for (size_t i = 0; i < ode.neqn; i++)
	{
		ode.y[i] = y_initial[i];
	}
}
