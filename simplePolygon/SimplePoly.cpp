#include "SimplePoly.h"




//Check if the given set of Points form a Simple polygon or not
bool SimplePoly::checkPoly(){
	return !this->anytwoIntersect();
}

bool SimplePoly::anytwoIntersect(){
    
    /*One time cost of sorting for both*/

     int size = this->polyPoints.size();
     for (int i = 0; i < size; ++i)
     {
     	Point P;
     	P.copyVertex(polyPoints[i]);
     	points.push_back(P);
     	dummyPoints.push_back(P);
     }
     for (int i = 0; i < size; ++i)
     {
     	Line l;
     	Point *P0 = &points[i];
     	Point *P1 = &points[(i+1)%size];
     	if(P0->compare(P1))
     		l = Line(P0,P1,i);
     	else
     		l = Line(P1,P0,i);
     	Lines.push_back(l);

     }

     sort(dummyPoints.begin(),dummyPoints.end(),comparePoints);
     StatusDs status_DS;
     status_DS.root = NULL;
     status_DS.root = status_DS.Insert(status_DS.root,& Lines[ 0 ]);
     status_DS.root = status_DS.Insert(status_DS.root,& Lines[ size-1 ]);

   	 for (int i = 1; i < size-1; ++i)
   	 {
   		int id = dummyPoints[i].id;
   		int prevId;
   		if(id == 0)
   	        prevId = size-1;
   	    else
   	        prevId = id -1;
   		int nextId = id+1;
   	 	Line *left =& Lines[ id ];// line from point id to id+1
   	 	Line *right = &Lines[prevId];//line from point id-1 to id

   	 	Point   P1 = points[prevId],
   	            P0 = points[id],
   	            P2 = points[nextId];

   	    bool p1small = P1.compare(&P0);
   	    bool p2small = P2.compare(&P0);
   	 	if(p1small && p2small){
			if(rightDelete(status_DS,left))
				return true;
   	 	   	if(rightDelete(status_DS,right))
   	 	   	 	 return true;
   	 	}
   	 	else if(!p1small && !p2small){
   	 		if(leftInsert(status_DS,left))
   	 	   	 	return true;
   	 	   	if(leftInsert(status_DS,right))
   	 	   	 	return true;
   	 	}
   	 	else if(p1small && !p2small){

   	 		if(rightDelete(status_DS,right))
   	 			return true;
   	 		if(leftInsert(status_DS,left))
   	 	   	 	 return true;
   	 	}
   	 	else{
   	 		if(rightDelete(status_DS,left))
   	 			return true;
   	 		if(leftInsert(status_DS,right))
   	 	   	    return true;
   	 	}
   	 }
   	 return false;
}

bool SimplePoly::leftInsert(StatusDs &status_DS,Line *left){
			status_DS.root = status_DS.Insert(status_DS.root,left);
	   	 	linenode *above = status_DS.getAbove(status_DS.root,left);
	   	 	linenode *below = status_DS.getBelow(status_DS.root,left);
	   	 	if( (above != NULL && abs(above->line->id-left->id) != 1 &&  left->dointersect(above->line) ) ||
	   	 			(below != NULL && abs(left->id -below->line->id) != 1 && left->dointersect(below->line)) ){
	   	 		return true;

	   	 	}
	   	 	return false;
}

bool SimplePoly::rightDelete(StatusDs &status_DS,Line *right){
			linenode *above = status_DS.getAbove(status_DS.root,right);
			linenode *below = status_DS.getBelow(status_DS.root,right);

	   	 	if( (above != NULL && below != NULL && abs(above->line->id-below->line->id) != 1 &&
	   	 			above->line->dointersect(below->line)) ){
	   	 		return true;
	   	 	}
	   	 	status_DS.root = status_DS.deleteNode(status_DS.root,right);
	   	 	return false;
}


