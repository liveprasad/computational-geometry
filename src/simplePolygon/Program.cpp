
#include<stdio.h>
#include <stdlib.h>

#include "SimplePoly.h"

int main(int argc, char **argv) {

      FILE *fp = fopen("input3","r");
      SimplePoly simplePoly;
      vector<Vertex> vector1;
      int num;
      cin>>num;
      Point minP0,P1;
      int min = 0;
      for (int var = 0; var < num; ++var) {
    	  Vertex v;

    	  fscanf(fp,"%lf %lf\n",&v.x,&v.y );
    	  P1.copyVertex(v);
    	  if(var == 0)
    	      minP0 = P1;
    	  else if(P1.compare(&minP0)){
    		  minP0 = P1;
    	      min = var;
    	  }
    	  v.ownId = var;
    	  vector1.push_back(v);
      }
      if(min == 0)
    	  simplePoly.polyPoints = vector1;
      else
		  for (int var = 0; var < num; ++var) {

			  simplePoly.polyPoints.push_back(vector1[min]);
			  simplePoly.polyPoints[var].ownId = var;
			  min++;
			  min = min%num;
		  }
      simplePoly.CreateDCEL();
      cout<<simplePoly.calAngle(Point(10,10),Point(20,20),Point(30,40))<<endl;
      //simplePoly.triangulateMonotone(simplePoly.polyPoints);
      if(!simplePoly.checkPoly()){
    	  cout << "Polygon is not simple";
    	  return 0;
      }
      simplePoly.divideInMonotone();
      simplePoly.writesvg("out1.svg", 0);
      simplePoly.tringulatePolyFaces();
      simplePoly.ColorPloyDFS();
      simplePoly.writesvg("out.svg", 1);
      cout << "Least color is "<< simplePoly.coll[simplePoly.minColor]<<endl;
      cout << "TotalGaurds = "<< simplePoly.totalGaurd;
}



