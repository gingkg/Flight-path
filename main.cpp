//产生一段飞机的飞行轨迹，可以添加或者删除某一小段轨迹，
//可以产生匀速直线运动，曲线运动，螺旋运动
//将三维坐标写入txt文件保存
//Author:Shenbaoyin & PangHongli & XuChensong Time:2018/05/24
#include <iostream>
#include "Plane.h"
using namespace std;

int main()
{
	Plane P1;
	P1.gettrack();//初始化轨迹
	P1.addtrack(3,2);//添加一条轨迹
	P1.puttrack();//输出轨迹序列
	P1.deletrack(2);//删除一条轨迹
	P1.puttrack();
	P1.plot();//轨迹点采样
	P1.show();//输出轨迹点
	P1.writefile();//将三维坐标写入txt文件保存

	return 0;
}
