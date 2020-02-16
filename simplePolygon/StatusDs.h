#ifndef StatusDs_H_INCLUDED
#define StatusDs_H_INCLUDED


#include <stdlib.h>
#include "Points.h"
#include "Line.h"
#include <stack>

using namespace std;

class linenode
{
public:
    Line *line;
    class linenode *left;
    class linenode *right;
    int height;
};
class StatusDs
{
        // A utility function to get height of the tree
        public:
        class linenode* root ;
        class linenode* parent ;
        stack<class linenode*> prevParents;
        class linenode* successor;
        class linenode* key;     


        int height(class linenode *N)
        {
            if (N == NULL)
                return 0;
            return N->height;
        }
        int Max(int a, int b)
        {
            return (a > b)? a : b;
        }
        class linenode* newNode(Line *key)
        {

            class linenode* lineNode = new linenode();


            lineNode->line = key;

            lineNode->left   = NULL;
            lineNode->right  = NULL;
            lineNode->height = 1;  // new lineNode is initially added at leaf
            return(lineNode);
        }
        class linenode *rightRotate(class linenode *y)
        {
            class linenode *x = y->left;
            class linenode *T2 = x->right;
            x->right = y;
            y->left = T2;

            // Update heights
            y->height = Max(height(y->left), height(y->right))+1;
            x->height = Max(height(x->left), height(x->right))+1;

            // Return new root
            return x;
        }

        // A utility function to left rotate subtree rooted with x
        class linenode *leftRotate(class linenode *x)
        {
            class linenode *y = x->right;
            class linenode *T2 = y->left;

            // Perform rotation
            y->left = x;
            x->right = T2;

            //  Update heights
            x->height = Max(height(x->left), height(x->right))+1;
            y->height = Max(height(y->left), height(y->right))+1;

            // Return new root
            return y;
        }

        class linenode* getAbove( class linenode *root1,Line *X){
            class linenode* l =  Search(root1,X);
            return Successor(l);
        }

        class linenode* getBelow( class linenode *root1,Line *X){
            class linenode* l =  Search(root1,X);
            return Predecessor(l);  
        }


        // Get Balance factor of lineNode N
        int getBalance(class linenode *N)
        {
            if (N == NULL)
                return 0;
            return height(N->left) - height(N->right);
        }

        class linenode* Insert(class linenode* lineNode, Line *key)
        {
            /* 1.  Perform the normal BST rotation */

            if (lineNode == NULL)
                return(newNode(key));

            if (key->compare(lineNode->line))
                lineNode->left  = Insert(lineNode->left, key);
            else
                lineNode->right = Insert(lineNode->right, key);

            /* 2. Update height of this ancestor lineNode */
            lineNode->height = Max(height(lineNode->left), height(lineNode->right)) + 1;

            /* 3. Get the balance factor of this ancestor lineNode to check whether
               this lineNode became unbalanced */
            int balance = getBalance(lineNode);

            // If this lineNode becomes unbalanced, then there are 4 cases

            // Left Left Case
            if (balance > 1 && key->compare(lineNode->left->line))
                return rightRotate(lineNode);

            // Right Right Case
            if (balance < -1 && !(key->compare(lineNode->right->line)))
                return leftRotate(lineNode);

            // Left Right Case
            if (balance > 1 && !(key->compare(lineNode->left->line)))
            {
                lineNode->left =  leftRotate(lineNode->left);
                return rightRotate(lineNode);
            }

            // Right Left Case
            if (balance < -1 && key->compare(lineNode->right->line))
            {
                lineNode->right = rightRotate(lineNode->right);
                return leftRotate(lineNode);
            }

            /* return the (unchanged) lineNode pointer */
            return lineNode;
        }

       class linenode * Successor(class linenode* lineNode)
        {
    	    stack<class linenode*> prevParents = this->prevParents;
    	    class linenode * parent = this->parent;

