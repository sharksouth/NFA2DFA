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
	int Vex[MaxVertenNum];					 //�����
	string Edge[MaxVertenNum][MaxVertenNum]; //ת�ƾ���
	set <char> trch;						 //ת����a,b,c,...
	int vexnum, arcnum;						 //ͼ�ĵ�ǰ�������ͻ���
	int startnode;					         //��ʼ�ڵ�
	int endnode;							 //���һ���ڵ�
	int fnum;								 //��ֹ״̬����
	set <int> F;							 //��ֹ״̬����
};
graph NFA;

struct graph2
{
	int q;									 //ͼ�ĵ�ǰ״̬��
	int zm;									 //��ĸ�ĸ���
	int vexnum, arcnum;						 //ͼ�ĵ�ǰ�������ͻ���
	string Edge[4096][MAX];					 //ת�ƾ���
	queue<int> start;						 //״̬ת����ʼ
	queue<int> end;							 //״̬ת�ƽ���
	queue<char> l;							 //start->end�еļ���
	int fnum;								 //��ֹ״̬����
	set <int> F;							 //��ֹ״̬����

};
graph2 DFA;

//ͼ�ĳ�ʼ��
void init(graph& g)
{
	for (int i = 0; i < MaxVertenNum; i++)
	{
		for (int j = 0; j < MaxVertenNum; j++)
		{
			g.Edge[i][j] = "*";//��*����ʼ����������ӹ�ϵ
		}
	}
	g.vexnum = 0;
	g.arcnum = 0;
}

//���룬��ͼ
void input()
{
	int zt, zifu;//״̬���͸ı�״̬���ַ���
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
	ch[zifu] = '#'; //��#����մ�                                                 

	for (int i = 0; i < zt; i++)
	{
		cin >> NFA.Vex[i];
		if (i == 0)NFA.startnode = NFA.Vex[i];
		if (i == zt - 1)NFA.endnode = NFA.Vex[i];
	}
	int q = 0;





	for (int i = 0; i <= zt; i++)                                    //��ͼ
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
						if (NFA.Edge[l][k] == "*")//����ʼ��ʱ��*ɾ�������ַ�
						{
							NFA.Edge[l][k] = ch[count1];
						}
						else
						{
							NFA.Edge[l][k] = NFA.Edge[l][k] + ',' + ch[count1];//һ��״̬���ܿ���ͨ�������ͬ���ַ�ת����һ��״̬���á��������Ӷ���ַ�
						}
					}
					j++;
				}
				count1++;
			}
		}
	}

}


set<int>  eclosure(set<int> N, graph g, char ch)                    //��e�հ����Ӽ�
{
	set<int>U = {};//U���淵�صļ���
	if (ch == '#')U = N;//����󼯺ϵ�e�հ�����Ҫ��ԭ���ϵ�״̬���뷵�ؼ����У�������
	queue<int> Q;
	set<int>::iterator it = N.begin();//����Q����ԭ���ϵ�״̬
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
		for (i = 0; i < g.vexnum; i++)//ȷ��aΪDFA�е���һ��״̬
		{
			if (a == g.Vex[i])break;
		}
		for (int j = 0; j < g.vexnum; j++)
		{
			int flag = 0;
			for (int k = 0; k < g.Edge[i][j].size(); k++)
			{
				if (g.Edge[i][j][k] == ch)//����״̬ת����
				{
					set<int>::iterator I = U.begin();
					for (; I != U.end(); I++)//���ҷ��ؼ���U�������ظ�״̬
					{
						if (g.Vex[j] == *I)flag = 1;
					}
					if (flag == 0)//û�оͲ���״̬a��e�հ����뷵�ؼ���U
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

//�ϲ�����set
set<int> addset(const set<int>& s1, const set<int>& s2)
{
	set<int> temp(s1.begin(), s1.end());
	temp.insert(s2.begin(), s2.end());
	return temp;
}

//�ж�����set�ǲ���һ��
bool equal_set(const set<int>& s1, const set<int>& s2)
{
	// ��С��һ���ļ��ϱ�Ȼ��ͬ
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

vector<set<int>> D; //����D �������DFA�ĸ���״̬ �ж��ظ�
queue<set<int>> S; //��������S ���������Ҫ�����ת��״̬
void NFA2DFA(graph& g)
{


	queue<set<int>> ans;//�������϶���
	set<int> a;
	a.insert(g.startnode);
	set<int> U = eclosure(a, g, '#');//��״̬0��e�հ�
	D.push_back(a);
	
	S.push(U);//�������������
	D.push_back(U);//�������ض���

	int dfaq = 0;//����״̬��
	int dfan = dfaq;//�´�״̬��
	while (!S.empty())
	{
		set<int> top = S.front();
		S.pop();//ȡ��������������

		dfan = dfaq;//�´�״̬��

		//��ÿһ��ת���ַ�a,b,c,d...����
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
	cout << "״̬��Q��ԭ״̬��ӦΪ��" << endl;
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

	cout << "��ĸ��TΪ��" << endl;
	set<char>::iterator mm;
	for (mm = NFA.trch.begin(); mm != NFA.trch.end(); mm++)
	{
		cout << *mm << " ";
	}
	cout << endl << endl;

	cout << "ת������Ϊ��" << endl;
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

	cout << "��ʼ�ڵ�Ϊ��" << endl;
	cout << "q0" << endl << endl;


	cout << "�����ڵ�FΪ��" << endl;

	set<int> tempff;
	tempff.insert(0);
	set<int> Fch = eclosure(tempff, NFA, '#');//��״̬0�Ķ�Ӧ#��̬�հ�
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
