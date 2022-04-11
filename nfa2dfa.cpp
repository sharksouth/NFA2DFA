#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<set>
#include<queue>
#include<stdio.h>
#include<stdlib.h> 
#define MaxVertenNum 12
#define MAX 20
using namespace std;

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
graph NFA;

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
graph2 DFA;

//图的初始化
void init(graph& g)
{
	for (int i = 0; i < MaxVertenNum; i++)
	{
		for (int j = 0; j < MaxVertenNum; j++)
		{
			g.Edge[i][j] = "*";//用*来初始化顶点的连接关系
		}
	}
	g.vexnum = 0;
	g.arcnum = 0;
}

//输入，制图
void input()
{
	int zt, zifu;//状态数和改变状态的字符数
	int count = 0;
	char ch[MAX];
	string str;
	cin >> zt;

	NFA.vexnum = zt;
	DFA.vexnum = zt;

	cin >> zifu;
	DFA.zm = zifu;

	int tempf = 0;
	cin >> NFA.fnum;
	for (int i = 0; i < NFA.fnum; i++)
	{
		cin >> tempf;
		NFA.F.insert(tempf);
	}
	for (int i = 0; i < zifu; i++)
	{
		cin >> ch[i];
		NFA.trch.insert(ch[i]);
	}
	ch[zifu] = '#'; //以#代表空串                                                 

	for (int i = 0; i < zt; i++)
	{
		cin >> NFA.Vex[i];
		if (i == 0)NFA.startnode = NFA.Vex[i];
		if (i == zt - 1)NFA.endnode = NFA.Vex[i];
	}
	int q = 0;





	for (int i = 0; i <= zt; i++)                                    //制图
	{
		int l = 0;
		cin.sync();
		getline(cin, str);
		for (l = 0; l < zt; l++)
		{
			if (NFA.Vex[l] == str[0] - '0')break;
		}
		if (str.size() > 1)
		{
			int j = 0, k, count1 = 0;
			while (j < str.size() - 1)
			{
				while (str[j] != '{')j++;
				j++;
				while (str[j] != '}')
				{
					if (str[j] != ',')
					{
						for (k = 0; k < zt; k++)
						{
							if (NFA.Vex[k] == str[j] - '0')break;
						}
						if (NFA.Edge[l][k] == "*")//将初始化时的*删掉换成字符
						{
							NFA.Edge[l][k] = ch[count1];
						}
						else
						{
							NFA.Edge[l][k] = NFA.Edge[l][k] + ',' + ch[count1];//一个状态可能可以通过多个不同的字符转到另一个状态，用“，”连接多个字符
						}
					}
					j++;
				}
				count1++;
			}
		}
	}

}


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

//合并两个set
set<int> addset(const set<int>& s1, const set<int>& s2)
{
	set<int> temp(s1.begin(), s1.end());
	temp.insert(s2.begin(), s2.end());
	return temp;
}

//判断两个set是不是一样
bool equal_set(const set<int>& s1, const set<int>& s2)
{
	// 大小不一样的集合必然不同
	if (s1.size() != s2.size())
	{
		return false;
	}

	set<int> temp;
	temp = addset(s1, s2);

	if (temp.size() != s1.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}

vector<set<int>> D; //创建D 用来存放DFA的各种状态 判断重复
queue<set<int>> S; //创建队列S 用来存放需要处理的转换状态
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


void output(graph2& g)
{
	cout << "状态表Q与原状态对应为：" << endl;
	int tt = D.size();
	for (int i = 1; i < tt; i++)
	{
		cout << "q" << i - 1 << "->";
		set<int>::iterator j;
		int k = 0;
		for (j = D[i].begin(); j != D[i].end(); j++)
		{

			int si = D[i].size() - 1;
			cout << *j;
			if (k < si)
			{
				cout << ",";
				k++;
			}

		}

		cout << endl;

	}
	cout << endl;

	cout << "字母表T为：" << endl;
	set<char>::iterator mm;
	for (mm = NFA.trch.begin(); mm != NFA.trch.end(); mm++)
	{
		cout << *mm << " ";
	}
	cout << endl << endl;

	cout << "转换函数为：" << endl;
	int sizei = DFA.start.size();
	for (int i = 0; i < sizei; i++)
	{

		cout << "q";
		cout << DFA.start.front();
		DFA.start.pop();
		cout << " ---";
		cout << DFA.l.front();
		DFA.l.pop();
		cout << "--> ";
		if (DFA.end.front() == -1)
			cout << "$";
		else
		{
			cout << "q";
			cout << DFA.end.front();
		}
		DFA.end.pop();
		cout << endl;
	}
	cout << endl;

	cout << "起始节点为：" << endl;
	cout << "q0" << endl << endl;


	cout << "结束节点F为：" << endl;

	set<int> tempff;
	tempff.insert(0);
	set<int> Fch = eclosure(tempff, NFA, '#');//求状态0的对应#形态闭包
	set<int>::iterator te;
	for (te = Fch.begin(); te != Fch.end(); te++)
	{

		int tt = D.size();
		for (int i = 1; i < tt; i++)
		{
			if (D[i].count(*te))
			{
				DFA.F.insert(i - 1);
			}

		}
	}
	set<int>::iterator tem;
	for (tem = DFA.F.begin(); tem != DFA.F.end(); tem++)
		cout << *tem << " ";


}

int main()
{
	//FILE* stream;
	//freopen("in.txt", "r", stdin);
	//freopen("out.txt","r",stdout);
	init(NFA);
	input();
	NFA2DFA(NFA);
	output(DFA);
	getchar();
	getchar();
	//fclose(stdin);
	//fclose(stdout); 
	return 0;
}
