#ifndef SA_H_INCLUDED
#define SA_H_INCLUDED

#include "report.h"
#include "testfun.h"

using namespace std;

static const unsigned sa_seed = std::chrono::system_clock::now().time_since_epoch().count();
static mt19937 gen_sa(sa_seed);

int sign(double x)
{
	/*������� ����� �����*/
	if (x == 0) return 0;
	else if (x > 0) return 1;
	else return -1;
}

FullReport SimulatedAnnealing(const int &fcode, const int &n, const point &st, const int &max_call_f, const double &InitialTemperature, const double &EndTemperature, const int &inner_iter_max, const double &rate)
{
	/*��������� - ��������� �����, �����������, �������, ��������� �����������, �������� �����������, ������������ ����� ���������� ��������, ������������ ����� ������� �������, �������� ��������� �����������*/
	FullReport fr;
	uniform_real_distribution <double> distr_01(0.0, 1.0); /*��������� �����*/
	testfun::call_f = 0; /*������ ��*/
	point x = st; /*����� ������*/
	double y = testfun::f(n, x, fcode); /*�������� ������� � �����*/
	Report r(testfun::call_f, y);
	fr.insert_into_report(r);
	double t = InitialTemperature; /*��������� �����������*/
	while (t > EndTemperature && testfun::call_f<max_call_f)
	{
		for (int inner_iter = 0; inner_iter < inner_iter_max && testfun::call_f<max_call_f; ++inner_iter)
		{
			point sc = pure_random(n, testfun::lb[fcode], testfun::rb[fcode]); /*��������� ����� �� ������� ���������*/
			double g = testfun::f(n, sc, fcode); /*�������� ������� � ����� �����*/
			double prob = y>g ? 1.0 : exp((y - g) / t); /*���� ����� �������� ������, ��� ������, �� ����������� 1.0, ����� ������� �� �������*/
			double coin = distr_01(gen_sa); /*��������� �����*/
			if (coin<prob) /*����������� ������*/
			{
				x = sc;
				y = g;
				/*������ ������ ����� � �������� ������*/
			}
			Report r(testfun::call_f, y);
			fr.insert_into_report(r);
		}
		t *= rate;
	}
	return fr;
}


FullReport VeryFastSimulatedAnnealing(const int &fcode, const int &n, const point &st, const int &max_call_f, const double &InitialTemperature, const double &EndTemperature, const int &inner_iter_max, const double &rate)
{
	/*very fasts imulated annealing ������������ ������� ������ - �� �.������� (�� ����� ������ �������� "����� ������")*/
	/*��������� - ��������� �����, �����������, �������, ��������� �����������, �������� �����������, ������������ ����� ���������� ��������, ������������ ����� ������� �������, �������� ��������� �����������*/
	FullReport fr;
	string fname = "VeryFastSimulatedAnnealing_" + testfun::namefun[fcode] + ".csv"; /*��� �����+������*/
	freopen(fname.c_str(), "w", stdout); /*������ �����������*/
	uniform_real_distribution<double> distr_01(0.0, 1.0); /*��������� �����*/
	testfun::call_f = 0; /*���������� ������� �������*/
	point x = st; /*����� ������*/
	double y = testfun::f(n, x, fcode); /*�������� ������� � �����*/
	Report r(testfun::call_f, y);
	fr.insert_into_report(r);
	uniform_int_distribution<int> distr_index(0, 1);
	vector < double > t(n, InitialTemperature); /*������ �������� ���������� �� �����������*/
	vector < double > alpha(n, 0);
	vector < double > z(n, 0);
	vector < double > c(n, 0);
	vector < double > m(n, 0), s(n, 0);
	vector < double > sc(n, 0); /*����� ��������*/
	for (int i = 0; i<n; ++i)
	{
		m[i] = distr_01(gen_sa);
		s[i] = distr_01(gen_sa);
	}
	int l = 0;
	double tg = InitialTemperature;
	while (testfun::call_f < max_call_f && tg > EndTemperature)
	{
		for (int i = 0; i<n; ++i)
		{
			do
			{
				alpha[i] = distr_01(gen_sa);
				z[i] = sign(alpha[i] - 0.5)*t[i] * (pow(1 + l / t[i], 2 * alpha[i] - 1) - 1);
				sc[i] = x[i] + z[i] * (testfun::rb[fcode] - testfun::lb[fcode]);
			} while (sc[i]<testfun::lb[fcode] || sc[i]>testfun::rb[fcode]);
		}
		double g = testfun::f(n, sc, fcode);
		++testfun::call_f; /*+1 ����� �������*/
		double dt = g - y;
		double h_x = 1 / (1 + exp(dt / tg)); /*��������� ����������� ������*/
		double coin = distr_01(gen_sa); /*��������� �����*/
		if (coin<h_x) /*�������� ����������� ������*/
		{
			x = sc;
			y = g;
			/*������ ������ ����� � �������� ������*/
		}
		//cout << testfun::call_f << ";" << y << ";"; /*����� ������ � �������� ���� - ���������� ������� �������,  �������� �������*/
		Report r(testfun::call_f, y);
		fr.insert_into_report(r);
		for (int i = 0; i<n; ++i)
		{
			c[i] = m[i] * exp(-s[i] / 2);
			t[i] = InitialTemperature*exp(-c[i] * pow(l, 1 / 2));
		}
		double sg = distr_01(gen_sa);
		double mg = distr_01(gen_sa);
		double cg = mg*exp(-sg / 2);
		tg = InitialTemperature*exp(-cg*pow(l, 1 / 2));
	}
	return fr;
}

#endif