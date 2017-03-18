#include"node.h"
//void exeRoot(Node *root);
void imple_elimination(Node *p);
void move_not_inward(Node *p, Node *father);
void distribute_or_over_and(Node *p,Node *father, int i);
void distribute_left(Node *p, Node *father, int i);
void distribute_right(Node *p, Node *father, int i);

void imple_elimination(Node *p)
{
	//DFS from bottom to top to eliminate "=>"
	if(p == NULL || p->type == TYPE_PRE)
		return;
	else{
		imple_elimination(p->uni.object.left);
		imple_elimination(p->uni.object.right);
		if(p->name == "=>")
		{
			Node *nl = new Node;
			nl->type = TYPE_OP;
			nl->name = "~";
			nl->uni.object.left = p->uni.object.left;
			nl->uni.object.right = NULL;

			p->name = "|";
			p->uni.object.left = nl;
		}
	}	
	
}

void move_not_inward(Node *p, Node *father)
{
	//DFS	from top to bottom to move (can counteract double not)
	if(p == NULL || p->type == TYPE_PRE)
		return;
	if(p->type == TYPE_OP && p->name == "~" && p->uni.object.left->type != TYPE_PRE)
	{
		Node *l = p->uni.object.left;
		string s = l->name;
		if(s == "&" || s == "|")
		{
			Node * a = new Node;
			a->type = TYPE_OP;
			a->name = "~";
			a->uni.object.left = l->uni.object.right;
			a->uni.object.right = NULL;
			
			l->name = "~";
			l->uni.object.right = NULL;

			p->uni.object.right = a;
			p->name = s == "&" ? "|" : "&";

			move_not_inward(l,p);
			move_not_inward(a,p);
			
		}
		else{
			// s == "~" double nots
			father->uni.object.left = l->uni.object.left;
			delete(p);
			delete(l);
			move_not_inward(father->uni.object.left, father);
		}
	}
	else{
			move_not_inward(p->uni.object.left,p);
			move_not_inward(p->uni.object.right,p);
		
	}
}

void distribute_or_over_and(Node *p,Node *father, int i)
{
	//filter down from bottom to up , firstly left, then right
	if(p->type == TYPE_PRE || p->type == TYPE_OP && p->name == "~")
		return;
	else{
		Node *l = p->uni.object.left;
		Node *r = p->uni.object.right;
		distribute_or_over_and(l,p,0);
		distribute_or_over_and(r,p,1);
		
		if(p->type == TYPE_OP && p->name == "|")
		{
			distribute_left(p,father,i);
		}
	}
	

}

Node* copy_tree(Node *p)
{
	//DFS
	if(p->type == TYPE_OP && p->name == "&")
	{
		Node *l = copy_tree(p->uni.object.left);
		Node *r = copy_tree(p->uni.object.right);

		Node *q = new Node;

		q->type = p->type;
		q->name = p->name;
		q->uni.object.left = l;
		q->uni.object.right = r;
		
		return q;

	}
	else 
		return p;
	
}

void distribute_left(Node *p, Node *father, int i)// i :0 left; 1 right
{
	Node *l = p->uni.object.left;
	Node *r = p->uni.object.right;
	if(l->name == "&")
	{
		if(i)
			father->uni.object.right = l;
		else
			father->uni.object.left = l;

		Node *p1, *p2;

		p1 = new Node;
		p1->type = TYPE_OP;
		p1->name = "|";
		p1->uni.object.left = l->uni.object.left;

		p2 = new Node;
		p2->type = TYPE_OP;
		p2->name = "|";
		p2->uni.object.left = l->uni.object.right;


		p1->uni.object.right = r;
		p2->uni.object.right = copy_tree(r);
		

		l->uni.object.left = p1;
		l->uni.object.right = p2;

		delete(p);
		
		distribute_left(p1,l,0);
		distribute_left(p2,l,1);
	}
	else{
		distribute_right(p,father,i);
	}


}


void distribute_right(Node *p, Node *father, int i)
{
	Node *l = p->uni.object.left;
	Node *r = p->uni.object.right;
	if(r->name == "&")
	{
		if(i)
			father->uni.object.right = r;
		else
			father->uni.object.left = r;

		Node *p1, *p2;

		p1 = new Node;
		p1->type = TYPE_OP;
		p1->name = "|";
		p1->uni.object.right = r->uni.object.left;

		p2 = new Node;
		p2->type = TYPE_OP;
		p2->name = "|";
		p2->uni.object.right = r->uni.object.right;


		p1->uni.object.left = p2->uni.object.left = l;

		r->uni.object.left = p1;
		r->uni.object.right = p2;

		delete(p);
		
		distribute_right(p1,r,0);
		distribute_right(p2,r,1);
	}
	

}


void transMain()
{ 
//cout <<"-------------------------KB----------------------"<<endl;
	//exeRoot(aRule);
	imple_elimination(aRule);
	//exeRoot(aRule);
	Node *root = new Node();
	root->uni.object.left = aRule;

	move_not_inward(aRule, root);
	aRule = root->uni.object.left;
	//exeRoot(aRule);
	distribute_or_over_and(aRule,root,0);
	aRule = root->uni.object.left;
	//exeRoot(aRule);
	delete(root);

}

