#ifndef PLANE_H
#define PLANE_H
#include <fstream>
#include <iostream>
#include"Vector.h"
#include"List.h"
#include<math.h>
using namespace std;

#define PI 3.1415926
#define Ts 0.1//采样周期

struct Coordinate//坐标
{
	double x;
	double y;
	double z;
};

struct velocity//速率
{
	double x;
	double y;
	double z;
};

class Plane
{
	private:
		List<int> FlyPlane;//飞机
		Vector<double> X,Y,Z;//飞机的三维坐标
		velocity init_v;//初始速度（单位：Km/h）
		velocity RealTime_v;//实时速度（单位：Km/h）
		double acceleration=0;//加速度(单位：Km/h/h)
		double R;//半径（单位：Km）
		Coordinate RealTime_c;//飞机的实时坐标(单位：Km）
		double T;//飞机飞行的时间(单位：h）
		int N=0;//Vector<double> X,Y,Z的规模
		int order[100];//初始轨迹序列
		int s=0;//数组order的长度

	public:

		Plane();
		Plane(Coordinate c,velocity v);
		virtual ~Plane();

	public:
		void linear(Coordinate c0,velocity v0,double t);//匀速直线运动
		void ualinear(Coordinate c0,velocity v0,double t,double a);//曲线运动
		void ucirculcar(Coordinate c0,velocity v0,double t,double r);//螺旋运动
		void track(int x);//飞机的三种轨迹
		void show();//输出飞机各时间点的坐标
		void plot();//依次绘制飞机轨迹
		void writefile();//将飞机三维坐标分别写到out.txt文件中
		void deletrack(int i);//删除第i段轨迹
		void addtrack(int i,int j);//在第i段轨迹后添加轨迹j
		void gettrack();//输入轨迹
		void puttrack();//输出轨迹序列
		void updata_c(Coordinate _c);//更新位置和速度
		void assignment(int n,Coordinate _c);//将离散化的坐标保存到X,Y,Z中

	protected:

};

#endif // PLANE_H