bool SimplePoly::comparePoints(Point p1,Point p2){

	if(p1.equals(&p2))
		return 0;
	return p1.compare(&p2);

}
void SimplePoly::CreateDCEL(){

  	int size = this->polyPoints.size();
    // create first edge and its twin edge and add them to list
    Edge *first = new Edge(polyPoints[0].ownId,NULL,1);
    Edge *firstTwin = new Edge(polyPoints[1].ownId,NULL,2);
    first->twinsId = firstTwin;
    firstTwin->twinsId =  first;
    dcel.EdgeList.push_back(first);
    dcel.EdgeList.push_back(firstTwin);

    /*Add first two faces with respect to first edge and its twin*/
    Face f0 = Face(firstTwin,0);
    Face f1 = Face(first,1);
    dcel.FaceList.push_back(f0);
    dcel.FaceList.push_back(f1);

    //Set the vertexList of the DCEL
    dcel.vertexList = polyPoints;
    for (int i = 1; i < size; ++i)
    {
       addEdge(polyPoints[i],polyPoints[(i+1)%size],1,0);
    }
    dcel.vertexList[0].edgeId = first;
    //set Next of last edges and first edges appropriately
    int esize = dcel.EdgeList.size();
    dcel.EdgeList[esize-2]->nextId = first;
    first->prevId = dcel.EdgeList[esize-2];
    dcel.EdgeList[esize-1]->prevId = firstTwin;
    firstTwin->nextId = dcel.EdgeList[esize-1];
}

void SimplePoly::addEdge(Vertex start,Vertex end,int Face1 , int Face2){

  	int s = dcel.EdgeList.size();
  	Edge * e = new Edge(start.ownId,dcel.EdgeList[s-2],Face1);
  	Edge * eTwin = new Edge(end.ownId,dcel.EdgeList[s-1],Face2);
  	e->twinsId = eTwin;
  	eTwin->twinsId = e;
  	dcel.EdgeList[s-2]->nextId = e;
  	dcel.EdgeList[s-1]->prevId = eTwin;
  	eTwin->nextId  = dcel.EdgeList[s-1];
  	dcel.EdgeList.push_back(e);
  	dcel.EdgeList.push_back(eTwin);
    dcel.vertexList[start.ownId].edgeId = e;  
}

/* Add the diagonal between two points.
 * @param start point is the bigger point
 * @param End point ths smaller point
 *
 *  */

void SimplePoly:: addDiagonal(Vertex &start, Vertex &end){

    // Get the edges from the Points first
    Edge *e2 = dcel.vertexList[start.ownId].edgeId;

    Edge *e3 = dcel.vertexList[end.ownId].edgeId;
    if(e2->faceId !=  e3->faceId){
    	while(1){
    		Edge *prev3 = e3;
    		while( e2->faceId !=  e3->faceId){
    			e3 = e3->twinsId;
    			e3 = e3->nextId;
    			if(prev3 == e3)
    				break;
    		}
    		if(e2->faceId ==  e3->faceId && binary_search (curFaceIdList.begin(), curFaceIdList.end(),e2->faceId ))
    			break;
    		e2 = e2->twinsId;
    		e2 = e2->nextId;
    	}
    }
    Edge *e1 = e2->prevId;

    Edge *e4 = e3->prevId;

    Edge * e = new Edge(start.ownId,NULL,e1->faceId);
    dcel.FaceList[e1->faceId].edgeId = e;
    //curFaceIdList.clear();
    int fid  = dcel.FaceList.size();
    //curFaceIdList.push_back(e1->faceId);
    curFaceIdList.push_back(fid);
    Edge * eTwin = new Edge(end.ownId,NULL,fid);

    Face f = Face(eTwin,fid);
    // set them as each others twins
    e->twinsId = eTwin;
    eTwin->twinsId = e;
    ///Now set the edge list Respectively.
    e1->nextId = e;
    e->prevId  = e1;
    e3->prevId = e;
    e->nextId  = e3;
    e2->prevId    = eTwin;   
    eTwin->nextId = e2;
    e4->nextId    = eTwin;
    eTwin->prevId = e4;

    Edge *tt = eTwin->nextId;
    do{
    	tt->faceId = fid;
    	tt = tt->nextId;
    }while(tt != eTwin);

    dcel.EdgeList.push_back(e);
    dcel.EdgeList.push_back(eTwin);
    dcel.FaceList.push_back(f);
}


