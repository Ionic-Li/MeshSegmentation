#include "Functions.h"

void splitString(const std::string & s, std::vector<std::string>& v, const std::string & c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

bool readObjFile(string& FileName, vector<Vertice>& Vertices, vector<Face>& Faces)
{
	cin >> FileName;
	ifstream ObjFile(FileName.c_str());//��Obj�ļ�
	if (!ObjFile)
	{
		cout << "���ļ�ʧ��" << endl;
		return false;
	}

	string Temp;
	vector<string> VecTemp;
	map<pair<int, int>, int> Edges;//���ڵõ����ڵı�

	while (getline(ObjFile, Temp))
	{
		VecTemp.clear();
		splitString(Temp, VecTemp, " ");
		if (!VecTemp.empty())
		{
			if (VecTemp[0] == "v")//���������Ƕ���
			{
				Vertice TempV;
				if (VecTemp.size() == 4)
				{
					TempV.Position[0] = stod(VecTemp[1]);
					TempV.Position[1] = stod(VecTemp[2]);
					TempV.Position[2] = stod(VecTemp[3]);
					Vertices.push_back(TempV);
				}
				else if (VecTemp.size() == 7)
				{
					TempV.Position[0] = stod(VecTemp[1]);
					TempV.Position[1] = stod(VecTemp[2]);
					TempV.Position[2] = stod(VecTemp[3]);
					TempV.ColorData.R = stoi(VecTemp[4]);
					TempV.ColorData.G = stoi(VecTemp[5]);
					TempV.ColorData.B = stoi(VecTemp[6]);
					Vertices.push_back(TempV);
				}
				else
				{
					cout << "�ļ���ʽ����" << endl;
					ObjFile.close();
					return false;
				}
			}
			else if (VecTemp[0] == "f")//������������Ƭ
			{
				Face TempFace;
				TempFace.V[0] = stoi(VecTemp[1]) - 1;
				TempFace.V[1] = stoi(VecTemp[2]) - 1;
				TempFace.V[2] = stoi(VecTemp[3]) - 1;
				//��������
				TempFace.Center[0] =
					(Vertices[TempFace.V[0]].Position[0]
						+ Vertices[TempFace.V[1]].Position[0]
						+ Vertices[TempFace.V[2]].Position[0]) / 3;
				TempFace.Center[1] =
					(Vertices[TempFace.V[0]].Position[1]
						+ Vertices[TempFace.V[1]].Position[1]
						+ Vertices[TempFace.V[2]].Position[1]) / 3;
				TempFace.Center[2] =
					(Vertices[TempFace.V[0]].Position[2]
						+ Vertices[TempFace.V[1]].Position[2]
						+ Vertices[TempFace.V[2]].Position[2]) / 3;
				//���㷨����
				TempFace.NormalVector[0] = (Vertices[TempFace.V[1]].Position[1] - Vertices[TempFace.V[0]].Position[1]) * (Vertices[TempFace.V[2]].Position[2] - Vertices[TempFace.V[1]].Position[2])
					- (Vertices[TempFace.V[2]].Position[1] - Vertices[TempFace.V[1]].Position[1]) * (Vertices[TempFace.V[1]].Position[2] - Vertices[TempFace.V[0]].Position[2]);
				TempFace.NormalVector[1] = (Vertices[TempFace.V[1]].Position[2] - Vertices[TempFace.V[0]].Position[2]) * (Vertices[TempFace.V[2]].Position[0] - Vertices[TempFace.V[1]].Position[0])
					- (Vertices[TempFace.V[2]].Position[2] - Vertices[TempFace.V[1]].Position[2]) * (Vertices[TempFace.V[1]].Position[0] - Vertices[TempFace.V[0]].Position[0]);
				TempFace.NormalVector[2] = (Vertices[TempFace.V[1]].Position[0] - Vertices[TempFace.V[0]].Position[0]) * (Vertices[TempFace.V[2]].Position[1] - Vertices[TempFace.V[1]].Position[1])
					- (Vertices[TempFace.V[2]].Position[0] - Vertices[TempFace.V[1]].Position[0]) * (Vertices[TempFace.V[1]].Position[1] - Vertices[TempFace.V[0]].Position[1]);
				double VectorLength = sqrt(pow(TempFace.NormalVector[0], 2) + pow(TempFace.NormalVector[1], 2) + pow(TempFace.NormalVector[2], 2));
				//������λ��
				TempFace.NormalVector[0] = TempFace.NormalVector[0] / VectorLength;
				TempFace.NormalVector[1] = TempFace.NormalVector[1] / VectorLength;
				TempFace.NormalVector[2] = TempFace.NormalVector[2] / VectorLength;

				//Ѱ�����ڵ���
				if (TempFace.V[0] < TempFace.V[1])
				{
					int& Temp = Edges[pair<int, int>(TempFace.V[0], TempFace.V[1])];
					if (Temp)//����ñߴ洢�������Ϣ�����¼Ϊ���ڱ�
					{
						Neighbor TempNeighbor;
						TempNeighbor.EdgeV[0] = TempFace.V[0];
						TempNeighbor.EdgeV[1] = TempFace.V[1];
						TempNeighbor.Face = Temp - 1;
						TempFace.Neighbors.push_back(TempNeighbor);
						TempNeighbor.Face = Faces.size();
						Faces[Temp - 1].Neighbors.push_back(TempNeighbor);
					}
					else
					{
						Temp = Faces.size() + 1;
					}
				}
				else
				{
					int& Temp = Edges[pair<int, int>(TempFace.V[1], TempFace.V[0])];
					if (Temp)
					{
						Neighbor TempNeighbor;
						TempNeighbor.EdgeV[0] = TempFace.V[0];
						TempNeighbor.EdgeV[1] = TempFace.V[1];
						TempNeighbor.Face = Temp - 1;
						TempFace.Neighbors.push_back(TempNeighbor);
						TempNeighbor.Face = Faces.size();
						Faces[Temp - 1].Neighbors.push_back(TempNeighbor);
					}
					else
					{
						Temp = Faces.size() + 1;
					}
				}
				if (TempFace.V[0] < TempFace.V[2])
				{
					int& Temp = Edges[pair<int, int>(TempFace.V[0], TempFace.V[2])];
					if (Temp)//����ñߴ洢�������Ϣ�����¼Ϊ���ڱ�
					{
						Neighbor TempNeighbor;
						TempNeighbor.EdgeV[0] = TempFace.V[0];
						TempNeighbor.EdgeV[1] = TempFace.V[2];
						TempNeighbor.Face = Temp - 1;
						TempFace.Neighbors.push_back(TempNeighbor);
						TempNeighbor.Face = Faces.size();
						Faces[Temp - 1].Neighbors.push_back(TempNeighbor);
					}
					else
					{
						Temp = Faces.size() + 1;
					}
				}
				else
				{
					int& Temp = Edges[pair<int, int>(TempFace.V[2], TempFace.V[0])];
					if (Temp)
					{
						Neighbor TempNeighbor;
						TempNeighbor.EdgeV[0] = TempFace.V[0];
						TempNeighbor.EdgeV[1] = TempFace.V[2];
						TempNeighbor.Face = Temp - 1;
						TempFace.Neighbors.push_back(TempNeighbor);
						TempNeighbor.Face = Faces.size();
						Faces[Temp - 1].Neighbors.push_back(TempNeighbor);
					}
					else
					{
						Temp = Faces.size() + 1;
					}
				}
				if (TempFace.V[1] < TempFace.V[2])
				{
					int& Temp = Edges[pair<int, int>(TempFace.V[1], TempFace.V[2])];
					if (Temp)//����ñߴ洢�������Ϣ�����¼Ϊ���ڱ�
					{
						Neighbor TempNeighbor;
						TempNeighbor.EdgeV[0] = TempFace.V[1];
						TempNeighbor.EdgeV[1] = TempFace.V[2];
						TempNeighbor.Face = Temp - 1;
						TempFace.Neighbors.push_back(TempNeighbor);
						TempNeighbor.Face = Faces.size();
						Faces[Temp - 1].Neighbors.push_back(TempNeighbor);
					}
					else
					{
						Temp = Faces.size() + 1;
					}
				}
				else
				{
					int& Temp = Edges[pair<int, int>(TempFace.V[2], TempFace.V[1])];
					if (Temp)
					{
						Neighbor TempNeighbor;
						TempNeighbor.EdgeV[0] = TempFace.V[1];
						TempNeighbor.EdgeV[1] = TempFace.V[2];
						TempNeighbor.Face = Temp - 1;
						TempFace.Neighbors.push_back(TempNeighbor);
						TempNeighbor.Face = Faces.size();
						Faces[Temp - 1].Neighbors.push_back(TempNeighbor);
					}
					else
					{
						Temp = Faces.size() + 1;
					}
				}

				Faces.push_back(TempFace);
			}
		}
	}

	ObjFile.close();
	return true;
}

void outputObjFile(string NewFileName, const vector<Vertice>& Vertices, const vector<Face>& Faces)
{
	ofstream OutputObj(NewFileName.c_str());
	OutputObj << "# " << Vertices.size() << " vertices, " << Faces.size() << " faces" << endl;

	unsigned int VerticesCnt = Vertices.size();
	unsigned int FacesCnt = Faces.size();

	//��������Ϣ
	for (unsigned int i = 0; i < VerticesCnt; i++)
	{
		OutputObj << "v " << Vertices[i].Position[0] << ' '
			<< Vertices[i].Position[1] << ' '
			<< Vertices[i].Position[2] << ' '
			<< Vertices[i].ColorData.R << ' '
			<< Vertices[i].ColorData.G << ' '
			<< Vertices[i].ColorData.B << endl;
	}

	//��������Ϣ
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		OutputObj << "f " << Faces[i].V[0] + 1 << ' '
			<< Faces[i].V[1] + 1 << ' '
			<< Faces[i].V[2] + 1 << endl;
	}
	OutputObj.close();
}

