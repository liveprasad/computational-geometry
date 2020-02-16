#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string.h>

#include "Points.h"
#include "DCEL.h"


using namespace std;
//extern enum color {BLACK,GRAY};

class Line{
	public:

		int id;
		Point *left;
		Point *right;
		Point *helper;
		int col;
		vector<int> adjLines;

		Line(){
			helper = NULL;
		}

		Line(Point *p1, Point *p2,int id){
			this->left = p1;
			this->right = p2;
			this->id = id;
			col = 0;
		}
		bool equals(Line *l){

			if(this->id == l->id)
				return true;
			else
				return false;
		}

		// @ param : line instance to be inserted into status queue will be cheked against current node line
		// Returns true when this line instance is on the upper side of the l line instance passed

		bool compare(Line *l){

			    if(orientation(*l->left,*this->left,*l->right) == 0 && orientation(*l->left,*this->right,*l->right) == 2 )
								return true;
				if(orientation(*l->left,*this->left,*l->right) == 2 && orientation(*l->left,*this->right,*l->right) == 1 && !doIntersect(*this->left,*this->right,*l->left,*l->right)  ) {
                    return !l->compare(this);
				}

				if(orientation(*l->left,*this->left,*l->right) == 1 && orientation(*l->left,*this->right,*l->right) == 2 && !doIntersect(*this->left,*this->right,*l->left,*l->right)  ) {
                    return !l->compare(this);
				}


				if(orientation(*l->left,*this->left,*l->right) == 2){
					return true;
				}
				else
					return false;

		}

		int orientation(Point p, Point q, Point r)
		{

		    int val = (q.y - p.y) * (r.x - q.x) -
		              (q.x - p.x) * (r.y - q.y);

		    if (val == 0) return 0;  // colinear

		    return (val > 0)? 1: 2; // clock or counterclock wise
		}

		void copyLine(Line *l){
			this->left = new Point(l->left->x,l->left->y);
			this->right = new Point(l->right->x,l->right->y);
			this->id = l->id;
		}

		bool onSegment(Point p, Point q, Point r)
		{
		    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		       return true;

		    return false;
		}

		bool dointersect(Line *l){
			return doIntersect(*this->left,*this->right,*l->left,*l->right);
		}

		bool doIntersect(Point p1, Point q1, Point p2, Point q2)
		{
		    // Find the four orientations needed for general and
		    // special cases
		    int o1 = orientation(p1, q1, p2);
		    int o2 = orientation(p1, q1, q2);
		    int o3 = orientation(p2, q2, p1);
		    int o4 = orientation(p2, q2, q1);

		    // General case
		    if (o1 != o2 && o3 != o4)
		        return true;

		    // Special Cases
		    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
		    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

		    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
		    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

		    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
		    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

		     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
		    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

		    return false; // Doesn't fall in any of the above cases
		}
};



#endif // LINE_H_INCLUDED
