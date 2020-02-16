#ifndef SIMPLEPOLY_H_INCLUDED
#define SIMPLEPOLY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "DCEL.h"
#include <stdbool.h>
#include "StatusDs.h"
#include "Line.h"
#include <math.h>
#include <stack>
#include <fstream>

class SimplePoly{
	public:
	vector<Vertex> polyPoints;
	vector<Point> points;
	vector<Line>  Lines;
	vector<Point> dummyPoints;
	vector<int>  curFaceIdList;
	string coll[3]={"red","green","blue"};
	int colorValues[3]={1,1,1};
	int minColor;
	int totalGaurd;
	DCEL dcel;
	SimplePoly(){
		totalGaurd = 0;
		minColor   = 0;
	}
	bool checkPoly();
	bool anytwoIntersect();
	void CreateDCEL();
	void sortPoly();
	static bool comparePoints(Point p1,Point p2);
	void addEdge(Vertex start,Vertex end,int Face1 , int Face2);
	void addDiagonal(Vertex &start, Vertex &end);
	void divideInMonotone();
	double calAngle(Point P1,Point P0,Point P2);
	void HandleEndv(Vertex v);
	void relaxEdge(Vertex &v, Line &e);
	bool rightDelete(StatusDs &status_DS,Line *right);
	bool leftInsert(StatusDs &status_DS,Line *left);
	void triangulateMonotone(vector<Vertex> vertices);
	bool compareVertex(Vertex p1,Vertex p2);
	void writesvg(string filename,int status);
	void tringulatePolyFaces();
	void ColorPloyDFS();
};

#endif //SIMPLEPOLY_H_INCLUDED