pair<double, double> calculateDist(vector<Vertice>& Vertices, vector<Face>& Faces)
{
	unsigned int FacesCnt = Faces.size();
	double AverageGeod = 0;
	double AverageAngleDist = 0;
	unsigned int Cnt = 0;
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		for (unsigned int j = 0; j < Faces[i].Neighbors.size(); j++)
		{
			++Cnt;
			//�����ؾ���
			int NeiFace;//���ڱߵ����
			NeiFace = Faces[i].Neighbors[j].Face;
			Coordinate Direction;//�����߷�������
			Direction[0] = Vertices[Faces[i].Neighbors[j].EdgeV[1]].Position[0] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[0];
			Direction[1] = Vertices[Faces[i].Neighbors[j].EdgeV[1]].Position[1] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[1];
			Direction[2] = Vertices[Faces[i].Neighbors[j].EdgeV[1]].Position[2] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[2];
			double DirLength;//������������
			DirLength = sqrt(pow(Direction[0], 2) + pow(Direction[1], 2) + pow(Direction[2], 2));
			Direction[0] /= DirLength;//������λ��
			Direction[1] /= DirLength;
			Direction[2] /= DirLength;
			Coordinate L1;//����1�����ĵķ�������
			L1[0] = Faces[i].Center[0] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[0];
			L1[1] = Faces[i].Center[1] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[1];
			L1[2] = Faces[i].Center[2] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[2];
			double L1Length;//L1�ĳ���
			L1Length = sqrt(pow(L1[0], 2) + pow(L1[1], 2) + pow(L1[2], 2));
			L1[0] /= L1Length;//������λ��
			L1[1] /= L1Length;
			L1[2] /= L1Length;
			Coordinate L2;//����2�����ĵķ�������
			L2[0] = Faces[NeiFace].Center[0] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[0];
			L2[1] = Faces[NeiFace].Center[1] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[1];
			L2[2] = Faces[NeiFace].Center[2] - Vertices[Faces[i].Neighbors[j].EdgeV[0]].Position[2];
			double L2Length;//L2�ĳ���
			L2Length = sqrt(pow(L2[0], 2) + pow(L2[1], 2) + pow(L2[2], 2));
			L2[0] /= L2Length;//������λ��
			L2[1] /= L2Length;
			L2[2] /= L2Length;
			double Alpha = acos(Direction[0] * L1[0] + Direction[1] * L1[1] + Direction[2] * L1[2]);//�����������������ļн�
			double Beta = acos(Direction[0] * L2[0] + Direction[1] * L2[1] + Direction[2] * L2[2]);
			//չƽ������ؾ���
			Faces[i].Neighbors[j].Geod = sqrt(pow(L1Length, 2) + pow(L2Length, 2) - 2 * L1Length * L2Length * cos(Alpha + Beta));
			AverageGeod += Faces[i].Neighbors[j].Geod;

			//����Ǿ���
			double CosAngle = Faces[i].NormalVector[0] * Faces[NeiFace].NormalVector[0] + Faces[i].NormalVector[1] * Faces[NeiFace].NormalVector[1] + Faces[i].NormalVector[2] * Faces[NeiFace].NormalVector[2];
			double CenterVec[3];//��������֮�������
			CenterVec[0] = Faces[NeiFace].Center[0] - Faces[i].Center[0];
			CenterVec[1] = Faces[NeiFace].Center[1] - Faces[i].Center[1];
			CenterVec[2] = Faces[NeiFace].Center[2] - Faces[i].Center[2];
			double VecLength = sqrt(pow(CenterVec[0], 2) + pow(CenterVec[1], 2) + pow(CenterVec[2], 2));
			CenterVec[0] /= VecLength;//������λ��
			CenterVec[1] /= VecLength;
			CenterVec[2] /= VecLength;
			double TempCos;//�����жϰ�͹�Ľǣ�������Ϊ���棬С����Ϊ͹��
			TempCos = Faces[i].NormalVector[0] * CenterVec[0] + Faces[i].NormalVector[1] * CenterVec[1] + Faces[i].NormalVector[2] * CenterVec[2];
			double Eta;
			if (TempCos < 0)
			{
				Eta = CONVEXETA;
			}
			else
			{
				Eta = 1;
			}
			Faces[i].Neighbors[j].AngDist = Eta * (1 - CosAngle);
			if (Faces[i].Neighbors[j].AngDist < 0)
				Faces[i].Neighbors[j].AngDist = 0;
			AverageAngleDist += Faces[i].Neighbors[j].AngDist;
			//cout << Angle << ' ' << AverageAngleDist << ' ' << i << endl;
		}
	}
	AverageGeod /= Cnt;
	AverageAngleDist /= Cnt;
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		for (unsigned int j = 0; j < Faces[i].Neighbors.size(); j++)
		{
			Faces[i].Neighbors[j].Weight = DELTA * (Faces[i].Neighbors[j].Geod / AverageGeod) + (1 - DELTA) * (Faces[i].Neighbors[j].AngDist / AverageAngleDist);
		}
	}
	return pair<double, double>(AverageGeod, AverageAngleDist);
}

