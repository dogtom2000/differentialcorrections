#include <cmath>
#include <Eigen/Dense>
#include <iostream>
#include <iomanip>

#define rx(i, j) rx[i + j * n]
#define ry(i, j) ry[i + j * n]
#define rz(i, j) rz[i + j * n]
#define rr(i, j) rr[i + j * n]
#define rs(i, j) rs[i + j * n]
#define g(i, j) g[i + j * 3]

namespace diffeq {

	void two_body(double x, double* y, double* yp)
	{
		double temp = y[0] * y[0] + y[1] * y[1];
		double mu = 1;
		temp *= std::sqrt(temp);
		yp[0] = y[2];
		yp[1] = y[3];
		yp[2] = -mu / temp * y[0];
		yp[3] = -mu / temp * y[1];
	}

	void seven_body(double x, double* y, double* yp)
	{
		unsigned int nbody = 7;
		double mu[7] = { 1.0, 0.00000244784, 0.000003003489663, 0.000000036958626, 0.0000003227155, 0.000954594263, 0.00028581485 };
		for (size_t i = 0; i < nbody; i++)
		{
			int ii = 6 * i;
			for (size_t j = 0; j < 3; j++)
			{
				yp[ii + j] = y[ii + j + 3];
			}

			yp[ii + 3] = 0.0;
			yp[ii + 4] = 0.0;
			yp[ii + 5] = 0.0;

			for (size_t j = 0; j < nbody; j++)
			{
				if (i != j)
				{
					int jj = 6 * j;
					double rx = y[ii + 0] - y[jj + 0];
					double ry = y[ii + 1] - y[jj + 1];
					double rz = y[ii + 2] - y[jj + 2];
					double r = std::sqrt(rx * rx + ry * ry + rz * rz);

					yp[ii + 3] -= mu[j] * rx / std::pow(r, 3);
					yp[ii + 4] -= mu[j] * ry / std::pow(r, 3);
					yp[ii + 5] -= mu[j] * rz / std::pow(r, 3);
				}
			}
			if (i != 0)
			{
				yp[ii + 3] -= yp[3];
				yp[ii + 4] -= yp[4];
				yp[ii + 5] -= yp[5];
			}
		}
		yp[3] = 0.0;
		yp[4] = 0.0;
		yp[5] = 0.0;
	}

