#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<list>
#include<unordered_map>
#include<vector>
#include<queue>
#include<fstream>
#include<sstream>
#include<stack>
#include<unordered_set>
using namespace std;


typedef enum{
	TYPE_OP,//operator
	TYPE_PRE//predicate
}NodeType;



typedef enum{
	TYPE_VAR,
	TYPE_CONS
}ArgType;

typedef struct argNode
{
	ArgType type;
	string name;
	struct argNode *next;
		
}ArgNode;

typedef struct node
{
	NodeType type;
	string name;
	union{
		struct{
		struct node *left;
		struct node *right;
		}object;//operator
		ArgNode * arg;//argument list
	}uni;
}Node;	


struct type{
	Node* pointer;
	ArgNode * argPointer;
	string name;

};

#define YYSTYPE type

extern Node *aRule;
extern Node *aQuery;
extern long long varIndex;

typedef struct{
	string pre;
	ArgNode *argHead;// argHead itseft dose not point to an arg
	int isYes; // 1 yes; 0 no(means NOT)	
}Literal;


typedef struct{
	vector<Literal *> literals;
	list<Literal *> literals2;
	unordered_map<string, vector<int>> index;
	pair<long long, long long> varIndexRange;
}Clause;

extern unordered_map<string,list<Clause *>> KB; 
extern list<Literal *> queries;

extern bool kb_or_query;

typedef pair<bool, unordered_map<string, string>> Sub;


