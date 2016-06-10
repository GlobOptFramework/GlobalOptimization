#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <random>
#include <chrono>
#include <vector>

#include "testfun.h"

using namespace std;

typedef vector<double> point;

const double wa = 5.12;

static const unsigned seed_point = std::chrono::system_clock::now().time_since_epoch().count();
static mt19937 gen_point(seed_point);
uniform_real_distribution<double> distr_01(0.0, 1.0);
uniform_real_distribution<double> distr_point(-wa, wa);
uniform_real_distribution<double> distr_semipoint(0, wa);

point pure_random(const int &k)
{
	point x(k, 0);
	for (int i = 0; i<k; ++i)
		x[i] = distr_point(gen_point);
	return x;
}

point pure_random(const int &k, const double &la, const double &ra)               //��������� ��������� ����� � ���������������
{
	uniform_real_distribution<double> distr_point_border(la, ra);
	point x(k, 0);
	for (int i = 0; i<k; ++i)
		x[i] = distr_point_border(gen_point);
	return x;
}


/**UTILS**/
double radius(const int &n, const point &x)  /*������*/
{
	double s = 0;
	for (int i = 0; i<n; ++i)
		s = s + x[i] * x[i];
	return sqrt(s);
}
void mult_scalar(const int &n, point &x, const double &k)  /*��������� ����� (�������) �� ������*/
{
	for (int i = 0; i<n; ++i)
		x[i] = x[i] * k;
}
double dist_points(const int &n, const point &x, const point &y)  /*���������� ����� �������*/
{
	double s = 0;
	for (int i = 0; i<n; ++i)
	{
		double t = x[i] - y[i];
		s = s + t*t;
	}
	return sqrt(s);
}
double dist_points(const point &x, const point &y)   /*���������� ����� �������*/
{
	if (x.size() != y.size())
		return -1;
	else
	{
		double s = 0;
		int n = x.size();
		for (int i = 0; i<n; ++i)
		{
			double t = x[i] - y[i];
			s = s + t*t;
		}
		return sqrt(s);
	}
}
/**ZERO CENTERED**/
bool check_zero_ball(const int &n, const point &x, const double &a)  /* ������ ����� <= ����� a: true  - ��, false - ���?*/
{
	double s = 0;
	for (int i = 0; i<n; ++i)
		s = s + x[i] * x[i];
	return s <= a*a ? true : false;
}
point inner_zero_cube(const int &n, const double &a)       /*���������� ����� ������ ��������������� �� �������� 2a*/
{
	uniform_real_distribution<double> distr_a(-a, a);
	point x(n, 0);
	for (int i = 0; i<n; ++i)
		x[i] = distr_a(gen_point);
	return x;
}
point inner_zero_ball_bruteforce(const int &n, const double &a, const int &maxattempts, int &attempts)
{
	/*non-effective but works, use inner_zero_ball_scaling instead*/
	attempts = 0;
	point x = inner_zero_cube(n, a); /*���������� ����� ������ ������� �� -a �� �*/
	while ((attempts<maxattempts) && (!check_zero_ball(n, x, a)))
	{
		x = inner_zero_cube(n, a);
		++attempts;
	}
	return x;	/*���������� �����*/
}
point inner_zero_ball_scaling(const int &n, const double &a)
{
	point x = inner_zero_cube(n, a);	  /*���������� ����� ������ ��������������� �� �������� 2a*/
	double r = radius(n, x);              //������ (���������� �� ������ ��������� �� ����� x)
	double k = r / a;                     //��������� ������� � a
	double alpha = distr_01(gen_point);   //��������� ����� �� 0 �� 1
	double c = alpha / k;                 //��������� alpha � k
	mult_scalar(n, x, c);                 //���������� ����� (��������� ����� �� ����� c)
	return x;                             //���������� ���������� �����
}
point inner_zero_ring_brute(const int &n, const double &a, const double &b)
{
	point x = inner_zero_ball_scaling(n, b);
	double r = radius(n, x);
	while ((r<a) || (r>b))
	{
		x = inner_zero_ball_scaling(n, b);
		r = radius(n, x);
	}  /*���������� ����� ����� ���, ����� ������ ��� � ��������� �� a �� b*/
	return x;
}
bool check_zero_ring_ab(const int &n, const point &x, const double &a, const double &b)
{
	double s = radius(n, x);
	return ((s >= a) && (s <= b)) ? true : false;  /*�� ��������� �� ������ ����� �������� ��������: true - ��, false- ��� */
}
/**ANY CENTERED**/
void parallel_translation(const int &n, point &x, const point &c)  /*������������ �������*/
{
	for (int i = 0; i<n; ++i)
		x[i] = x[i] + c[i];
}
bool check_centered_ball(const int &n, const point &x, const point &c, const double &r)
{
	/*return true, ���� ����� ����������� ����*/
	double s = 0;
	for (int i = 0; i<n; ++i)
	{
		double t = c[i] - x[i];
		s = s + t*t;
	}
	return s <= r*r ? true : false;
}
point inner_centered_cube(const int &n, const point &c, const double &r)
{
	point x = inner_zero_cube(n, r);
	parallel_translation(n, x, c);
	return x;
}
point inner_centered_ball_scaling(const int &n, const point &c, const double &r)
{
	point x = inner_zero_ball_scaling(n, r);
	parallel_translation(n, x, c);               //������������ �������
	return x;
}
point stoh_inner_centered_ball_scaling(const int &n, const point &c, double &mut_radius)
{
	mut_radius = distr_semipoint(gen_point);                      //������ ������� = random(0, wa)
	point x = inner_zero_ball_scaling(n, mut_radius);             //�������� ����� x            
	parallel_translation(n, x, c);                                //������������ ������� x[i]=x[i]+c[i]
	return x;
}
point stoh_inner_centered_ball_scaling2(const int &n, const point &c, double &mut_radius)
{
	mut_radius = distr_01(gen_point);                             //������ ������� = random(0, 1)
	point x = inner_zero_ball_scaling(n, mut_radius);             //�������� ����� x            
	parallel_translation(n, x, c);                                //������������ ������� x[i]=x[i]+c[i]
	return x;
}
point inner_centered_ring_brute(const int &n, const point &c, const double &a, const double &b)
{
	point x = inner_zero_ring_brute(n, a, b);
	parallel_translation(n, x, c);
	return x;
}
/**ANY CENTERED STOHASTIC RADIUS**/
point inner_centered_ring_brute(const int &n, const point &c)
{
	double a = distr_semipoint(gen_point);
	double b = distr_semipoint(gen_point);
	if (a>b)
	{
		b = a + b;
		a = b - a;
		b = b - a;
	}
	point x = inner_centered_ring_brute(n, c, a, b);
	return x;
}
#endif // POINT_H_INCLUDED