void SimplePoly::divideInMonotone(){
  int size = polyPoints.size();
  StatusDs status_DS;
  status_DS.root = NULL;
  curFaceIdList.push_back(1);
  for (int i = 0; i < size; ++i)
  {

      int thisid = dummyPoints[i].id;
      int prevId;
      if(thisid == 0)
        prevId = size-1;
      else
        prevId = thisid -1;
      int nextId = (thisid+1)%size;
      Point P1 = points[prevId],
            *P0 = &points[thisid],
            P2 = points[nextId];

      bool p1small = P1.compare(P0);
      bool p2small = P2.compare(P0);

      double ang = calAngle(P1,*P0,P2);
      Line *l2 = &Lines[thisid ];// line from point id to id+1
      Line *l1 = &Lines[prevId];//line from point id-1 to id
      Line *ea=NULL,*eb=NULL;
      //Internal angle is calculated is as P0P1 is clockwise closer to P0P2
      // hence positive angle is less than 180 internal angle
      // negative angle is greater than 180 internal angle
      if(!p1small && !p2small && ang >0){
    	  //start vertex
    	  ea = l2;
    	  ea->helper = P0;
    	  ea->helper->isMerge = false;

    	  status_DS.root = status_DS.Insert(status_DS.root,l1);
    	  status_DS.root = status_DS.Insert(status_DS.root,l2);
      }
      else if(p1small && p2small && ang > 0 ){ //

    	  //end vertex
    	  relaxEdge(dcel.vertexList[P0->id],*l1);

    	  status_DS.root = status_DS.deleteNode(status_DS.root,l1);
    	  status_DS.root = status_DS.deleteNode(status_DS.root,l2);

      }
      else if(p1small && p2small && ang <= 0.0000){
    	  //mergeVertex internal angle is greater than 180 means it is negative

    	  linenode *above = status_DS.getAbove(status_DS.root,l2);
    	  linenode *below = status_DS.getBelow(status_DS.root,l1);

    	  ea = above->line;
    	  eb = below->line;

    	  status_DS.root = status_DS.deleteNode(status_DS.root,l1);
    	  status_DS.root = status_DS.deleteNode(status_DS.root,l2);


    	  relaxEdge(dcel.vertexList[P0->id],*l1);
    	  relaxEdge(dcel.vertexList[P0->id],*ea);
    	  ea->helper = P0;
    	  ea->helper->isMerge = true;
      }
      else if(!p1small && !p2small && ang <= 0.0000){
    	  //splitVertex internal angle is greater than 180 means it is negative

    	  status_DS.root  = status_DS.Insert(status_DS.root,l1);
    	  status_DS.root  = status_DS.Insert(status_DS.root,l2);
    	  linenode *above = status_DS.getAbove(status_DS.root,l1);
    	  linenode *below = status_DS.getBelow(status_DS.root,l2);
    	  ea = above->line;
    	  eb = below->line;
    	  // Add diagonal first and then change the helper of ea(edge above split vertex)
    	  addDiagonal(dcel.vertexList[P0->id],dcel.vertexList[ea->helper->id]);
    	  ea->helper = P0;
    	  eb->helper = P0;
    	  eb->helper->isMerge = false;

      }
      else if(p1small && !p2small){
    	  //upper vertex
    	  relaxEdge(dcel.vertexList[P0->id],*l1);
    	  l2->helper = P0;
    	  l2->helper->isMerge = false;
    	  status_DS.root = status_DS.deleteNode(status_DS.root,l1);

    	  status_DS.root = status_DS.Insert(status_DS.root,l2);

      }
      else{
    	  //LowerVertex
    	  status_DS.root = status_DS.deleteNode(status_DS.root,l2);
    	  status_DS.root = status_DS.Insert(status_DS.root,l1);
    	  linenode *above = status_DS.getAbove(status_DS.root,l1);
    	  ea = above->line;
    	  relaxEdge(dcel.vertexList[P0->id],*ea);
    	  ea->helper = P0;
    	  ea->helper->isMerge = false;
      }
  }
}
void SimplePoly::relaxEdge(Vertex &v, Line &e){

	if(e.helper !=NULL && e.helper->isMerge ){
		int id = e.helper->id;
		Vertex end = polyPoints[id];
		addDiagonal(v,end);
	}
}
double SimplePoly::calAngle(Point P1,Point P0,Point P2){
    
      //define P0P1 = {P1.x-P0.x,P1.y-P0.y}
      //define P0P2 = {P2.x-P0.x,}
      //cross product = 

      double  x1 = P1.x-P0.x,
              y1 = P1.y-P0.y,
              x2 = P2.x-P0.x,
              y2 = P2.y-P0.y;
      double dot = (x1*x2) + (y1 * y2); // dot product
      double cross = (x1 * y2 - x2 * y1); // cross product

      double alpha = atan2(cross, dot);

      return (alpha * 180. / M_PI );

  }
