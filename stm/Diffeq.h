#pragma once

namespace diffeq {
	void two_body(double x, double* y, double* yp);
	void seven_body(double x, double* y, double* yp);
	void stm3b(double x, double* y, double* yp);
	void stm3bsimp(double x, double* y, double* yp);
};