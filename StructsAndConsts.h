//�ṹ�塢������ö�����͵Ķ���
#ifndef STRUCTSANDCONSTS_H
#define STRUCTSANDCONSTS_H

#include <vector>
using namespace std;

typedef double Coordinate[3];

const double DELTA = 0.6;//��ʼ�����ӵļ�Ȩ����
const double CONVEXETA = 0.1;//�����Etaֵ
const double INF = 999999;//����
const double EPSILON = 0.1;//ģ���ָ����ֵ

enum FaceState
{
	UNDISCOVERED, DISCOVERED, VISITIED
};

enum FaceClass
{
	A, B, FUZZY
};

struct Edge
{
	int Start;
	int End;
	double Distance;
	friend bool operator < (const Edge& A, const Edge& B);
};

struct Neighbor
{
	int EdgeV[2];
	int Face;
	double Geod;//��ؾ���
	double AngDist;//�Ǿ���
	double Weight;//Ȩ��
};

struct Face
{
	int V[3];//��������������
	Coordinate Center;
	vector<Neighbor> Neighbors;
	double NormalVector[3];
	FaceState State;
	double Pa;//�������������еĸ���
	double Pb;
	FaceClass Kind;//��Ƭ������
	int Parent;//������Ƭ��ţ������������������������
	double CurMaxCap;//���ﵱǰ�ڵ�������
	Face();
};

struct Color
{
	int R;
	int G;
	int B;
};

struct Vertice
{
	Coordinate Position;
	Color ColorData;
	Vertice();
};


#endif
