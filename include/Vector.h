#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include<stdlib.h>
#include <ctime>
using namespace std;

typedef int Rank;//秩
#define DEFAULT_CAPACITY 1000//默认的初始容量（实际可以设置的更大）
#define NCOUNT 10000000//默认的循环查找次数

template <typename T> class Vector{//向量模板类
protected:
	Rank _size;int _capacity;T* _elem;//规模、容量、数据区
	void copyFrom(T const* A,Rank lo,Rank hi);//复制数组区A[lo,hi)
	void expand();//空间不足时扩容
	void shrink();//填装因子过小时压缩
	bool bubble(Rank lo,Rank hi);//扫描交换
	void bubbleSort(Rank lo,Rank hi);//起泡排序算法
	Rank max(Rank lo,Rank hi);//选取最大元素
	void selectionSort(Rank lo,Rank hi);//选择排序算法
	void merge(Rank lo,Rank mi,Rank hi);//归并算法
	void mergeSort(Rank lo,Rank hi);//归并排序算法
	Rank partion(Rank lo,Rank hi);//轴点构造算法
	void quickSort(Rank lo,Rank hi);//快速排序算法
	void heapSort(Rank lo,Rank hi);//堆排序

public:
	//构造函数
	Vector(int c=DEFAULT_CAPACITY,int s=0,T v=0)//容量为C、规模为S、所有元素初始为v
	{
		_elem=new T[_capacity=c];
		for(_size=0;_size<s;_elem[_size++]=v);
	}//s<=c




	Vector(T const* A,Rank lo,Rank hi)//数组区间复制
	{
		copyFrom(A,lo,hi);
	}

	Vector(T const* A,Rank n)//数组整体复制
	{
		copyFrom(A,0,n);
	}

	Vector(Vector<T> const& V,Rank lo,Rank hi)//向量区间复制
	{
		copyFrom(V._elem,lo,hi);
	}

	Vector(Vector<T> const& V)//向量整体复制
	{
		copyFrom(V._elem,0,V._size);
	}

//析构函数
	~Vector(){delete [] _elem;}//释放内部空间

//只读访问接口
	Rank size() const
	{
		return _size;
	}//规模

	void sizeup()
	{
		_size++;
	}//规模加一
	int capacity() const
	{
		return _capacity;
	}//容量

	bool empty() const
	{
		return !_size;
	}//判空

	int disordered() const;//判断序列是否已排序

	Rank find(T const& e) const
	{
		return find(e,0,(Rank)_size);
	}//无序向量整体查找

	Rank find(T const& e,Rank lo,Rank hi) const;//无序向量区间查找

	Rank search(T const& e) const//有序向量整体查找
	{
		return (0>=_size)?-1:search(e,(Rank)0,(Rank)_size);
	}

	Rank search(T const& e,Rank lo,Rank hi) const;//有序向量区间查找

//可写访问接口
    T& operator[](Rank r) ;//重载下标操作符，可以类似数组形式引用各元素
    Vector<T> & operator=(Vector<T> const&);//重载赋值操作符，以便直接克隆向量
    Vector<Vector<T> > & operator=(Vector<Vector<T> > const& );//重载二维向量赋值操作符

    T remove(Rank r);//删除秩为r的元素
    int remove(Rank lo,Rank hi);//删除秩在区间[lo,hi)的元素
    void bivectorremoverow(Rank r);//删除二维向量的第r行
    Rank insert(Rank r,T const& e);//插入元素
    Rank insert(T const& e)
    {
    	return insert(_size,e);
    }//默认作为末元素插入

    void sort(Rank lo,Rank hi);//对[lo,hi)排序
    void sort()
    {
    	sort(0,_size);
    }//整体排序

    void unsort(Rank lo,Rank hi);//对[lo,hi)置乱
    void unsort()
    {
    	unsort(0,_size);
    }//整体置乱

    int deduplicate();//无序去重
    int uniquify();//有序去重

//遍历
    void traverse(void(*)(T&));//遍历（使用函数指针，只读或局部性修改）
    template <typename VST> void traverse(VST&);//遍历（使用函数对象，可全局性修改）

};//Vector