void SimplePoly::tringulatePolyFaces(){
		int size = dcel.FaceList.size();
		vector<Face> erFaceList = dcel.FaceList;
		curFaceIdList.clear();
	    for (int var = 1; var < size; ++var) {
	    	curFaceIdList.push_back(erFaceList[var].ownId);
	    	Edge *e = erFaceList[var].edgeId, *e1 = e;
	    	vector<Vertex> v;
	    	int i = 0;
	    	bool isminFound = false;
	    	do{
	    		v.push_back(dcel.vertexList[e1->originId]);
	    		e1 = e1->nextId;
	    		i++;

	    	}while(e!= e1);

	    	if(v.size()>3)
	    		triangulateMonotone(v);
	    	v.clear();

	    	curFaceIdList.clear();
		}
}

void SimplePoly :: triangulateMonotone(vector<Vertex> vertices){
		dummyPoints.clear();
		stack<Point> stack_points;
		int maxId,minId;
		int size = vertices.size();
		Point maxPoint,minPoint;
		int minIndex;
		for (int i = 0; i < size; ++i)
		{
				Point P;
				P.copyVertex(vertices[i]);
				points.push_back(P);
				if(i == 0){
					maxPoint = P;
					minPoint  = P;
					minIndex = 0;
				}

				if(maxPoint.compare(&P)){
					maxPoint = P;
				}
				if(!minPoint.compare(&P)){
					minPoint = P;
					minIndex = i;
				}
				dummyPoints.push_back(P);
		}
		maxId = maxPoint.id;
		minId = dummyPoints[minIndex].id;
		int id;
		int index = minIndex;
		do{
			id = vertices[index].ownId;
			dcel.vertexList[id].isUpper = true;
			index++;
			index = index%size;

		}while(maxId != id);

		do{
		    id = vertices[index].ownId;
		 	dcel.vertexList[id].isUpper = false;
		 	index++;
		 	index %= size;
		}while(minId != id);

		sort(dummyPoints.begin(),dummyPoints.end(),comparePoints);
		stack_points.push(dummyPoints[0]);
		stack_points.push(dummyPoints[1]);

	   for(int i = 2; i < size;i++){
		   int lastId = stack_points.top().id;
		   Point P1 = dummyPoints[i];
		   int nowId  = P1.id;
		   double angle;
		   if(dcel.vertexList[lastId].isUpper  && dcel.vertexList[nowId].isUpper){

			   Point P0 = stack_points.top();
			   stack_points.pop();
			   Point P2 = stack_points.top();
			   angle = calAngle(P2,P0,P1);
			   if(angle > 0 && angle < 180.000000){ // this angle is always a inner angle(clockwise closeP0P1 to P0P2 ) considered
				   // keeps on popping till upper angle becomes greater then or equal to 180
				   while( angle > 0 && angle < 180.000000 &&!stack_points.empty()){
					  addDiagonal(dcel.vertexList[P1.id],dcel.vertexList[P2.id]) ;
					  P0 = P2;
					  P2 = stack_points.top();
					  stack_points.pop();
				   }

				   stack_points.push(P2);
				   stack_points.push(P1);
			   }
			   else{// Form a reflex chain

				   stack_points.push(P0);
				   stack_points.push(P1);
			   }


		   }
		   else if(!dcel.vertexList[lastId].isUpper  && !dcel.vertexList[nowId].isUpper){
			   	   	   	   Point P0 = stack_points.top();
			   			   stack_points.pop();
			   			   Point P2 = stack_points.top();
			   			   angle = calAngle(P1,P0,P2);
			   			   if( angle > 0 && angle < 180.000000){

			   				   while( angle > 0 && angle < 180.000000 &&!stack_points.empty()){
			   					  addDiagonal(dcel.vertexList[P1.id],dcel.vertexList[P2.id]) ;
			   					  P0 = P2;
			   					  P2 = stack_points.top();
			   					  stack_points.pop();
			   				   }

			   				   stack_points.push(P2);
			   				   stack_points.push(P1);
			   			   }
			   			   else{ // Form a reflex chain

			   				   stack_points.push(P0);
			   				   stack_points.push(P1);
			   			   }
		   }
		   else{
				   Point P0 = stack_points.top();
				   Point firstP0 = P0;
				   stack_points.pop();

				   while(!stack_points.empty()){
					   addDiagonal(dcel.vertexList[P1.id], dcel.vertexList[P0.id]);
					   P0 = stack_points.top();
					   stack_points.pop();
				   }
				   stack_points.push(firstP0);
				   stack_points.push(P1);

		   }
	   }

}

