#ifndef LIST_H
#define LIST_H
#define Posi(T) ListNode<T>*//列表节点
typedef int Rank;//秩
#include<stdlib.h>
template <typename T>
struct ListNode//列表节点模板类（以双向链表形式实现）
{
	T data;//数值
	Posi(T) pred;//前驱
	Posi(T) succ;//后继
	ListNode() {}//针对header和trailer的构造
	ListNode(T e,Posi(T) p=NULL,Posi(T) s=NULL)
	{
		data=e;
		pred=p;
		succ=s;
	}//默认构造器
	Posi(T) insertAsPred(T const& e);//前插入
	Posi(T) insertAsSucc(T const& e);//后插入
};



template<typename T>
class List//列表模板类
{
	private:
		int _size;//规模
		Posi(T) header;//头哨兵
		Posi(T) trailer;//尾哨兵

	protected:
		void init();//；列表创建时的初始化
		int clear();//清除所有节点
		void copyNodes(Posi(T),int);//复制列表中自位置p起的n项
		void merge(Posi(T)&,int,List<T>&,Posi(T),int);//有序列表区间归并
		void mergesort(Posi(T)&,int);//对从p开始连续的n个节点归并排序
		void selectionsort(Posi(T),int);//对从p开始连续的n个节点选择排序
		void insertionsort(Posi(T),int);//对从p开始连续的n个节点插入排序

	public:
		//构造函数
		List()
		{
			init();//默认
		}
		List(List<T> const& L);//整体复制列表L
		List(List<T> const& L,Rank r,int n);//复制列表L中自r起的n项
		List(Posi(T) p,int n);//复制列表中自位置p起的n项

		//析构函数
		~List()
		{
			clear();
			delete header;
			delete trailer;
		}// 释放所有节点

		//只读访问接口
		Rank size() const
		{
			return _size;//规模
		}
		bool empty() const
		{
			return _size<=0;//判空
		}
		T& operator[](Rank r) const;//重载，支持循秩访问
		Posi(T) first() const
		{
			return header->succ;//首节点位置
		}
		Posi(T) last() const
		{
			return trailer->pred;//尾节点位置
		}
		bool valid(Posi(T) p)//判断位置p对外是否合法
		{
			return p&&(trailer!=p)&&(header!=p);//将头尾节点等同于NULL
		}
		int disordered() const;//判断列表是否已排序
		Posi(T) find(T const& e) const//无序列表查找
		{
			return find(e,_size,trailer);
		}
		Posi(T) find(T const& e,int n,Posi(T) p) const;//无序区间查找
		Posi(T) search(T const& e) const//有序列表查找
		{
			return search(e,_size,trailer);
		}
		Posi(T) search(T const& e,int n,Posi(T) p) const;//有序区间查找
		Posi(T) selectMax(Posi(T) p,int n);//在p及其n-1个后继中选出最大者
		Posi(T) selectMax()//整体最大者
		{
			return selectMax(header->succ,_size);
		}

		//可写访问接口
		Posi(T) insertAsFirst(T const& e);//将e当作首节点插入
		Posi(T) insertAsLast(T const& e);//将e当作末节点插入
		Posi(T) insertBefore(Posi(T) p,T const& e);//将e当作p的前驱插入
		Posi(T) insertAfter(Posi(T) p,T const& e);//将e当作p的后继插入
		void remove(Posi(T) p);//删除p处的节点
		void merge(List<T>& L)
		{
			merge(first(),size,L,L.first(),L,_size);//全列表归并
		}
		void sort(Posi(T) p,int n);//列表区间排序
		void sort()
		{
			sort(first(),_size);//列表整体排序
		}
		int deduplicate();//有序去重
		int uniquify();//无序去重

	    //遍历
	void traverse(void(*)(T&));//遍历（使用函数指针，只读或局部性修改）
    template <typename VST> //操作器
    void traverse(VST&);//遍历（使用函数对象，可全局性修改）
};//List

template<typename T>
void List<T>::init()//初始化，创建列表对象时统一调用
{
	header=new ListNode<T>;//创建头哨兵节点
	trailer=new ListNode<T>;//创建尾哨兵节点
	header->succ=trailer;
	header->pred=NULL;//互联
	trailer->pred=header;
	trailer->succ=NULL;//互联
	_size=0;//记录规模
}

template <typename T>
T& List<T>::operator[](Rank r)const//O(r)效率低下
{
	Posi(T) p=first();//从首节点出发
	while(0<r--)
	{
		p=p->succ;//顺数第r个节点
	}
	return p->data;//目标节点
}//任一节点的秩，亦即其前驱总数

template <typename T>//在无序列表内节点p的n个前驱中，找到等于e的最后者
Posi(T) List<T>::find(T const& e,int n,Posi(T) p) const
{
	while(0<n--)//从右向左
	{
		if(e==(p=p->pred)->data) return p;
	}
	return NULL;
}//失败，返回NULL

template <typename T>
Posi(T) List<T>::insertAsFirst(T const& e)
{
	_size++;
	return header->insertAsSucc(e);
}//e当作首节点插入

template <typename T>
Posi(T) List<T>::insertAsLast(T const& e)
{
	_size++;
	return trailer->insertAsPred(e);
}//e当作尾节点插入

template <typename T>
Posi(T) List<T>::insertBefore(Posi(T) p,T const& e)
{
	_size++;
	return p->insertAsPred(e);
}//e当作p的前驱插入

template <typename T>
Posi(T) List<T>::insertAfter(Posi(T) p,T const& e)
{
	_size++;
	return p->insertAsSucc(e);
}//e当作p的后继插入

