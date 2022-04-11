# BUPT形式语言与自动机实验

## NFA到DFA的转化

### 小组成员



###  程序分工



### 实验环境

​	DEV C++

​	C++ 11标准

### 开发语言

​	C/C++

### 实验任务

编程实现确定有限状态自动机（DFA）到不确定有限状态自动机（NFA）的转化，并理解不同自动机的转化过程。

### 核心算法

​	子集构造法

### 设计思路

1. NFA、DFA采用状态转移表形式输入，通过图的邻接矩阵存储自动机。
2. 使用图的深度遍历求取e-闭包
3. 通过**子集构造法**，从开始节点的e-闭包开始，求取转换结果，并将新的节点加入状态集合Q中，重复求取转换结果，直到不再产生新节点
4. 经过合并化简得到DFA

### 数据结构

NFA的储存

```c++
struct graph
{
	int Vex[MaxVertenNum];					 //顶点表
	string Edge[MaxVertenNum][MaxVertenNum]; //转移矩阵
	set <char> trch;						 //转换表a,b,c,...
	int vexnum, arcnum;						 //图的当前顶点数和弧数
	int startnode;					         //开始节点
	int endnode;							 //最后一个节点
	int fnum;								 //终止状态个数
	set <int> F;							 //终止状态集合
};
```



DFA的储存

```c++
struct graph2
{
	int q;									 //图的当前状态数
	int zm;									 //字母的个数
	int vexnum, arcnum;						 //图的当前顶点数和弧数
	string Edge[4096][MAX];					 //转移矩阵
	queue<int> start;						 //状态转移起始
	queue<int> end;							 //状态转移结束
	queue<char> l;							 //start->end中的集合
	int fnum;								 //终止状态个数
	set <int> F;							 //终止状态集合

};
```



### 输入格式

输入样例

```c
3									//状态数
2									//字母数
1									//中止状态个数
2									//中止状态编号 有多个用空格隔开
a b									//状态转移条件 有多个用空格隔开 只允许输入26个字母大小写
0 1 2								//状态编号 只允许按自然数顺序排序 最多12个 默认0为初始状态
0 {1} {} {2}						//从左到右为 状态编号 {输入第一个字符转移的状态集合}..
1 {2} {1,2} {}						//最后一个集合为通过空串转移的集合
2 {0} {} {}							//如果转移后状态为空集 表示为{}

    
    
```

### 输入范围

```
状态数 Q <= 12
字母表 T <= 52 26个字母大小写

```



### 核心算法

1. 求e-闭包算法

   参数为一个集合N（求子集的状态集合或者求e闭包的状态集合），图g（DFA）和字符ch（状态集合N可识别字符ch，空串用‘#’代替），将原状态集合N里的状态传入队列Q中，并将Q中的状态采用深度遍历将通过字符ch可转到的状态并入返回集合U中，并且将转到的状态的e闭包同时传入返回集合中，最后返回集合就是原集合通过识别字符ch可达到的新状态的集合

   ```c++
   set<int>  eclosure(set<int> N, graph g, char ch)                    //求e闭包和子集
   {
   	set<int>U = {};//U储存返回的集合
   	if (ch == '#')U = N;//如果求集合的e闭包，则要将原集合的状态存入返回集合中，否则不用
   	queue<int> Q;
   	set<int>::iterator it = N.begin();//队列Q储存原集合的状态
   	while (it != N.end())
   	{
   		Q.push(*it);
   		it++;
   	}
   	int a;
   	while (!Q.empty())
   	{
   		a = Q.front();
   		int i;
   		for (i = 0; i < g.vexnum; i++)//确认a为DFA中的哪一个状态
   		{
   			if (a == g.Vex[i])break;
   		}
   		for (int j = 0; j < g.vexnum; j++)
   		{
   			int flag = 0;
   			for (int k = 0; k < g.Edge[i][j].size(); k++)
   			{
   				if (g.Edge[i][j][k] == ch)//根据状态转换表
   				{
   					set<int>::iterator I = U.begin();
   					for (; I != U.end(); I++)//查找返回集合U中有无重复状态
   					{
   						if (g.Vex[j] == *I)flag = 1;
   					}
   					if (flag == 0)//没有就查找状态a的e闭包并入返回集合U
   					{
   						set<int> T;
   						T.insert(g.Vex[j]);
   						T = eclosure(T, g, '#');
   						U.insert(T.begin(), T.end());
   					}
   				}
   			}
   		}
   		Q.pop();
   	}
   	return U;
   }
   ```

   