void dijkstra(vector<Vertice>& Vertices, vector<Face>& Faces, double**& ppEdges)
{
	unsigned int FacesCnt = Faces.size();
	priority_queue<Edge> EdgeQ;
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		Edge E, Temp;
		E.Start = i;
		E.End = i;
		E.Distance = 0;
		EdgeQ.push(E);
		Faces[i].State = DISCOVERED;
		while (!EdgeQ.empty())
		{
			E = EdgeQ.top();
			EdgeQ.pop();
			if (Faces[E.End].State == VISITIED)
				continue;
			Faces[E.End].State = VISITIED;
			ppEdges[E.Start][E.End] = E.Distance;
			int EdgeCnt = Faces[E.End].Neighbors.size();
			for (int j = 0; j < EdgeCnt; j++)
			{
				Temp.Start = i;
				Temp.End = Faces[E.End].Neighbors[j].Face;
				Temp.Distance = Faces[E.End].Neighbors[j].Weight + E.Distance;
				if (Faces[Temp.End].State != VISITIED)
				{
					if (!ppEdges[Temp.Start][Temp.End])//���û�д����ʱ���룬�ͱ�����ʱ����
					{
						EdgeQ.push(Temp);
						ppEdges[Temp.Start][Temp.End] = Temp.Distance;
					}
					else if (Temp.Distance < ppEdges[Temp.Start][Temp.End])//��������ʱ���룬���ж��¾����Ƿ�С����ʱ���룬���С�������
					{
						EdgeQ.push(Temp);
						ppEdges[Temp.Start][Temp.End] = Temp.Distance;
					}
				}
			}
		}
		clearState(Faces);
	}
}