            if(lineNode->right != NULL)
                return getMinimum(lineNode->right );
            else{

                if(parent != NULL && parent->left == lineNode)
                    return parent;
                else while(!prevParents.empty() ){

                    if(prevParents.top()->left == parent)
                        return prevParents.top();
                    else
                        parent = prevParents.top();
                    prevParents.pop();
                }

            }

            return NULL;

        }
        class linenode * Predecessor(class linenode *lineNode)
        {

        	stack<class linenode*> prevParents = this->prevParents;
        	class linenode * parent = this->parent;
            if(lineNode->left !=NULL)
               return getMaximum(lineNode->left);
            else{

                if(parent != NULL && parent->right == lineNode){
                    return parent;

                }

                else while(!prevParents.empty() ){

                    if(prevParents.top()->right == parent)
                        return prevParents.top();
                    else
                        parent = prevParents.top();
                    prevParents.pop();
                }
           }

           return NULL;
        }

        class linenode * getMinimum(class linenode* lineNode)
        {
            class linenode* current = lineNode;
            while (current->left != NULL)
                current = current->left;

            return current;
        }

        class linenode * getMaximum(class linenode *lineNode)
        {
            class linenode *current = lineNode;
            while(current->right !=NULL)
                current = current->right;

            return current;
        }

        class linenode* deleteNode(class linenode* root, Line *key)
        {
            if (root == NULL)
                return root;
            if ( key->compare(root->line))
                root->left = deleteNode(root->left, key);
            else if(!(key->equals(root->line)))
                root->right = deleteNode(root->right, key);
            else
            {
                if( (root->left == NULL) || (root->right == NULL) )
                {
                    class linenode *temp = root->left ? root->left : root->right;
                    if(temp == NULL)
                    {
                        temp = root;
                        root = NULL;
                    }
                    else // One child case
                     *root = *temp; // Copy the contents of the non-empty child
                    free(temp);
                }
                else
                {
                    class linenode* temp = getMinimum(root->right);
                    root->line = temp->line;

                    root->right = deleteNode(root->right,temp->line);
                }
            }
            if (root == NULL)
              return root;
            root->height = Max(height(root->left), height(root->right)) + 1;
            int balance = getBalance(root);
            if (balance > 1 && getBalance(root->left) >= 0)
                return rightRotate(root);
            if (balance > 1 && getBalance(root->left) < 0)
            {
                root->left =  leftRotate(root->left);
                return rightRotate(root);
            }
            if (balance < -1 && getBalance(root->right) <= 0)
                return leftRotate(root);
            if (balance < -1 && getBalance(root->right) > 0)
            {
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }

            return root;
        }
        class linenode * Search(class linenode *root1,Line *X)
        	{
        		
        		int count = 0;
                parent = NULL;
                stack<class linenode*> prevParents;
                if(root1==NULL){
                     this->prevParents = prevParents;
                    return NULL;
                }
                while(root1!=NULL)
                {
                        count++;
                        if(X->equals(root1->line)){
                            this->prevParents = prevParents;
                            return root1;
                        }
                        if(X->compare(root1->line)){
                                if(count >1)
                                    prevParents.push( parent);
                                parent = root1;
                                root1=root1->left;
                            }
                            else{
                                if(count >1)
                                    prevParents.push(parent);
                                parent = root1;
                                root1=root1->right;
                            }

                }
                this->prevParents = prevParents;
                return NULL;
        	}
            void infix(class linenode *root1)
            {

                if(root1->left!=NULL)
                    infix(root1->left);
                cout<< "lineId: " << root1->line->id <<endl;
                cout<<"left Point : x  is "<<root1->line->left->x <<" and y is "<<root1->line->left->y<<endl;
                cout<<"right Point : x  is "<<root1->line->right->x<<" and y is "<<root1->line->right->y<<endl;

                if(root1->right!=NULL)
                    infix(root1->right);
                return;
            }
};

#endif // StatusDs_H_INCLUDED