	void stm3b(double x, double*y, double* yp)
	{
		const unsigned int n{ 3 };
		const unsigned int k{ n - 1 };
		const unsigned int m{ 6 };
		const double mu[n]{ 0.012155650403207, 0, 0.987844349596793 };

		double rr[n * n]{ 0 };
		double rs[n * n]{ 0 };
		double rx[n * n]{ 0 };
		double ry[n * n]{ 0 };
		double rz[n * n]{ 0 };

		for (size_t i = 0; i < n; i++)
		{
			int ii = m * i;
			for (size_t j = 0; j < n; j++)
			{
				int jj = m * j;
				if (j > i)
				{
					if (j != 2)
					{
						rx(i, j) = y[jj + 0] - y[ii + 0];
						ry(i, j) = y[jj + 1] - y[ii + 1];
						rz(i, j) = y[jj + 2] - y[ii + 2];
					}
					else
					{
						rx(i, j) = -y[ii + 0];
						ry(i, j) = -y[ii + 1];
						rz(i, j) = -y[ii + 2];
					}
					rs(i, j) = rx(i, j) * rx(i, j) + ry(i, j) * ry(i, j) + rz(i, j) * rz(i, j);
					rr(i, j) = std::sqrt(rs(i, j));
				}
				else if (j < i)
				{
					rx(i, j) = -rx(j, i);
					ry(i, j) = -ry(j, i);
					rz(i, j) = -rz(j, i);
					rs(i, j) = rs(j, i);
					rr(i, j) = rr(j, i);
				}
			}
		}

		for (size_t i = 0; i < n - 1; i++)
		{
			int ii = m * i;

			yp[ii + 0] = y[ii + 3];
			yp[ii + 1] = y[ii + 4];
			yp[ii + 2] = y[ii + 5];

			double temp1 = -(mu[i] + mu[k]) / (rs(k, i) * rr(k, i));

			yp[ii + 3] = temp1 * rx(k, i);
			yp[ii + 4] = temp1 * ry(k, i);
			yp[ii + 5] = temp1 * rz(k, i);

			for (size_t j = 0; j < n; j++)
			{
				if (j != i && j != k)
				{
					int jj = m * j;
					double temp2 = (rs(i, j) * rr(i, j));
					double temp3 = (rs(k, j) * rr(k, j));

					yp[ii + 3] += mu[j] * (rx(i, j) / temp2 - rx(k, j) / temp3);
					yp[ii + 4] += mu[j] * (ry(i, j) / temp2 - ry(k, j) / temp3);
					yp[ii + 5] += mu[j] * (rz(i, j) / temp2 - rz(k, j) / temp3);
				}
			}
		}

		int i = 1;
		int j = 0;

		double temp4 = mu[i] + mu[k];
		double temp5 = mu[j];
		double rk3 = rr(k, i) * rs(k, i);
		double rk5 = rk3 * rs(k, i);

		double rj3 = rr(i, j) * rs(i, j);
		double rj5 = rj3 * rs(i, j);

		double rxki = rx(k, i);
		double ryki = ry(k, i);
		double rzki = rz(k, i);

		double rxij = rx(i, j);
		double ryij = ry(i, j);
		double rzij = rz(i, j);


		double a41 = temp4 * (3 * rxki * rxki / rk5 - 1 / rk3) + temp5 * (3 * rxij * rxij / rj5 - 1 / rj3);
		double a52 = temp4 * (3 * ryki * ryki / rk5 - 1 / rk3) + temp5 * (3 * ryij * ryij / rj5 - 1 / rj3);
		double a63 = temp4 * (3 * rzki * rzki / rk5 - 1 / rk3) + temp5 * (3 * rzij * rzij / rj5 - 1 / rj3);

		double a42 = temp4 * (3 * rxki * ryki / rk5) + temp5 * (3 * rxij * ryij / rj5);
		double a43 = temp4 * (3 * rxki * rzki / rk5) + temp5 * (3 * rxij * rzij / rj5);
		double a53 = temp4 * (3 * ryki * rzki / rk5) + temp5 * (3 * ryij * rzij / rj5);

		double a51 = a42;
		double a61 = a43;
		double a62 = a53;


		for (size_t i = 0; i < 18; i++)
		{
			yp[i + 12] = y[i + 30];
		}
		for (size_t i = 0; i < 6; i++)
		{
			yp[i + 30] = a41 * y[i + 12] + a42 * y[i + 18] + a43 * y[i + 24];
			yp[i + 36] = a42 * y[i + 12] + a52 * y[i + 18] + a53 * y[i + 24];
			yp[i + 42] = a43 * y[i + 12] + a53 * y[i + 18] + a63 * y[i + 24];
		}
	}

	void stm3bsimp(double x, double*y, double* yp)
	{
		unsigned int nbody = 3;
		const double mu[3]{ 0.012155650403207, 0, 0.987844349596793 };

		for (size_t i = 0; i < nbody; i++)
		{
			int ii = 6 * i;
			for (size_t j = 0; j < 3; j++)
			{
				yp[ii + j] = y[ii + j + 3];
			}

			yp[ii + 3] = 0.0;
			yp[ii + 4] = 0.0;
			yp[ii + 5] = 0.0;

			for (size_t j = 0; j < nbody; j++)
			{
				if (i != j)
				{
					int jj = 6 * j;
					double rx = y[ii + 0] - y[jj + 0];
					double ry = y[ii + 1] - y[jj + 1];
					double rz = y[ii + 2] - y[jj + 2];
					double r = std::sqrt(rx * rx + ry * ry + rz * rz);

					yp[ii + 3] -= mu[j] * rx / std::pow(r, 3);
					yp[ii + 4] -= mu[j] * ry / std::pow(r, 3);
					yp[ii + 5] -= mu[j] * rz / std::pow(r, 3);
				}
			}
		}
		for (size_t i = 0; i < 18; i++)
		{
			yp[i + 18] = 0;
		}
		for (size_t i = 0; i < 6; i++)
		{
			yp[i + 36] = 0;
			yp[i + 42] = 0;
			yp[i + 48] = 0;
		}



	}
}