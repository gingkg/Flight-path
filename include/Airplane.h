#ifndef AIRPLANE_H
#define AIRPLANE_H
//#include"List.h"
#include"Vector.h"

class Airplane
{
	private:
		Vector<double> X(1000,0,0),Y(1000,0,0),Z(1000,0,0);//飞机的三维坐标
		// X(1000,0,0),Y(1000,0,0),Z(1000,0,0);//飞机的三维坐标
        double A,V,VX,VZ,VY;//加速度，速度
        double XX,YY,ZZ;//飞机的实时坐标
        double T;//飞机飞行的时间
	    int N=0;

	public:

		Airplane();
		Airplane(double x0,double y0,double z0,double vx,double vy,double vz);
		virtual ~Airplane();
    void linear(double x0,double y0,double z0,double vx,double vy,double vz,double t);
    int ualinear(double x0,double y0,double z0,double vx,double vy,double vz,double t,double a);
	int ucirculcar(double x0,double y0,double z0,double v,double t,double r);
	void track(int x);
	void show();
	int size()
	{
		return X.size();
	}
	int count()
	{
		return N;
	}
	protected:


};

#endif // AIRPLANE_H
