#include "Plane.h"

Plane::Plane()
{
	init_v.x=1,init_v.y=2,init_v.z=3;
	RealTime_c.x=0,RealTime_c.y=0,RealTime_c.z=0;
	RealTime_v.x=init_v.x,RealTime_v.y=init_v.y,RealTime_v.z=init_v.z;
}

Plane::Plane(Coordinate c,velocity v)
{
	RealTime_c.x=c.x,RealTime_c.y=c.y,RealTime_c.z=c.z;
	RealTime_v.x=v.x,RealTime_v.y=v.y,RealTime_v.z=v.z;
}

Plane::~Plane()
{
	//dtor
}

void Plane::linear(Coordinate c0,velocity v0,double t)
{
	Coordinate c;
	int n=N;
	acceleration=0;//加速度置0
	for(double nt=0.0;nt<=t;n++,nt=nt+Ts)
	{
		c.x=c0.x+v0.x*nt;
			c.y=c0.y+v0.y*nt;
		c.z=c0.z+v0.z*nt;
		assignment(n,c);
	}
	updata_c(c);
	N=n;
}

void Plane::ualinear(Coordinate c0,velocity v0,double t,double a)
{
	Coordinate c;
	int n=N;
	acceleration=a;//更新加速度
	for(double nt=0.0;nt<=t;n++,nt=nt+Ts)
	{
		c.x=c0.x+v0.x*nt+0.5*a*nt*nt;
		c.y=c0.y+v0.y*nt;
		c.z=c0.z+v0.z*nt;
		assignment(n,c);
	}
	updata_c(c);
	RealTime_v.x=v0.x+a*t;//更新速度
	N=n;
}

void Plane::ucirculcar(Coordinate c0,velocity v0,double t,double r)
{
	acceleration=0;//加速度置0
	double v=sqrt(v0.x*v0.x+v0.y*v0.y);
	Coordinate c;
	double w=v/r;
	double a,b,x1,y1;
	double q;
	int n=N;
	if(c0.x==0&&c0.y!=0)
	{
		a=0;b=1;
	}
	else if(c0.x!=0&&c0.y==0)
	{
		a=1;b=0;
	}
	else if(c0.x==0&&c0.y==0)
	{
		a=cos(PI/4);
		b=sin(PI/4);
	}
	else
	{
		a=v0.y/v;
		b=v0.x/v;
	}
	if(v0.x>=0&&v0.y>=0)
	{
		x1=c0.x-r*a;
	    y1=c0.y+r*b;
	    q=-acos(a);
	}
	else if(v0.x>0&&v0.y<0)
	{
		x1=c0.x-r*a;
	    y1=c0.y+r*b;
	    q=-(acos(b)+(PI/2));
	}
	else if(v0.x<=0&&v0.y<=0)
	{
		x1=c0.x-r*a;
	    y1=c0.y+r*b;
	    q=-(acos(-a)+PI);
	}
	else
	{
		x1=c0.x-r*a;
	    y1=c0.y+r*b;
	    q=acos(a);
	}

	for(double nt=0.0;nt<=t;n++,nt=nt+Ts)
	{
		c.x=x1+r*cos(q+w*nt);
		c.y=y1+r*sin(q+w*nt);
		c.z=c0.z+v0.z*nt;
		assignment(n,c);
	}
	updata_c(c);

	double e=(q+w*t)/(2*PI)-(int)((q+w*t)/(2*PI));
	double m=e*2*PI;
	if(0<=m&&m<(PI/2))
	{
		RealTime_v.x=-v*sin(m);
		RealTime_v.y=v*cos(m);
	}
	else if((PI/2)<=m&&m<PI)
	{
		RealTime_v.x=-v*sin(PI-m);
		RealTime_v.y=-v*cos(PI-m);
	}
	else if(PI<=m&&m<(PI*3/2))
	{
		RealTime_v.x=v*sin(m-PI);
		RealTime_v.y=-v*cos(m-PI);
	}
	else
	{
		RealTime_v.x=v*sin(2*PI-m);
		RealTime_v.y=v*cos(2*PI-m);
	}
	RealTime_v.z=v0.z;//更新速度
	N=n;
}

