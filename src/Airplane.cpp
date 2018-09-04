#include "Airplane.h"

Airplane::Airplane()
{
	XX=0;
	YY=0;
	ZZ=0;
	VX=0;
	VY=0;
	VZ=0;
	//ctor
}

Airplane::Airplane(double x0,double y0,double z0,double vx0,double vy0,double vz0)
{
	XX=x0;
	YY=y0;
	ZZ=z0;
	VX=vx0;
	VY=vy0;
	VZ=vz0;
}

void Airplane::linear(double x0,double y0,double z0,double vx,double vy,double vz,double t)
{
	double x,y,z;
	int n=N;
	for(double nt=0.0;nt<=t;n++,nt=nt+0.1)
	{
			x=x0+vx*nt;
			y=y0+vy*nt;
			z=z0+vz*nt;
			X[n]=x;
			X.sizeup();
			Y[n]=y;
			Y.sizeup();
			Z[n]=z;
			Z.sizeup();
	}
	XX=x;
	YY=y;
	ZZ=z;
	N=n;
}

void Airplane::ualinear(double x0,double y0,double z0,double vx,double vy,double vz,double t,double a)
{
	double x,y,z;
	int n=N;
	for(double nt=0.0;nt<=t;n++,nt=nt+0.1)
	{
			x=x0+vx*nt+0.5*a*nt*nt;
			y=y0+vy*nt+0.5*a*nt*nt;
			z=z0+vz*nt+0.5*a*nt*nt;
			X[n]=x;
			X.sizeup();
			Y[n]=y;
			Y.sizeup();
			Z[n]=z;
			Z.sizeup();
	}
	XX=x;
	YY=y;
	ZZ=z;
	N=n;
}

void Airplane::ucirculcar(double x0,double y0,double z0,double v,double t,double r)
{
	double x,y,z;
	double w=v/r;
	int n=X.size();
	for(double nt=0.0;nt<=t;n++,nt=nt+0.1)
	{
			x=x0+r*cos(w*nt);
			y=y0+r*sin(w*nt);
			z=z0+v*nt;
			X[n]=x;
			X.sizeup();
			Y[n]=y;
			Y.sizeup();
			Z[n]=z;
			Z.sizeup();
	}
	XX=x;
	YY=y;
	ZZ=z;
	N=n;
}

void Airplane::track(int x)
{
		switch(x)
	{
	case 1:
		 linear(XX,YY,ZZ,1.0,1.0,2.0,4.5);
		 break;
	case 2:
    ualinear(XX,YY,ZZ,1.0,1.0,2.0,4.5,1.0);
    break;
    case 3:
    ucirculcar(XX,YY,ZZ,1.0,4.5,1.0);
    break;
	}
}

void Airplane::show()
{
	cout<<"·É»úµÄ×ø±ê£º"<<endl;
for(int i=0;i<N;i++)
{
	cout<<"("<<X[i]<<","<<Y[i]<<","<<Z[i]<<")"<<endl;
}

}




Airplane::~Airplane()
{
	//dtor
}