template <typename T>//元素类型
void Vector<T>::copyFrom(T const* A,Rank lo,Rank hi)//以数组区间A[lo,hi)为蓝本复制向量
{
	_elem=new T[_capacity=2*(hi-lo)];//分配空间
	_size=0;//规模清零
	while(lo<hi)
	{
		_elem[_size++]=A[lo++];
	}
}

template <typename T> Vector<T>& Vector<T>::operator=(Vector<T> const& V)//重载赋值操作符
{
	if(_elem) delete [] _elem;//释放原有内容
	copyFrom(V._elem,0,V.size());//整体复制
	return *this;//返回当前对象的引用，以便链式赋值
}

template <typename T> void Vector<T>::expand()//向量空间不足时扩容
{
	if(_size<_capacity) return;//尚未满员时，不必扩容
	if(_capacity<DEFAULT_CAPACITY)_capacity=DEFAULT_CAPACITY;//不低于最小容量
	T* oldElem=_elem;
	_elem=new T[_capacity<<1];//容量加倍
	for(int i=0;i<_size;i++)
		_elem[i]=oldElem[i];//复制原向量内容（T为基本类型，或已重载赋值操作符）
	delete [] oldElem;//释放原空间
}

template <typename T> void Vector<T>::shrink()//填装因子过小时压缩向量所占空间
{
	if(_capacity<DEFAULT_CAPACITY<<1) return;//不致收缩到DEFAULT_CAPACITY以下
	if(_size<<2>_capacity)return;//以25%为界
	T* oldElem=_elem;
	_elem=new T[_capacity>>=1];//容量减半
	for(int i=0;i<_size;i++)
		_elem[i]=oldElem[i];//复制原向量内容
	delete [] oldElem;//释放原空间
}

template <typename T> T& Vector<T>::operator[](Rank r) //重载下标操作符
{
	expand();
	return _elem[r];//assert:0<=r<=_size
}






template <typename T> void permute(Vector<T>& V)//随机置乱向量，使各元素等概率出现在每个位置
{
	for(int i=V.size();i>0;i--)//自后向前
		swap(V[i-1],V[rand()%i]);//V[i-1]与V[0,i)中某一随机元素交换
}

template <typename T> void Vector<T>::unsort(Rank lo,Rank hi)//等概率置乱向量区间[lo,hi)
{
	T* V=_elem+lo;//将子向量_elem[lo,hi)视作另一向量V[0,hi-lo)
	for(Rank i=hi-lo;i>0;i--)//自后向前
		swap(V[i-1],V[rand()%i]);//V[i-1]与V[0,i)中某一随机元素交换
}

template <typename T> static bool lt(T* a,T* b){return lt(*a,*b);}//less than
template <typename T> static bool lt(T& a,T& b){return a<b;}//less than
template <typename T> static bool eq(T* a,T* b){return eq(*a,*b);}//equal
template <typename T> static bool eq(T& a,T& b){return a==b;}//equal

template <typename T>//无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo-1
Rank Vector<T>::find(T const& e,Rank lo,Rank hi)const//assert：0<=lo<hi<=_size
{
	while((lo<hi--)&&(e!=_elem[hi]));//从后向前
	return hi;//若hi<lo,则意味着失败；否则hi即命中元素的秩
}

template <typename T>//将e作为秩为r元素插入
Rank Vector<T>::insert(Rank r,T const& e)
{
	expand();//若有必要，扩容
	for(int i=_size;i>r;i--)
	{
		_elem[i]=_elem[i-1];
	}//自后向前，后继元素顺次后移一个单元
	_elem[r]=e;
	_size++;//置入新元素并更新容量
	return r;//返回秩
}

template <typename T>
int Vector<T>::remove(Rank lo,Rank hi)//删除区间[lo,hi)
{
	if(lo==hi) return 0;//
	while(hi<_size){_elem[lo++]=_elem[hi++];}//顺次前移
	_size=lo;//更新规模
	shrink();//
	return hi-lo;//
}

