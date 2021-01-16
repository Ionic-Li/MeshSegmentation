#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <queue>
#include <functional>
#include <algorithm>
#include "StructsAndConsts.h"
#include "Functions.h"
using namespace std;


int main(int argc, char* argv[])
{
	string FileName;//Obj�ļ�������
	vector<Vertice> Vertices;//���ж���
	vector<Face> Faces;//������
	double** ppEdges;//������������ľ��󣬾�ȷ�ָ�ʱΪ����
	pair<double, double> AverageDist;//��һ����AverageGeod���ڶ�����AverageAngleDist
	vector<int> Reps;//��������
	//��ȡ�ļ�
	if (!readObjFile(FileName, Vertices, Faces))
		return 0;

	unsigned int FacesCnt = Faces.size();//
	ppEdges = new double*[FacesCnt];
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		ppEdges[i] = new double[FacesCnt];
	}

	AverageDist = calculateDist(Vertices, Faces);
	dijkstra(Vertices, Faces, ppEdges);//�õ���ʼ��������

	decideReps(Faces, ppEdges, Reps);

	cluster(Faces, Reps, ppEdges);//���о������
	Div(Vertices, Faces, Reps, AverageDist.second, ppEdges, FileName);

	outputObjFile("New" + FileName, Vertices, Faces);

	for (unsigned int i = 0; i < FacesCnt; i++)//����ߵľ���
	{
		delete[] ppEdges[i];
	}
	delete ppEdges;
	return 0;
}
