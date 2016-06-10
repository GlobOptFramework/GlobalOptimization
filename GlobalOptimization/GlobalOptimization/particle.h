#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

static const unsigned seed_pso = std::chrono::system_clock::now().time_since_epoch().count();
static mt19937 gen_pso(seed_pso);
uniform_real_distribution<double> dc(0.0, 1.0);

class Particle
{
public:
	point _currentPosition;			            //������� ������� (���������� �������)
	double _currentValue;                       //�������� �� � ������� �������
	point _localBestPosition;			        //������ ������� �������
	double _localBestValue;                     //�������� �� � ������ �������
	vector<double> _velocity;					//�������� �������		

	Particle() {}		//���������� ��� ����������
	Particle(const int &fcode, const point &p) : _currentPosition(p)                    //����������� �� ������� point
	{
		_currentValue = testfun::f(_currentPosition.size(), _currentPosition, fcode);
		_localBestPosition = _currentPosition;
		_localBestValue = _currentValue;
		_velocity.resize(_currentPosition.size());
		_velocity.assign(_velocity.size(), 0);
	}
	Particle(const int &n, const int &fcode)     //����������� ��������� �������
	{
		_currentPosition = pure_random(n, testfun::lb[fcode], testfun::rb[fcode]);
		_currentValue = testfun::f(n, _currentPosition, fcode);
		_localBestPosition = _currentPosition;
		_localBestValue = _currentValue;
		_velocity.resize(_currentPosition.size());
		_velocity.assign(_velocity.size(), 0);
	}
	Particle(const int &n, point &currentPosition, vector<double> &velocity, const int &fcode) // ����������� � �����������
	{
		_currentPosition = currentPosition;
		_currentValue = testfun::f(n, _currentPosition, fcode);
		_localBestPosition = _currentPosition;
		_localBestValue = _currentValue;
		_velocity = velocity;
	}

	/*	 point show_currentPosition()const		//�������� ������� �������
	{
	return _currentPosition;
	}

	double show_currentValue()const        //�������� �������� �� � ������� �������
	{
	return _currentValue;
	}

	point show_localBestPosition()const		//�������� ������ ������� �������
	{
	return _localBestPosition;
	}

	double show_localBestValue()const		//�������� �������� �� � ������ �������
	{
	return _localBestValue;
	}

	vector<double> show_velocity()const		//�������� �������� �������
	{
	return _velocity;
	}*/

	void checkParticle(Particle &x, const int &fcode)		//����� ��������� �������
	{
		for (int j = 0; j < x._currentPosition.size(); j++)
		{
			if (x._currentPosition[j] < testfun::lb[fcode])
				x._currentPosition[j] = testfun::lb[fcode];
			if (x._currentPosition[j] > testfun::rb[fcode])
				x._currentPosition[j] = testfun::rb[fcode];
		}
	}

};



#endif