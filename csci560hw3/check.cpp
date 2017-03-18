#include<stdio.h>
#include<stdlib.h>
#include"node.h"
//#include"yacchw3.tab.h"
#include<string>
#include<iostream>
int depth;
static int n = 1;
void exeNode(Node *aNode);
void exeRoot(Node* root)
{
	depth = -1;
	printf("rule (%d)\n",n++);
	exeNode(root);
}
void exeNode(Node *aNode)
{
	depth++;
	if (aNode->type == TYPE_OP)
	{
		Node *p = aNode->uni.object.left, *q = aNode->uni.object.right;
		if(p != NULL)
		{
			cout << "depth( "<<  depth <<" )   "<< aNode->name<<" -> "<< p->name<<endl;
			exeNode(p);
		
		}
	
		if(q != NULL)
		{
			cout << "depth( "<<  depth <<" )   "<< aNode->name<<" -> "<< q->name<<endl;
				exeNode(q);
		
		}
	}
	else{
		//predicate
		
		ArgNode *a;
		printf("depth( %d ):\t",depth);
		cout <<  aNode->name<<" : ";
		for(a = aNode->uni.arg;a != NULL; a = a->next)
			cout << a->name <<" " ; 
		printf("\n");
	}
	depth--;
}