void Plane::track(int x)//飞机的三种轨迹
{
	switch(x)
	{
	case 1:
		{
		 linear(RealTime_c,RealTime_v,T);
		 break;
		}
	case 2:
		{
         ualinear(RealTime_c,RealTime_v,T,acceleration);
		 break;
		}
    case 3:
    	{
         ucirculcar(RealTime_c,RealTime_v,T,R);
         break;
    	}
	}
}

void Plane::show()//输出飞机各时间点的坐标
{
	cout<<"飞机的坐标："<<endl;
	for(int i=0;i<N;i++)
	{
		cout<<"("<<X[i]<<","<<Y[i]<<","<<Z[i]<<")"<<endl;
	}
}

void Plane::plot()//依次绘制飞机轨迹
{
	Posi(int) p=FlyPlane.first();//p指向Plane的首节点
	for(int i=0;i<FlyPlane.size();i++)
	{
		if(p->data==1)
		{
			cout<<"请输入直线运动的时间："<<endl;
			cin>>T;
		}
		else if(p->data==2)
		{
			cout<<"请输入曲线运动的时间和加速度："<<endl;
			cin>>T>>acceleration;
		}
		else
		{
			cout<<"请输入匀速上升螺旋运动的时间和半径："<<endl;
			cin>>T>>R;
		}
    track(p->data);
	p=p->succ;
}
	delete p;
}

void Plane::writefile()//将飞机三维坐标分别写到out.txt文件中
{
	ofstream outfile("./out.txt"); //写入文件
	for(int i=0;i<N;i++)
	{
		outfile<<X[i];
		outfile<<" ";
		outfile<<Y[i];
		outfile<<" ";
		outfile<<Z[i];
		if(i<N-1)outfile<<"\n";
	}
	outfile.close();
}


void Plane::deletrack(int i)//删除第i段轨迹
{
	cout<<"删除第"<<i<<"段轨迹:"<<endl;
	Posi(int) pi=FlyPlane.first();
	for(int j=0;j<i-1;j++)
	{
		pi=pi->succ;
	}
	FlyPlane.remove(pi);
	delete pi;
}

void Plane::addtrack(int i,int j)//在第i段轨迹后添加轨迹j
{
	cout<<"在第"<<i<<"段轨迹后添加轨迹"<<j<<":"<<endl;
	Posi(int) pi=FlyPlane.first();
	Posi(int) px;
	for(int k=0;k<i-1;k++)
	{
		pi=pi->succ;
	}
	px=FlyPlane.insertAfter(pi,j);
}

void Plane::gettrack()//输入轨迹
{
	cout<<"1:匀速直线运动"<<endl;
	cout<<"2:曲线运动（当加速度方向与速度方向相同时，为匀加速直线运动）"<<endl;
	cout<<"3:匀速上升的螺旋运动"<<endl;
	cout<<"请依此输入轨迹（回车结束）："<<endl;

	for(int i=0;;i++)
    {
        cin>>order[i];
        if(getchar()=='\n')//遇回车结束
		{
			s=i+1;
            break;
		}
    }

    for(int i=0;i<s;i++)
	{
		Posi(int) p=FlyPlane.insertAsLast(order[i]);//将order[i]当作末节点插入
	}

}

void Plane::puttrack()//输出轨迹序列
{
	Posi(int) p1=FlyPlane.first();
	for(int i=0;i<FlyPlane.size();i++)
	{
		cout<<p1->data<<" ";
		p1=p1->succ;
	}
	cout<<endl;
	delete p1;
}

void Plane::updata_c(Coordinate _c)
{
	RealTime_c.x=_c.x;RealTime_c.y=_c.y;RealTime_c.z=_c.z;//更新位置
}

void Plane::assignment(int n,Coordinate _c)
{
	X[n]=_c.x;
	X.sizeup();//X[]规模加1
	Y[n]=_c.y;
	Y.sizeup();
	Z[n]=_c.z;
	Z.sizeup();
}