void clearState(vector<Face>& Faces)
{
	unsigned int FacesCnt = Faces.size();
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		Faces[i].State = UNDISCOVERED;
	}
}

void decideReps(vector<Face>& Faces, double **& ppEdges, vector<int>& Reps)
{
	unsigned int FacesCnt = Faces.size();
	double LastMinRepDist;//��һ�ξ�������֮�����С����
	double MaxRepDistChange;//��������֮����С����仯�����ֵ
	unsigned int RepsCnt = 0;//�仯ǰ������������
	unsigned int MaxChangeStayCnt;//�仯���ֵ���ִ����ĳ���


	double MinDist = INF;//�������ľ�������
	int InitialRep;
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		double DistSum = 0;
		for (unsigned int j = 0; j < FacesCnt; j++)
		{
			DistSum += ppEdges[i][j];
			if (DistSum > MinDist)
				break;
		}
		if (DistSum < MinDist)//�����С�ڵ�ǰ����Сֵ�����³�ʼ��������
		{
			MinDist = DistSum;
			InitialRep = i;
		}
	}
	Reps.push_back(InitialRep);

	while (true)
	{
		double MaxDist = 0;//���ľ���
		int NewRep;//�µľ�������
		for (unsigned int i = 0; i < FacesCnt; i++)
		{
			double MinDist = INF;//���������ĵ���С����
			for (unsigned int j = 0; j < Reps.size(); j++)
			{
				if (ppEdges[i][Reps[j]] < MinDist)
					MinDist = ppEdges[i][Reps[j]];
			}
			if (MinDist > MaxDist)//�����ǰ��Ƭ�����о������ĵ���С�������֮ǰ����С���룬���õ�ǰ����Ƭ����֮ǰ����Ƭ
			{
				MaxDist = MinDist;
				NewRep = i;
			}
		}

		Reps.push_back(NewRep);

		double CurRepsMinDist = INF;//��ǰ��ͬ�������ļ����С����

		for (unsigned int i = 0; i < Reps.size() - 1; i++)
		{
			if (ppEdges[Reps[i]][NewRep] < CurRepsMinDist)//�����ǰ�����������ļ�ľ����С���͸��¾�������֮�����С����
			{
				CurRepsMinDist = ppEdges[Reps[i]][NewRep];
			}
		}

		if (!RepsCnt)//���֮ǰֻ��һ���������ģ��Ͳ������ֵ
		{
			RepsCnt = 1;
			LastMinRepDist = CurRepsMinDist;
			MaxRepDistChange = 0;//�仯����Ϊ0
			MaxChangeStayCnt = 0;//ά�ִ�����Ϊ0
		}
		else
		{
			if (LastMinRepDist - CurRepsMinDist > MaxRepDistChange)//����仯���죬�͸��¾����������仯б��
			{
				MaxRepDistChange = LastMinRepDist - CurRepsMinDist;
				RepsCnt = Reps.size() - 1;
				MaxChangeStayCnt = 0;
			}
			else//����仯���������仯����
			{
				MaxChangeStayCnt++;
			}
			LastMinRepDist = CurRepsMinDist;
		}

		if (MaxChangeStayCnt >= 15)
		{
			while (Reps.size() > RepsCnt)
			{
				Reps.pop_back();//�����������������
			}
			return;
		}
	}
}

