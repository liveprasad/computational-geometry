#ifndef POINTS_H_INCLUDED
#define POINTS_H_INCLUDED

#include <cmath>
#include <algorithm>
#include <vector>
#include "DCEL.h"

#define EPSILON 0.0001

using namespace std;
class Point{

	public:
		int id;
		double x;
		double y;
		bool isMerge;
		int leftline;
		int rightline;
		Point(){}
		Point(double x, double y){
			this->x = x;
			this->y = y;
		}

		Point(double x, double y,int id){
			this->x = x;
			this->y = y;
			this->id = id;
		}

		//member function declaration
        void add(Point *p){
        	this->x += p->x;
			this->y += p->y;
		}
		bool compare(Point *p){
			if(this->x < p->x)
				return true;
			else if(abs(p->x -this->x)< EPSILON )
			{
				if(this->y < p->y)
					return true;
				else
					return false;
			}
			else
				return false;
		}

		void copyVertex(Vertex V){

			this->x = V.x;
			this->y = V.y;
			this->id = V.ownId;
		}



		bool equals(Point *p){

			if(this->id == p->id)
				return true;
			else
				return false;			

		}



};




#endif // POINTS_H_INCLUDED
