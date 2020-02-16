#ifndef DCEL_H_INCLUDED
#define DCEL_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;




class Edge {
	public:

		int  originId;
		Edge *twinsId;
		Edge *nextId;
		Edge *prevId;
		int faceId;
		Edge(){
			
		}

		Edge(int origin,Edge* prevId,int faceId){
					this->originId = origin;
					this->twinsId = NULL;
					this->nextId  = NULL;
					this->prevId  = prevId;
					this->faceId  = faceId;
		}

};


class Face{
	public:
		Edge 	*edgeId;
		int  	ownId;
		bool 	isColored;
		Face(Edge *e,int ownId){
			edgeId = e;
			this->ownId  = ownId;
			this->isColored = false;
		}
};


class Vertex{
	public:
		int ownId;
		Edge *edgeId;
		double x;
		double y;
		int    col;
		bool   isUpper;
		Vertex(){
			col = -1;
		}
};

class DCEL{
	public:

		DCEL(){

	    }
		vector<Vertex> vertexList;
		vector<Edge * > EdgeList;
		vector<Face  > FaceList;
		void addvertex(Vertex p);
		void addFace(Face  f);
		void addEdge(Vertex start,Vertex end,int Face1 , int Face2);
		vector<int> getEdges(Vertex *v);

};

#endif // DCEL_H_INCLUDED