void cluster(vector<Face>& Faces, vector<int>& Reps, double**& ppEdges)
{
	unsigned int FacesCnt = Faces.size();
	vector<int> LastReps = Reps;
	vector<pair<int, double> > SmallestReps;//�¾�������
	cout << "��ʼ��������: ";
	for (unsigned int i = 0; i < Reps.size(); i++)
	{
		cout << Reps[i] + 1 << ' ';
		pair<int, double> Temp(Reps[i], 0);
		SmallestReps.push_back(Temp);
	}
	cout << endl;


	//��ʼ���¾�������
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		double SumDist = 0;;//�������������ľ��뵹����
		for (unsigned int j = 0; j < Reps.size(); j++)
		{
			if (ppEdges[i][Reps[j]] <= 1E-6)//������ֳ�0�����
				continue;
			SumDist += 1 / ppEdges[i][Reps[j]];
		}
		for (unsigned int j = 0; j < Reps.size(); j++)
		{
			if (ppEdges[i][Reps[j]] <= 1E-6)//������ֳ�0�����
			{
				Faces[i].Pl.push_back(1);
				continue;
			}
			double Temp = (1 / ppEdges[i][Reps[j]]) / SumDist;
			Faces[i].Pl.push_back(Temp);
		}
	}
	for (unsigned int j = 0; j < FacesCnt; j++)
	{
		for (unsigned int k = 0; k < Reps.size(); k++)
		{
			SmallestReps[k].second += Faces[j].Pl[k] * ppEdges[Reps[k]][j];
		}
	}
	cout << "��ʼ��Ȩ�����: ";
	for (unsigned int k = 0; k < Reps.size(); k++)
	{
		cout << SmallestReps[k].second << ' ';
	}
	cout << endl;

	int Cnt = 0;
	do
	{
		Cnt++;
		LastReps = Reps;

		for (unsigned int i = 0; i < FacesCnt; i++)
		{
			for (unsigned int k = 0; k < Reps.size(); k++)
			{
				double TempSum = 0;//��ʱ�ļ�Ȩ��
				for (unsigned int j = 0; j < FacesCnt; j++)
				{
					TempSum += Faces[j].Pl[k] * ppEdges[i][j];
					if (TempSum > SmallestReps[k].second)
						break;
				}
				//���¾�������
				if (TempSum < SmallestReps[k].second)
				{
					SmallestReps[k].first = i;
					SmallestReps[k].second = TempSum;
				}
			}
		}
		for (unsigned int j = 0; j < Reps.size(); j++)
			Reps[j] = SmallestReps[j].first;
		if (Reps == LastReps)//�����������û���ֱ������ѭ��
			break;

		cout << "��" << Cnt << "�ε���:" << endl <<
			"��������: ";
		for (unsigned int i = 0; i < Reps.size(); i++)
		{
			cout << Reps[i] << ' ';
		}
		cout << endl;
		cout << "��Ȩ�����: ";
		for (unsigned int k = 0; k < Reps.size(); k++)
		{
			cout << SmallestReps[k].second << ' ';
		}
		cout << endl;
		for (unsigned int i = 0; i < FacesCnt; i++)
		{
			double SumDist = 0;;//�������������ľ��뵹����
			for (unsigned int j = 0; j < Reps.size(); j++)
			{
				if (ppEdges[i][Reps[j]] <= 1E-6)//������ֳ�0�����
					continue;
				SumDist += 1 / ppEdges[i][Reps[j]];
			}
			for (unsigned int j = 0; j < Reps.size(); j++)
			{
				if (ppEdges[i][Reps[j]] <= 1E-6)//������ֳ�0�����
				{
					Faces[i].Pl[j] = 1;
					continue;
				}
				double Temp = (1 / ppEdges[i][Reps[j]]) / SumDist;
				Faces[i].Pl[j] = Temp;
			}
		}
	} while (Cnt < 20);
}

