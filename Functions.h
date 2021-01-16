//��������
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <queue>
#include <functional>
#include "StructsAndConsts.h"
using namespace std;

//�ָ��ַ����ĺ���
void splitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
//��ȡobj�ļ�
bool readObjFile(string& FileName, vector<Vertice>& Vertices, vector<Face>& Faces);
//���obj�ļ�
void outputObjFile(string NewFileName, const vector<Vertice>& Vertices, const vector<Face>& Faces);
//�����ؾ���ͽǾ���
pair<double, double> calculateDist(vector<Vertice>& Vertices, vector<Face>& Faces);
//��������������Ƭ��ľ���
Edge dijkstra(vector<Vertice>& Vertices, vector<Face>& Faces, double**& ppEdges);
//������б��
void clearState(vector<Face>& Faces);
//����
void cluster(vector<Face>& Faces, int& Repa, int& Repb, double**& ppEdges);
//ģ������
void fuzzyDiv(vector<Vertice>& Vertices, vector<Face>& Faces, vector<int>& FuzzyFacesIdx);
//��ϸ���֣�������������л���
void accurateDiv(vector<Vertice>& Vertices, vector<Face>& Faces, vector<int>& FuzzyFacesIdx, double AverageAngleDist, double**& ppEdges, int Repa, int Repb);
//���������Ƭ���
void clearParent(vector<Face>& Faces, vector<int>& FacesIdx);
//�����ֵ
inline double absd(double a)
{
	if (a > 0)
		return a;
	else
		return -a;
}


#endif