2.子集构造法

​	将待操作的状态存在队列S中，每次取出队列首S，并将转移得到的新状态放在S中，直到S为空。同时保存转移时的起始、中止和转移条件，为输出做准备。

```c++
void NFA2DFA(graph& g)
{


	queue<set<int>> ans;//创建集合队列
	set<int> a;
	a.insert(g.startnode);
	set<int> U = eclosure(a, g, '#');//求状态0的e闭包
	D.push_back(a);
	
	S.push(U);//加入待操作队列
	D.push_back(U);//加入判重队列

	int dfaq = 0;//现在状态号
	int dfan = dfaq;//下次状态号
	while (!S.empty())
	{
		set<int> top = S.front();
		S.pop();//取出待操作队列首

		dfan = dfaq;//下次状态号

		//对每一个转换字符a,b,c,d...求结果
		set<char>::iterator i;
		int k = 0;
		for (i = g.trch.begin(); i != g.trch.end(); i++)
		{

			set<int> temp;
			int flag = 0;
			temp = eclosure(top, g, *i);
			if (temp.size() != 0)
			{
				int ddd = 0;
				vector<set<int>>::iterator d;
				for (d = D.begin(); d != D.end(); d++)
				{

					if (equal_set(*d, temp))
					{

						flag = 1;
						DFA.start.push(dfaq);
						DFA.end.push(ddd);
						DFA.l.push(*i);
						break;
					}
					ddd++;
				}

				if (flag == 0)
				{
					S.push(temp);
					D.push_back(temp);

					DFA.q++;
					DFA.start.push(dfaq);
					dfan++;
					DFA.end.push(dfan);
					DFA.l.push(*i);
				}

			}
			else
			{

				DFA.start.push(dfaq);
				DFA.end.push(-1);
				DFA.l.push(*i);

			}
			ans.push(temp);
			k++;
		
		}
		dfaq++;
		
	}
}
```



### 实验结果

样例输入1

```
3
2
1
2
a b
0 1 2
0 {1} {} {2}
1 {2} {1,2} {}
2 {0} {} {}

```

样例输出1

![image-20220409232740839](https://blogpic-1307415520.cos.ap-shanghai.myqcloud.com/blogimg/image-20220409232740839.png)



样例输入2

```
4
2
1
3
a b
0 1 2 3
0 {0,1} {0} {}
1 {2} {2} {}
2 {3} {} {}
3 {3} {3} {}
```

样例输出2

![image-20220409233001270](https://blogpic-1307415520.cos.ap-shanghai.myqcloud.com/blogimg/image-20220409233001270.png)



### 实验结论和收获

1. 完成了NFA-DFA的转化，并考虑了有空串的情况，通过实验详细理解了有限自动机的原理，熟悉了通过子集构造法将NFA转换为对应DFA的过程。
2. 通过对e闭包函数和求子集函数的设计，复习了图的深度遍历算法以及迭代器的用法，在函数里有个查重的过程运用了迭代器遍历集合，但其实直接用set.count()查重可能更快，代码更少。之后需要学习更多STL的常用函数以达到减少代码量和思考量的效果；
3. 运用腾讯会议进行团队合作，线上一起讨论交流。

### 可以改进的部分

1. 扩大输入范围，做错误输入提示，增加鲁棒性
2. 使用set.count()等库函数，减少代码量，增加可读性