void Div(vector<Vertice>& Vertices, vector<Face>& Faces, vector<int>& Reps, double AverageAngleDist, double**& ppEdges, string& FileName)
{
	map<pair<int, int>, vector<int> > FuzzyIdx;
	//ģ���ָ�
	unsigned int FacesCnt = Faces.size();
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		vector<double> TempPl = Faces[i].Pl;
		sort(TempPl.rbegin(), TempPl.rend());
		double MaxP = TempPl[0];//������������
		double MaxSecP = TempPl[1];
		int R[2];
		unsigned int k = 0;
		for (unsigned int j = 0; j < Faces[i].Pl.size(); j++)//�ҵ������������ʶ�Ӧ�ľ�������
		{
			if ((Faces[i].Pl[j] <= MaxP + 1E-6 && Faces[i].Pl[j] >= MaxP - 1E-6)
				|| (Faces[i].Pl[j] <= MaxSecP + 1E-6 && Faces[i].Pl[j] >= MaxSecP - 1E-6))
			{
				R[k] = j;
				if (k == 1)
					break;
				k++;
			}
		}

		double NormFactor = Faces[i].Pl[R[0]] + Faces[i].Pl[R[1]];
		Faces[i].Pl[R[0]] /= NormFactor;//��һ��
		Faces[i].Pl[R[1]] /= NormFactor;

		if (Faces[i].Pl[R[0]] > 0.5 + EPSILON)//������ĸ��ʱȵڶ���Ĵ�һ����ֵ������Ϊ���ڵ�һ��ľ��������ڲ�����֮ͬ��
		{
			Faces[i].Kind = DETERMINED;
			Faces[i].PossibleRep.first = Reps[R[0]];
			Faces[i].PossibleRep.second = Reps[R[1]];
		}
		else if (Faces[i].Pl[R[1]] > 0.5 + EPSILON)
		{
			Faces[i].Kind = DETERMINED;
			Faces[i].PossibleRep.first = Reps[R[1]];
			Faces[i].PossibleRep.second = Reps[R[0]];
		}
		else
		{
			Faces[i].Kind = FUZZY;
			Faces[i].PossibleRep.first = Reps[R[0]];
			Faces[i].PossibleRep.second = Reps[R[1]];

			FuzzyIdx[Faces[i].PossibleRep].push_back(i);
		}
	}

	//��ɫ
	unordered_map<int, Color> NewColor;
	int ColorDiff;
	if ((Reps.size() + FuzzyIdx.size()) % 6 == 0)
		ColorDiff = 255 / (maxNum(Reps.size(), FuzzyIdx.size()) / 6);
	else
		ColorDiff = 255 / (maxNum(Reps.size(), FuzzyIdx.size()) / 6 + 1);
	for (unsigned int i = 0; i < Reps.size(); i++)
	{
		if (i % 6 == 0)
		{
			NewColor[Reps[i]] = Color((i / 6 + 1) * ColorDiff, 0, 0);
		}
		else if (i % 6 == 1)
		{
			NewColor[Reps[i]] = Color(0, (i / 6 + 1) * ColorDiff, 0);
		}
		else if(i % 6 == 2)
		{
			NewColor[Reps[i]] = Color(0, 0, (i / 6 + 1) * ColorDiff);
		}
		else if (i % 6 == 3)
		{
			NewColor[Reps[i]] = Color((i / 6 + 1) * ColorDiff, (i / 6 + 1) * ColorDiff, 0);
		}
		else if (i % 6 == 4)
		{
			NewColor[Reps[i]] = Color((i / 6 + 1) * ColorDiff, 0, (i / 6 + 1) * ColorDiff);
		}
		else
		{
			NewColor[Reps[i]] = Color(0, (i / 6 + 1) * ColorDiff, (i / 6 + 1) * ColorDiff);
		}
	}

	//ģ��������ɫ�Լ�ģ����������»���
	unsigned int FuzzyColorCnt = 0;
	map<pair<int, int>, vector<int> >::iterator It;
	for (It = FuzzyIdx.begin(); It != FuzzyIdx.end(); It++)
	{
		unsigned int FuzzyCnt = (*It).second.size();
		//��ɫ
		Color Temp;
		if (FuzzyColorCnt % 6 == 0)
			Temp = Color((FuzzyColorCnt / 6 + 1) * ColorDiff, 0, 0);
		else if (FuzzyColorCnt % 3 == 1)
			Temp = Color(0, (FuzzyColorCnt / 6 + 1) * ColorDiff, 0);
		else if (FuzzyColorCnt % 3 == 2)
			Temp = Color(0, 0, (FuzzyColorCnt / 6 + 1) * ColorDiff);
		else if (FuzzyColorCnt % 3 == 3)
			Temp = Color((FuzzyColorCnt / 6 + 1) * ColorDiff, (FuzzyColorCnt / 6 + 1) * ColorDiff, 0);
		else if (FuzzyColorCnt % 3 == 4)
			Temp = Color((FuzzyColorCnt / 6 + 1) * ColorDiff, 0, (FuzzyColorCnt / 6 + 1) * ColorDiff);
		else
			Temp = Color(0, (FuzzyColorCnt / 6 + 1) * ColorDiff, (FuzzyColorCnt / 6 + 1) * ColorDiff);

		FuzzyColorCnt++;

		for (unsigned int i = 0; i < FuzzyCnt; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
				Vertices[Faces[(*It).second[i]].V[j]].ColorData = Temp;

			//���»���ģ������
			for (unsigned int j = 0; j < Faces[(*It).second[i]].Neighbors.size(); j++)
			{
				if (Faces[Faces[(*It).second[i]].Neighbors[j].Face].Kind != FUZZY)
				{
					if (Faces[Faces[(*It).second[i]].Neighbors[j].Face].PossibleRep == Faces[(*It).second[i]].PossibleRep
						|| (Faces[Faces[(*It).second[i]].Neighbors[j].Face].PossibleRep.first == Faces[(*It).second[i]].PossibleRep.second
							&& Faces[Faces[(*It).second[i]].Neighbors[j].Face].PossibleRep.second == Faces[(*It).second[i]].PossibleRep.first))
					{
						Faces[Faces[(*It).second[i]].Neighbors[j].Face].Kind = FUZZY;
						(*It).second.push_back(Faces[(*It).second[i]].Neighbors[j].Face);//���߽�����ģ�������ཻ����ƬҲ���ģ����
					}
				}
			}
		}

	}

	outputObjFile("Mid" + FileName, Vertices, Faces);//�������ɫ

	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		if (Faces[i].Kind == DETERMINED)
		{
			for (unsigned int j = 0; j < 3; j++)
				Vertices[Faces[i].V[j]].ColorData = NewColor[Faces[i].PossibleRep.first];
		}
	}

	for (It = FuzzyIdx.begin(); It != FuzzyIdx.end(); It++)
	{
		accurateDiv(Vertices, Faces, (*It).second, AverageAngleDist, ppEdges, (*It).first.first, (*It).first.second, NewColor);
	}
}

