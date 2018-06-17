//const int neqn = 42;
//double work[neqn * 22];

//double t = 0;
//double tout = 2.0 * PI;
//double relerr = 0;
//double abserr = 2e-8;

//double y[neqn]{	0,			0, 0, 0, 0,			0,
//				0.723,		0, 0, 0, 1.1760637,	0,
//				1,			0, 0, 0, 1,			0,
//				1.00257,	0, 0, 0, 1,			0,
//				1.524,		0, 0, 0, 0.810042,	0,
//				5.203,		0, 0, 0, 0.4384026,	0,
//				9.537,		0, 0, 0, 0.3238129,	0 };

//const int neqn = 48;
//double t = 0;
//double tout = 1e-3;
//double relerr = 0;
//double abserr = 2e-8;
//
//double y_reference[neqn]
//{
//	1, 0, 0, 0, 1, 0,
//	1.0046594693028, 0, 0, 0, 2.61517979500688, 0,
//	1, 0, 0, 0, 0, 0,
//	0, 1, 0, 0, 0, 0,
//	0, 0, 1, 0, 0, 0,
//	0, 0, 0, 1, 0, 0,
//	0, 0, 0, 0, 1, 0,
//	0, 0, 0, 0, 0, 1
//};
//
//double work_reference[neqn * 22];
//
//Ode reference(diffeq::stm3b, neqn, y_reference, t, tout, relerr, abserr, work_reference);
//reference.maxnum = 100000;
//reference.step();
//
//double y_actual[neqn]
//{
//	1, 0, 0, 0, 1, 0,
//	1.0046594693028, 0, 0, 0, 2.61517979500688, 0,
//	1, 0, 0, 0, 0, 0,
//	0, 1, 0, 0, 0, 0,
//	0, 0, 1, 0, 0, 0,
//	0, 0, 0, 1, 0, 0,
//	0, 0, 0, 0, 1, 0,
//	0, 0, 0, 0, 0, 1
//};
//double delta[6]{ 0.0001, -0.002, 0.001, -0.01, 0.1, -0.1 };
//
//for (int i = 0; i < 6; i++)
//{
//	y_actual[i + 6] += delta[i];
//}
//
//Ode actual(diffeq::stm3b, neqn, y_actual, t, tout, relerr, abserr, work_reference);
//actual.maxnum = 100000;
//actual.step();
//
//
//std::cout << std::fixed;
//std::cout << std::setprecision(5);
//for (int i = 0; i < 2; i++)
//{
//	for (int j = 0; j < 6; j++)
//	{
//		std::cout << std::setw(8) << std::setfill(' ') << reference.y[6 * i + j] << " ";
//	}
//	std::cout << '\n';
//}
//
//std::cout << reference.iflag;
//std::cout << '\n';
//std::cout << reference.nostep;
//std::cout << '\n';
//std::cout << '\n';
//
//
//for (int i = 0; i < 2; i++)
//{
//	for (int j = 0; j < 6; j++)
//	{
//		std::cout << std::setw(8) << std::setfill(' ') << actual.y[6 * i + j] << " ";
//	}
//	std::cout << '\n';
//}
//
//std::cout << actual.iflag;
//std::cout << '\n';
//std::cout << actual.nostep;
//std::cout << '\n';
//std::cout << '\n';
//
//double y_predicted[6];
//double deltay_predicted[6]{ 0 };
//
//
//for (size_t i = 0; i < 6; i++)
//{
//	deltay_predicted[i] = reference.y[i + 6];
//	for (size_t j = 0; j < 6; j++)
//	{
//		deltay_predicted[i] += reference.y[6 * i + j + 12] * delta[j];
//	}
//	std::cout << deltay_predicted[i] << ' ';
//}