template <typename T>
T Vector<T>::remove(Rank r)//单元素删除
{
	T e=_elem[r];//备份被删除元素
	remove(r,r+1);//
	return e;//
}

template <typename T>
int Vector<T>::deduplicate()//无序向量唯一化
{
	int oldSize=_size;//记录原规模
	Rank i=1;
	while(i<_size)//自前向后注意考察各元素
	{
		(find(_elem[i],0,i)<0)?//在其前缀中寻找与之雷同者
		i++:remove(i);//无雷同继续考察其后继
	}
	return oldSize-_size;//向量规模变化量
}


template <typename T>
void Vector<T>::traverse(void (*visit)(T&))//利用函数指针机制的遍历
{
	for(int i=0;i<_size;i++){visit(_elem[i]);}
}

template <typename T> template <typename VST>//元素类型、操作器
void Vector<T>::traverse(VST& visit)//利用函数对象机制的遍历
{
	for(int i=0;i<_size;i++){visit(_elem[i]);}
}

template <typename T> struct Increase//函数对象：递增一个T类对象
{
	virtual void operator()(T& e){e++;}
}; //假设T可以直接递增或已重载++

template <typename T> void increase(Vector<T> & V)//统一递增向量中的各元素
{
	V.traverse(Increase<T>());
}//以Increase<T>()为基本操作进行遍历

template <typename T> int Vector<T>::disordered()const//返回向量中逆序相邻元素对的总数
{
	int n=0;//计数器
	for(int i=1;i<_size;i++)
	{
		if(_elem[i-1]>_elem[i])n++;//逆序计数
	}
	return n;
}

template <typename T> int Vector<T>::uniquify()//有序唯一化
{
	Rank i=0,j=0;//各队互异“相邻”元素的秩
	while(++j<_size)//
		if(_elem[i]!=_elem[j])//跳过雷同者
	{
		_elem[++i]!=_elem[j];//发现不同元素时，向前移至紧邻于前者右侧
	}
	_size=++i;
	shrink();//直接截除尾部多余元素
	return j-i;//向量规模变化量
}



template <typename T> static Rank binSearchA(T *A,T const& e,Rank lo,Rank hi)//二分查找A
{
	while(lo<hi)
	{
		Rank mi=(lo+hi)>>1;//
		if(e<A[mi])hi=mi;//
		else if(A[mi]<e)lo=mi+1;//
		else return mi;
	}
	return -1;
}

template <typename T> static Rank binSearchB(T *A,T const& e,Rank lo,Rank hi)//二分查找B
{
	while(1<hi-lo)
	{
		Rank mi=(lo+hi)>>1;//
		(e<A[mi])?hi=mi:lo=mi;//
	}
	return (e==A[lo])?lo:-1;
}

template <typename T> static Rank binSearchC(T *A,T const& e,Rank lo,Rank hi)//二分查找C
{
	while(lo<hi)
	{
		Rank mi=(lo+hi)>>1;//
		(e<A[mi])?hi=mi:lo=mi+1;//
	}
	return --lo;
}



template <typename T>
Rank Vector<T>::search(T const& e,Rank lo,Rank hi)const//有序区间查找
{
	/*return(rand()%2)?
	      binSearch(_elem,e,lo,hi):fibSearch(_elem,e,lo,hi);//二分查找或Fibonacci查找
*/
clock_t start1,end1,start2,end2,start3,end3;
//start = clock();
//end = clock();
//cout << (double)(end - start) / CLOCKS_PER_SEC << endl;
    start1= clock();
	for(int i=0;i<NCOUNT;i++)
	{
		//cout<<binSearchA(_elem,e,lo,hi)<<"  ";
		binSearchA(_elem,e,lo,hi);
	}
	end1= clock();
	cout<<endl<<"算法A所需时间："<<(double)(end1-start1)/CLOCKS_PER_SEC<<"S"<<endl;
	//cout<<endl;
	 start2= clock();
		for(int i=0;i<NCOUNT;i++)
	{
		//cout<<binSearchB(_elem,e,lo,hi)<<"  ";
		binSearchB(_elem,e,lo,hi);
	}
	end2= clock();
	cout<<endl<<"算法B所需时间："<<(double)(end2-start2)/CLOCKS_PER_SEC<<"S"<< endl;
	//cout<<endl;
	start3= clock();
		for(int i=0;i<NCOUNT;i++)
	{
		//cout<<binSearchC(_elem,e,lo,hi)<<"  ";
		binSearchC(_elem,e,lo,hi);
	}
	end3= clock();
	cout<<endl<<"算法C所需时间："<<(double)(end3-start3)/CLOCKS_PER_SEC<<"S"<<endl;
	//cout<<endl;
    return 0;
}