void accurateDiv(vector<Vertice>& Vertices, vector<Face>& Faces, vector<int>& FuzzyFacesIdx, double AverageAngleDist, double**& ppEdges, int Repa, int Repb, unordered_map<int, Color>& NewColor)
{
	vector<int> ANeighbors;
	pair<int, int> CurReps(Repa, Repb);
	//����ÿ���ߵ�����
	unsigned int FuzzyFacesCnt = FuzzyFacesIdx.size();
	for (unsigned int i = 0; i < FuzzyFacesCnt; i++)
	{
		for (unsigned int j = 0; j < Faces[FuzzyFacesIdx[i]].Neighbors.size(); j++)
		{
			if (Faces[Faces[FuzzyFacesIdx[i]].Neighbors[j].Face].Kind != FUZZY)//���������A��B�������Ƭ��������Ϊ����
			{
				if (Faces[Faces[FuzzyFacesIdx[i]].Neighbors[j].Face].PossibleRep.first == Repa)//���������A������Ƭ���Ͱ���Ƭ����A�����������
				{
					ppEdges[FuzzyFacesIdx[i]][Repa] = INF;
					ppEdges[Repa][FuzzyFacesIdx[i]] = INF;
					ANeighbors.push_back(FuzzyFacesIdx[i]);
				}
				else if (Faces[Faces[FuzzyFacesIdx[i]].Neighbors[j].Face].PossibleRep.second == Repb)//���������B������Ƭ
				{
					ppEdges[FuzzyFacesIdx[i]][Repb] = INF;
					ppEdges[Repb][FuzzyFacesIdx[i]] = INF;
				}
			}
			else//����������������ݹ�ʽ����
			{
				ppEdges[FuzzyFacesIdx[i]][Faces[FuzzyFacesIdx[i]].Neighbors[j].Face] = 1 / (1 + Faces[FuzzyFacesIdx[i]].Neighbors[j].AngDist / AverageAngleDist);
			}
		}
	}

	bool PathExist = false;
	queue<int> NeighborQ;

	do
	{
		PathExist = false;
		clearState(Faces);
		clearParent(Faces, FuzzyFacesIdx);

		//�����������
		double CapMax = 0;//�������
		int EndFace = 0;
		for (unsigned int i = 0; i < ANeighbors.size(); i++)
		{
			if (ppEdges[Repa][ANeighbors[i]] <= 1E-6)//�������Ϊ0�Ͳ����
				continue;
			Faces[ANeighbors[i]].State = DISCOVERED;
			Faces[ANeighbors[i]].Parent = Repa;
			NeighborQ.push(ANeighbors[i]);//��A����ʼ���й�����ȱ���
			Faces[ANeighbors[i]].CurMaxCap = ppEdges[Repa][ANeighbors[i]];
		}
		while (!NeighborQ.empty())
		{
			int CurFace = NeighborQ.front();
			NeighborQ.pop();
			for (unsigned int j = 0; j < Faces[CurFace].Neighbors.size(); j++)
			{
				if (Faces[Faces[CurFace].Neighbors[j].Face].State == DISCOVERED)
					continue;
				if (ppEdges[CurFace][Faces[CurFace].Neighbors[j].Face] <= 1E-6)//�������һ����Ƭ������Ϊ0����һ����Ƭ�����
					continue;
				if (Faces[Faces[CurFace].Neighbors[j].Face].Kind == DETERMINED && Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.first == Repb)//�����һ����Ƭ����B�����ڣ��ҵ�ͨ·
				{
					if (Faces[CurFace].CurMaxCap > CapMax)
					{
						PathExist = true;
						CapMax = Faces[CurFace].CurMaxCap;//���������Ϊ��ǰ��Ƭ�д洢�������
						EndFace = CurFace;//��ǰ��ƬΪ����������Ƭ
					}
				}
				if (Faces[Faces[CurFace].Neighbors[j].Face].Kind == FUZZY &&
					(Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep == CurReps ||
					(Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.first == CurReps.second &&
						Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.second == CurReps.first)))//���û�е���B���򣬾���ӣ�����¼���ڵ�λ��
				{
					Faces[Faces[CurFace].Neighbors[j].Face].Parent = CurFace;
					Faces[Faces[CurFace].Neighbors[j].Face].State = DISCOVERED;
					NeighborQ.push(Faces[CurFace].Neighbors[j].Face);
					if (ppEdges[CurFace][Faces[CurFace].Neighbors[j].Face] < Faces[CurFace].CurMaxCap)//�������һ����Ƭ������С�ڵ�ǰ��Ƭ��������������õ���һ����Ƭ������Ϊ�����
					{
						Faces[Faces[CurFace].Neighbors[j].Face].CurMaxCap = ppEdges[CurFace][Faces[CurFace].Neighbors[j].Face];
					}
					else
					{
						Faces[Faces[CurFace].Neighbors[j].Face].CurMaxCap = Faces[CurFace].CurMaxCap;
					}
				}
			}
		}

		if (PathExist)
		{
			do
			{
				ppEdges[Faces[EndFace].Parent][EndFace] -= CapMax;//��ȥ��ǰͨ·�ϵ������
				//ppEdges[EndFace][Faces[EndFace].Parent] -= CapMax;
				EndFace = Faces[EndFace].Parent;//��������
			} while (EndFace != Repa);//���û�л��ݵ���㣬�ͼ�������
		}
	} while (PathExist);

	clearState(Faces);
	//��ϸ�ָ���ɫ
	for (unsigned int i = 0; i < ANeighbors.size(); i++)
	{
		NeighborQ.push(ANeighbors[i]);//��A����ʼ���й�����ȱ���
		Faces[ANeighbors[i]].Kind = DETERMINED;
		Faces[ANeighbors[i]].State = DISCOVERED;
		Faces[ANeighbors[i]].PossibleRep.first = Repa;
		Faces[ANeighbors[i]].PossibleRep.second = Repb;
	}
	while (!NeighborQ.empty())
	{
		int CurFace = NeighborQ.front();
		NeighborQ.pop();
		//��ɫ
		for (unsigned int k = 0; k < 3; k++)
			Vertices[Faces[CurFace].V[k]].ColorData = NewColor[Repa];

		for (unsigned int j = 0; j < Faces[CurFace].Neighbors.size(); j++)
		{
			if (Faces[Faces[CurFace].Neighbors[j].Face].State == DISCOVERED)
				continue;
			if (Faces[Faces[CurFace].Neighbors[j].Face].Kind != FUZZY)//�����һ����Ƭ���ڲ���ģ�������ڣ������
				continue;
			if (ppEdges[CurFace][Faces[CurFace].Neighbors[j].Face] <= 1E-6)//�������һ����Ƭ������Ϊ0����һ����Ƭ�����
				continue;
			if (Faces[Faces[CurFace].Neighbors[j].Face].Kind == FUZZY &&
				(Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep == CurReps ||
				(Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.first == CurReps.second &&
					Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.second == CurReps.first)))//���û�е���ֽ����򣬾����
			{
				Faces[Faces[CurFace].Neighbors[j].Face].Kind = DETERMINED;
				Faces[Faces[CurFace].Neighbors[j].Face].State = DISCOVERED;
				Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.first = Repa;
				Faces[Faces[CurFace].Neighbors[j].Face].PossibleRep.second = Repb;
				NeighborQ.push(Faces[CurFace].Neighbors[j].Face);
			}
		}
	}
	for (unsigned int i = 0; i < FuzzyFacesIdx.size(); i++)
	{
		if (Faces[FuzzyFacesIdx[i]].Kind == FUZZY)
		{
			Faces[FuzzyFacesIdx[i]].Kind = DETERMINED;
			Faces[FuzzyFacesIdx[i]].PossibleRep.first = Repb;
			Faces[FuzzyFacesIdx[i]].PossibleRep.second = Repa;
			//��ɫ
			for (unsigned int k = 0; k < 3; k++)
				Vertices[Faces[FuzzyFacesIdx[i]].V[k]].ColorData = NewColor[Repb];
		}
	}
}

void clearParent(vector<Face>& Faces, vector<int>& FacesIdx)
{
	unsigned int FacesCnt = FacesIdx.size();
	for (unsigned int i = 0; i < FacesCnt; i++)
	{
		Faces[FacesIdx[i]].Parent = -1;
		Faces[FacesIdx[i]].CurMaxCap = 0;//��ʼ�������Ϊ0
	}
}