template <typename T>
Posi(T) ListNode<T>::insertAsPred(T const& e)//前插
{
	Posi(T) x=new ListNode(e,pred,this);//创建新节点
	pred->succ=x;
	pred=x;//设置正向连接
	return x;//返回新节点
}

template <typename T>
Posi(T) ListNode<T>::insertAsSucc(T const& e)//后插
{
	Posi(T) x=new ListNode(e,this,succ);//创建新节点
	succ->pred=x;
	succ=x;//设置逆向连接
	return x;//返回新节点
}

template <typename T>//复制列表中自位置p起的n项
void List<T>::copyNodes(Posi(T) p,int n)
{
	init();//
	while(n--)
	{
		insertAsLast(p->data);
		p=p->succ;
	}
}

template<typename T>
List<T>::List(Posi(T) p,int n)//复制列表中自位置p起的n项
{
	copyNodes(p,n);
}

template<typename T>
List<T>::List(List<T> const& L)//复制整体列表L
{
	copyNodes(L.first(),L._size);
}

template<typename T>
List<T>::List(List<T> const& L,int r,int n)//复制L中自第r项起的n项
{
	copyNodes(L[r],n);
}

template<typename T>
void List<T>::remove(Posi(T) p)//删除p处节点，返回其数值
{
	//T e=p->data;//备份
	p->pred->succ=p->succ;//后继
	p->succ->pred=p->pred;//前驱
	delete p;//删除节点
	_size--;//更新规模
	//return e;//返回备份的值
}

template<typename T>
int List<T>::clear()//清空列表
{
	int oldSize=_size;
	while(0<_size)remove(header->succ);//反复删除首节点
	return oldSize;
}

template<typename T>
int List<T>::deduplicate()//无序唯一化
{
	if(_size<2) return 0;
	int oldSize=_size;//
	Posi(T) p=header;
	Rank r=0;
	while(trailer!=(p=p->succ))
	{
		Posi(T) q=find(p->data,r,p);//在p的r个前驱中寻找雷同者
		q?remove(q):r++;
	}
	return oldSize-_size;
}

template <typename T>
void List<T>::traverse(void (*visit)(T&))//遍历（使用函数指针，只读或局部性修改）
{
	for(Posi(T) p=header->succ;p!=trailer;p=p->succ)
	{
		visit(p->data);
	}
}

template <typename T> template <typename VST>
void List<T>::traverse(VST& visit)//遍历（使用函数对象，可全局性修改）
{
	for(Posi(T) p=header->succ;p!=trailer;p=p->succ)
	{
		visit(p->data);
	}
}

template<typename T>
int List<T>::uniquify()//有序唯一化
{
	if(_size<2) return 0;
	int oldSize=_size;//
	Posi(T) p,q;//依次指向紧邻的各对节点
	for(p=header,q=p->succ;trailer!=q;p=q,q=q->succ)//自左向右扫描
	{
		if(p->data==q->data)
		{
			remove(q);
			q=p;
		}//若p和q雷同，删除后者
	}
	return oldSize-_size;
}

template <typename T>
Posi(T) List<T>::search(T const& e,int n,Posi(T) p) const//有序查找
{
	while(0<=n--)//从右向左
	{
		if(((p=p->pred)->data)<=e) break;
	}
	return p;//返回查找终止的位置
}

template <typename T>
void List<T>::sort(Posi(T) p,int n)//列表排序
{
	switch (rand()%3)
	{
		case 1:insertionsort(p,n);break;
		case 2:selectionsort(p,n);break;
		default:mergesort(p,n);break;
	}
}

template <typename T>
void List<T>::insertionsort(Posi(T) p,int n)//列表插入排序
{
	for(int r=0;r<n;r++)//逐一为各节点
	{
		insertAfter(search(p->data,r,p),p->data);//查找适当的位置并插入
		p=p->succ;
		remove(p->pred);//转向下一个节点
	}
}

template <typename T>
void List<T>::selectionsort(Posi(T) p,int n)//列表选择排序
{
	Posi(T) head=p->pred;
	Posi(T) tail=p;
	for(int i=0;i<n;i++)
	{
		tail=tail->succ;
	}//待排序区间（head,tail）
	while(1<n)
	{
		Posi(T) max=selectMax(head->succ,n);//找出最大者
		insertBefore(tail,remove(max));//将其移至无序区间末尾
		tail=tail->pred;
		n--;
	}
}

template <typename T>
Posi(T) List<T>::selectMax(Posi(T) p,int n)//从起始于p的n个元素中选出最大者
{
	Posi(T) max=p;
	for(Posi(T) cur=p;1<n;n--)
	{
		if(!lt((cur=cur->succ)->data,max->data))
		{
			max=cur;
		}
	}
	return max;
}

template <typename T>
void List<T>::merge(Posi(T)& p,int n,List<T>& L,Posi(T) q,int m)//有序列表的归并
{
	Posi(T) pp=p->pred;
	while(0<m)
	{
		if((0<n)&&(p->data<=q->data))
		{
			if(q==(p=p->succ))break;
			n--;
		}
		else
		{
			insertBefore(p,L.remove((q=q->succ)->pred));
			m--;
		}
	}
	p=pp->succ;
}

template <typename T>
void List<T>::mergesort(Posi(T)& p,int n)//有序列表的归并排序算法
{
	if(n<2) return;
	int m=n>>1;
	Posi(T) q=p;
	for(int i=0;i<m;i++)
	{
		q=q->succ;
	}
	mergesort(p,m);
	mergesort(q,n-m);
	merge(p,m,*this,q,n-m);//
}

#endif // LIST_H