void SimplePoly::ColorPloyDFS(){
	stack<Face *> faces;
	faces.push(&dcel.FaceList[1]);

	vector<Vertex *> v;
	while(!faces.empty()){
		Face *face = faces.top();
		faces.pop();
		Edge *e = dcel.FaceList[face->ownId].edgeId, *e1 = e;


		int i = 0;
		do{
		    v.push_back( &dcel.vertexList[e1->originId]);
		    int f1 = e1->twinsId->faceId;
		    if(f1 != 0 && !dcel.FaceList[f1].isColored){
		    	Face *f = &dcel.FaceList[f1];
		    	faces.push(f);
		    }
		    e1 = e1->nextId;
		    i++;
		}while(e!= e1);

		for (int var = 0;  var < 3; ++ var) {
			if(v[var]->col == -1){
				int v1 = v[(var+1)%3]->col;
				int v2 = v[(var+2)%3]->col;

				if(v1 == -1 && v2 == -1){
					v[var]->col     = 0;
					v[(var+1)%3]->col	= 1;
					v[(var+2)%3]->col = 2;
				}
				else {
					v[var]->col = 3 - (v1 + v2);
					colorValues[v[var]->col]++;
				}
			}

		}
		face->isColored = true;
		v.clear();
	}
   totalGaurd = colorValues[0];

	if(totalGaurd > colorValues[1]){
		totalGaurd = colorValues[1];
		minColor = 1;
	}
	if(totalGaurd > colorValues[2]){
		totalGaurd = colorValues[2];
		minColor = 2;
	}

}

void SimplePoly:: writesvg(string filename,int status){
	fstream fw;
	fw.open(const_cast<char *> (filename.c_str()), ios::out);
				         cout << "in writer" << endl;

	string col = "red";
	fw << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" <<endl
	<< "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">"<<endl
	<< "<svg width=\"840px\" height=\"640px\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"<<endl
	<< "<rect x=\"0\" y=\"0\" width=\"800\" height=\"600\" style=\"fill:yellow;stroke:brown;stroke-width:2;fill-opacity:0.4;stroke-opacity:0.8\"/>"<<endl;

	int size = dcel.FaceList.size();
    for (int var = 1; var < size; ++var) {
    	Edge *e = dcel.FaceList[var].edgeId, *e1 = e;
    	vector<Vertex> v;
    	int i = 0;
    	do{
    		v.push_back( dcel.vertexList[e1->originId]);
    		e1 = e1->nextId;
    		i++;

    	}while(e!= e1);

    	fw<< "<polygon points=\"";
    	int max = v.size();
    	for (i = 0; i < max; ++i) {
    		fw<<v[i].x<<","<<v[i].y<<" ";
		}
    	fw<<"\" style=\"fill:white;stroke:purple;stroke-width:1\" />" << endl;
	}
    size = dcel.vertexList.size();

    for (int i = 0; i <size ; ++i)
    {
    	if(status)
    	fw<<"<circle cx=\""<< dcel.vertexList[i].x <<"\" cy=\"" << dcel.vertexList[i].y <<"\" r=\"5\" stroke=\""<<coll[dcel.vertexList[i].col]<<"\" stroke-width=\"1\"/>"<<endl;
    	fw  <<"<text x=\""<<dcel.vertexList[i].x<<"\" y=\""<<dcel.vertexList[i].y<<"\" text-anchor=\"middle\" font-size=\"20px\" fill=\"#aaaaaa\">"<<dcel.vertexList[i].ownId<<"</text>"<<endl;
    }
	fw<<"</svg>" <<endl;
	fw.close();
}


