#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "Ode.h"
#include "Diffeq.h"
#include "Dcp.h"

#define PI 3.14159265358979323846264338328

void calculate_target(double &x, double&y, double phi)
{
	//double moonx = 53.40297;
	//double moony = 27.93549;
	double moonx = 50.2257;
	double moony = 33.3113;
	double moonr = 60.26830685;
	double soi = 10.4;

	double temp1 = moonx * std::cos(phi) - moony * std::sin(phi);
	double temp2 = moony * std::cos(phi) + moonx * std::sin(phi);

	x = soi * temp1 / moonr + moonx;
	y = soi * temp2 / moonr + moony;
}

int main()
{
	// time unit: 801.893982496276 seconds
	// distance unit: 6378145 meters

	double xcoord;
	double ycoord;

	calculate_target(xcoord, ycoord, 1.965 + 0.29);

	const int neqn = 48;
	double t = 0;
	double tout = 274;
	double relerr = 0;
	double abserr = 1e-8;
	double work_reference[neqn * 22];

	Eigen::VectorXd state_vector(6);
	// state_vector << -0.75365, -0.73110, 0, 0.983333693745816, -0.953915534386672, 0;
	state_vector << -0.75365, -0.73110, 0, 0.952833, -0.975873, 0;

	double y[neqn]
	{
		60.26830685, 0, 0, 0, 0.128811757, 0,
		state_vector(0), state_vector(1), state_vector(2), state_vector(3), state_vector(4), state_vector(5),
		1, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0,
		0, 0, 1, 0, 0, 0,
		0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1
	};

	Ode moon_trajectory(diffeq::stm3b, neqn, y, t, tout, relerr, abserr, work_reference);

	std::vector<double> target_position = { xcoord, ycoord, 0};

	Dcp dcp(moon_trajectory, target_position);
	dcp.seek_target();
	dcp.print_solution();

	//Eigen::VectorXd target_position(3);
	//target_position << 43.01, 28.37, 0;

	//Eigen::VectorXd current_position(3);
	//current_position << moon_trajectory.y[6], moon_trajectory.y[7], moon_trajectory.y[8];

	//Eigen::VectorXd f(3);
	//f = current_position - target_position;

	//Eigen::MatrixXd df(3, 3);

	//Eigen::VectorXd current_velocity(3);
	//current_velocity << state_vector(3), state_vector(4), state_vector(5);

	//for (size_t i = 0; i < 3; i++)
	//{
	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		df(i, j) = moon_trajectory.y[i * 6 + j + 15];
	//	}
	//}

	//Eigen::VectorXd b_vector(3);
	//Eigen::VectorXd x_vector(3);
	//b_vector = df * current_velocity - f;

	//x_vector = df.fullPivLu().solve(b_vector);









































	//std::cout << std::fixed;
	//std::cout << std::setprecision(5);
	////std::cout << moon_trajectory.iflag;
	////std::cout << '\n';
	////std::cout << moon_trajectory.nostep;
	////std::cout << '\n';
	////std::cout << '\n';


	//for (size_t run = 0; run < 61; run++)
	//{
	//	


	//	for (int i = 0; i < 2; i++)
	//	{
	//		for (int j = 0; j < 6; j++)
	//		{
	//			std::cout << std::setw(8) << std::setfill(' ') << moon_trajectory.y[6 * i + j] << " ";
	//		}
	//		std::cout << '\n';
	//	}
	//	std::cout << '\n';
	//	moon_trajectory.tout += 225.5 / 30;
	//	moon_trajectory.step();
	//}
	return 0;
}