template<typename T>//向量的起泡排序
void Vector<T>::bubbleSort(Rank lo,Rank hi)
{
	while(!bubble(lo,hi--));//逐趟扫描交换，直至全序
}
template<typename T>bool Vector<T>::bubble(Rank lo,Rank hi)//一趟扫描交换
{
	bool sorted=true;//整体有序标志
	while (++lo<hi)//自左向右，逐一检查
	{
		if(_elem[lo-1]>_elem[lo])//若逆序
		{
			sorted=false;
			swap(_elem[lo-1],_elem[lo]);//通过交换使局部有序
		}
	}
	return sorted;//返回有序标志
}



template <typename T>//向量归并排序
void Vector<T>::mergeSort(Rank lo,Rank hi)
{
	if(hi-lo<2) return;//单元素区间自然有序
	int mi=(lo+hi)>>1;//以中点为界
	mergeSort(lo,mi);//前半段排序
	mergeSort(mi,hi);//后半段排序
	merge(lo,mi,hi);//归并
}

template <typename T>//有序向量的归并
void Vector<T>::merge(Rank lo,Rank mi,Rank hi)//以mi为界，各自有序的子向量[lo,mi)和[mi,hi)
{
	T* A=_elem+lo;//合并后的向量A[0,hi-lo)=_elem[lo,hi)
	int lb=mi-lo;
	T* B=new T[lb];//前子向量B[0,lb)=_elem[lo,mi)
	for(Rank i=0;i<lb;B[i]=A[i++]){}//复制前子向量
	int lc=hi-mi;
	T* C=_elem+mi;//后子向量C[0,lc)=_elem[mi,hi)
	for(Rank i=0,j=0,k=0;(j<lb)||(k<lc);)//将B[j]和C[k]中的小者续至A末尾
	{
		if((j<lb)&&((!(k<lc)||(B[j]<=C[k]))))A[i++]=B[j++];
		if((k<lc)&&((!(j<lb)||(C[k]<B[j]))))A[i++]=C[k++];
	}
	delete [] B;//释放临时空间B
}//

template <typename T>
void Vector<T>::sort(Rank lo,Rank hi)//总排序入口
{
	switch(3)
	{
		case 1:bubbleSort(lo,hi);break;//起泡排序
		case 2:selectionSort(lo,hi);break;//选择
		case 3:mergeSort(lo,hi);break;//归并
		case 4:heapSort(lo,hi);break;//堆
		default:quickSort(lo,hi);break;//快排
	}
}

template <typename T>
void traverse2(Vector<Vector<T> > const& V)//遍历输出二维数组
{
    for(int i=0;i<V.size();i++)
	  {
	for(int j=0;j<V[i].size();j++)
   {
   	cout<<V[i][j]<<" ";
   }
   cout<<endl;
	  }
}

