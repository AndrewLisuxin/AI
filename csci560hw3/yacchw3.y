
%{
//#include<stdio.h>
//#include<stdlib.h>
#include"node.h"
//#include<stdarg.h>
//#include"check.h"
//#include"yylex.h"
//#include<string>
  
//using namespace std;
Node *opr(string name, Node*p, Node *q);

Node *pre(string name, ArgNode *p);

void init_arg(ArgType a, string name);
void add_arg(ArgType a, string name);
//void exeRoot(Node* root);
void transMain();


extern "C"
{
void yyerror(const char *);

int yylex(void);
}
ArgNode *head, *tail;
//extern Node* aSentence;

%}

/*%struct{ 
	Node* pointer;
	ArgNode * argPointer;
	string name;
};*/

%token <name> VAR
%token <name> POC
%type <argPointer> argument
%type <pointer> expr expr2
%left '&''|''~' IMP

%%
rule:
|rule expr  { 
		if(kb_or_query)
		{	

			aRule = $2; 
			transMain();
		}
		else{

			aQuery = $2;
			//cout<<aQuery->name<<endl;
		}
		}//sentence
|rule expr2 {aQuery = $2;}//query
;

expr:
POC '(' argument ')' {$$ = pre($1,$3);}
|'('expr '&' expr')' {$$ = opr("&",$2,$4);}
|'('expr '|' expr')' {$$ = opr("|",$2,$4);}
|'(''~' expr')' {$$ = opr("~",$3,NULL);}
|'('expr IMP expr ')'{$$ = opr("=>",$2,$4);}
;

expr2:
'~' expr {$$ = opr("~",$2,NULL);}
;

argument:
VAR  { init_arg(TYPE_VAR, $1); $$ = head;}
|argument ',' VAR { add_arg(TYPE_VAR, $3); $$ = head;}
|POC  { init_arg(TYPE_CONS, $1); $$ = head;}
|argument ',' POC  { add_arg(TYPE_CONS, $3); $$ = head;}
;
%%

Node *opr(string name, Node*p, Node*q)
{
	Node *a = new Node;
	a->type = TYPE_OP;
	a->name = name;
	a->uni.object.left = p;
	a->uni.object.right = q; 
	return a;
	
}
//void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer );}

Node *pre(string name, ArgNode *p)
{
	Node *a = new Node;
	a->type = TYPE_PRE;
	a->name = name;
	a->uni.arg = p;
        return a;
}

void init_arg(ArgType aType, string name)
{
	head = new ArgNode;
	head->type = aType;
	head->name = name;
	//printf("%s\n",head->name);
	head->next = NULL;
	tail = head;	
}

void add_arg(ArgType aType, string name)
{
	ArgNode *a = new ArgNode;
	a->type = aType;
	a->name = name;
	a->next = NULL;
	tail->next = a;
	tail = a;	
}


void yyerror(const char *s)
{
	cout << s << endl;
}
void parse()
{
	
	yyparse();
}


















