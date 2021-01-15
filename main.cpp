#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <queue>
#include <functional>
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
	Edge LargestEdge;//��ľ���
	vector<int> FuzzyFacesIdx;//ģ������
	int Repa;//��������
	int Repb;
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
	LargestEdge = dijkstra(Vertices, Faces, ppEdges);
	Repa = LargestEdge.Start;//�õ���ʼ��������
	Repb = LargestEdge.End;
	cluster(Faces, Repa, Repb, ppEdges);//���о������

	fuzzyDiv(Vertices, Faces, FuzzyFacesIdx);

	outputObjFile("Mid" + FileName, Vertices, Faces);

	accurateDiv(Vertices, Faces, FuzzyFacesIdx, AverageDist.second, ppEdges, Repa, Repb);

	outputObjFile("New" + FileName, Vertices, Faces);

	for (unsigned int i = 0; i < FacesCnt; i++)//����ߵľ���
	{
		delete[] ppEdges[i];
	}
	delete ppEdges;
	return 0;
}