template <typename T>
void operation(Vector<Vector<T> > const& V,int x,T const& e)
{//二维向量所有元素同时+/-/*//一个实数
	switch(x){
		case 1://+
            for(int i=0;i<V.size();i++)
	            for(int j=0;j<V[i].size();j++)
                {
	                V[i][j]=V[i][j]+e;
                }
			break;
		case 2://-
            for(int i=0;i<V.size();i++)
	            for(int j=0;j<V[i].size();j++)
               {
	                V[i][j]=V[i][j]-e;
               }
            break;
		case 3://*
			for(int i=0;i<V.size();i++)
	            for(int j=0;j<V[i].size();j++)
               {
	                V[i][j]=V[i][j]*e;
               }
            break;
		case 4:// /
			for(int i=0;i<V.size();i++)
	            for(int j=0;j<V[i].size();j++)
              {
	                V[i][j]=V[i][j]/e;
              }
            break;
	}
}

template <typename T>
void add(Vector<Vector<T> > const& V1,Vector<Vector<T> > const& V2)//V1=V1+V2
{
            for(int i=0;i<V1.size();i++)
	            for(int j=0;j<V1[i].size();j++)
                {
	                V1[i][j]=V1[i][j]+V2[i][j];
                }
}

template <typename T>
void sub(Vector<Vector<T> > const& V1,Vector<Vector<T> > const& V2)//V1=V1-V2
{
            for(int i=0;i<V1.size();i++)
	            for(int j=0;j<V1[i].size();j++)
                {
	                V1[i][j]=V1[i][j]-V2[i][j];
                }
}

template <typename T>
Vector<Vector<T> > mul(Vector<Vector<T> > const& V1,Vector<Vector<T> > const& V2)//V1=V1*V2
{
	Vector<Vector<T> > V3,V4;
	Vector<Vector<T> > V5(6,V1.size(),Vector<int>(6,V2[0].size(),0));//3*1的矩阵
	V3=V1;
	V4=V2;
            for(int i=0;i<V1.size();i++)
	            for(int j=0;j<V2[0].size();j++)
                {
                	//V1[i][j]=0;
	               for(int k=0;k<V2.size();k++)
				   {
				   	V5[i][j]=V5[i][j]+V3[i][k]*V4[k][j];
				   }
                }
                return V5;
}

template <typename T>
Vector<Vector<T> > & Vector<T>::operator=(Vector<Vector<T> > const& V)//重载二维向量赋值操作符
{
	for(int i=0;i<V.size();i++){
	if(_elem) delete [] _elem;
	copyFrom(V[i]._elem,0,V[i].size());
	return *this;
	}
}

/*template <typename T>
void insertrow(Vector<Vector<T> > const& V,int k)//二维向量插入行
{
	Vector<T> Vx(6,3,0);
	cout<<"正在插入行...."<<endl<<V.insert(k,Vx)<<endl<<"插入成功"<<endl;
}*/

template <typename T>
void insertlist(Vector<Vector<T> > const& V,Rank r,T value)//二维向量插入列
{
    cout<<"正在插入列...."<<endl;
	for(int i=0;i<V.size();i++)
	  {
       cout<<V[i].insert(r,value);
       cout<<endl;
	  }
	cout<<"插入成功"<<endl;
}

template <typename T>
void bivectorfind(Vector<Vector<T> > const& V,T value)//二维向量无序逐行查找
{
	for(int i=0;i<V.size();i++)
	  {
	    cout<<"第"<<i+1<<"行的查找结果是："<<V[i].find(value)<<endl;
	  }
}


template <typename T>
void Vector<T>::bivectorremoverow(Rank r)//删除二维向量的第r+1行
{
	cout<<"正在删除行..."<<endl;
    Rank hi=r+1;
	while(r+1<_size)
		{
			_elem[r++]=_elem[hi++];
	}//顺次前移
	_size--;//更新规模
	cout<<"成功删除第"<<r<<"行"<<endl;
}

template <typename T>
void bivectorremovelist(Vector<Vector<T> > const& V,Rank r)//二维向量删除第r+1列
{
    cout<<"正在删除列...."<<endl;
	for(int i=0;i<V.size();i++)
	  {
       cout<<V[i].remove(r);
       cout<<endl;
	  }
	cout<<"成功删除第"<<r+1<<"列"<<endl;
}


#endif // VECTOR_H
