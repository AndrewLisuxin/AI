#include"node.h"

//use hash table to store substitution to implement fast search

// backward chaining to find the path.

Sub Unify(ArgNode *x, ArgNode * y, Sub s);
Sub Unify_Var(ArgNode *x, ArgNode * y, Sub s);
Literal * Subst(Sub s, Literal * first);
bool Resolution(Literal *query);
list<Sub> Resolution_Or(Literal *goal, Sub s);
list<Sub> Resolution_And(list<Literal *> a, Sub s);

Literal * currentQuery;

Sub Unify(ArgNode *x, ArgNode * y, Sub s)
{
	
	if(!s.first)
		return s;
	else	
	{
		if(x->next == NULL)
		{	
			if(x->name == y->name)
				return s;
			else{
				if(x->type == TYPE_VAR)
					return Unify_Var(x, y, s);
				else{
					if(y->type == TYPE_VAR)
						return Unify_Var(y, x, s);
					else
					{
						s.first = false;
						return s;
					}
				}
			}
		}
		else{
			ArgNode *x_first = new ArgNode;
			x_first->type = x->type;
			x_first->name = x->name;
			x_first->next = NULL;
			
			ArgNode *y_first = new ArgNode;
			y_first->type = y->type;
			y_first->name = y->name;
			y_first->next = NULL;
			
			
			s = Unify(x->next, y->next, Unify(x_first, y_first, s));
			delete(x_first);
			delete(y_first);
			return s;
		}
	

	
	}


}
bool isVar(string s)
{
	if(s[0] >= 'a' && s[0] <= 'z')
		return true;
	else
		return false;
}
Sub Unify_Var(ArgNode *x, ArgNode * y, Sub s)
{
	unordered_map<string, string>::iterator it;
	ArgNode *ex;
	
	it = s.second.find(x->name);

	if(it != s.second.end())
	{
		ex = new ArgNode;
		ex->type = isVar(it->second) ? TYPE_VAR : TYPE_CONS;
		ex->name = it->second;
		ex->next = NULL;
		return Unify(ex, y, s);
	}
	else{
		it = s.second.find(y->name);
		if(it != s.second.end())
		{
			ex = new ArgNode;
			ex->type = isVar(it->second) ? TYPE_VAR : TYPE_CONS;
			ex->name = it->second;
			ex->next = NULL;
			return Unify(x, ex, s);
		}
		else
			s.second.insert(pair<string, string>(x->name, y->name));
			return s;
	}

}

//unordered_map<string,list<Clause *>>::iterator iterKB;






//use a hash table to store intermedia states to check if there is a loop 

unordered_set<string> checkLoop;

bool Resolution(Literal *query)
{
	Sub s;
	s.first = true;
	s.second.clear();
	//suppose
	query->isYes = 1 - query->isYes;
	
	currentQuery = query;
	list<Sub> result = Resolution_Or(query, s);
	if(!result.empty())
		return true;
	else 	
		return false;
}


list<Sub> Resolution_Or(Literal *goal, Sub s)
{
//cout << "----------------------Resolution_Or---------------------"<<endl;
//cout << goal->pre<<goal->isYes<<endl;
	list<Clause *>::iterator iterClause;
	vector<int>::iterator iterIndex;
	list<Sub> choices;
	
///////////////////////  check loop /////////////////////// 
string checkStr = goal->pre + to_string(goal->isYes);
unordered_map<string, string> stand;
stand.clear();
unordered_map<string, string>::iterator iterStand;
int localIndex = 0;
string exName;
for(ArgNode * a = goal->argHead->next; a != NULL;a = a->next)
{
        checkStr += "-";
	if(a->type == TYPE_VAR)
	{
		iterStand = stand.find(a->name);
		if(iterStand == stand.end())
		{
			exName = "y" + to_string(localIndex++);
			checkStr += exName;
			stand.insert(pair<string,string>(a->name, exName));
		}
		else
			checkStr += iterStand->second;
	}
	else
		checkStr += a->name;
}
unordered_set<string>::iterator iterLoop = checkLoop.find(checkStr);
if(iterLoop != checkLoop.end())// has a loop
	return choices;
else
	checkLoop.insert(checkStr);
	




///////////////////////////////////////////////////////////
	string key;
	unordered_map<string,list<Clause *>>::iterator iterKB;
	key = goal->pre + to_string(1 - goal->isYes);
//cout << key <<endl;
	iterKB = KB.find(key);
if(iterKB != KB.end())//exist predicates could be resoluted
{	list<Clause *> & Clist = iterKB->second;

	for(iterClause = Clist.begin(); iterClause != Clist.end(); iterClause++)
	{
		vector<int> & Lndex = (*iterClause)->index.find(key)->second;
		for(iterIndex = Lndex.begin(); iterIndex != Lndex.end(); iterIndex++)
		{
			
			int i = *iterIndex;

///////////////copy the whole clause to avoid name conflict when unification/////////////////////////
			list<Literal *>::iterator iterLiteralA, iterLiteralB;

			list<Literal *> literals3;

			for(iterLiteralA = (*iterClause)->literals2.begin();iterLiteralA != (*iterClause)->literals2.end();iterLiteralA ++)
			{
				Literal * l = new Literal;
				
				l->pre = (*iterLiteralA)->pre;
				l->isYes = (*iterLiteralA)->isYes;

				l->argHead = new ArgNode;
				ArgNode *p = l->argHead;
				for(ArgNode *q = (*iterLiteralA)->argHead->next;q != NULL; q = q->next)
				{
					p->next = new ArgNode;
					p->next->type = q->type;
					p->next->next = NULL;
					if(q->type == TYPE_VAR)
					{
						//change variable name to a brand new one
						unsigned long long ind = stoll (q->name.substr(1,q->name.length()-1),nullptr,10);
						p->next->name = "x" + to_string((varIndex + (ind - (*iterClause)->varIndexRange.first)));
					}	
					else
						p->next->name = q->name;
					
					p = p->next;			
				}

				literals3.push_back(l);
			}
///////////////         //update varIndex
			varIndex += ((*iterClause)->varIndexRange.second - (*iterClause)->varIndexRange.first + 1);
//cout << "-------------(*iterClause)->varIndexRange.second - (*iterClause)->varIndexRange.first + 1-----------"<<endl;
//cout<< (*iterClause)->varIndexRange.second - (*iterClause)->varIndexRange.first + 1<<endl;
//cout << "-------------------------update varIndex---------------------" << endl;
//cout<<varIndex<<endl;
			//////////////////////
			iterLiteralB = literals3.begin();

			while(i-- > 0)
				iterLiteralB++;
			
			ArgNode * rhl = (*iterLiteralB)->argHead ->next;
//cout << literals3.size() <<endl;
			literals3.erase(iterLiteralB);
//cout << literals3.size() <<endl;
/////////////////

list<Sub> part = Resolution_And(literals3, Unify(rhl, goal->argHead->next, s));
choices.insert(choices.end(),part.begin(),part.end());
//////////////////
			
			
			
		}
	}
}
// need check if negation of the query can be used repeatlly////////////////////////////
if(goal->pre == currentQuery->pre && goal->isYes == 1 - currentQuery->isYes)
{
	Sub tmp = Unify(goal->argHead->next, currentQuery->argHead->next,s);
	if(tmp.first)
		choices.push_back(tmp);
		
	
}
	checkLoop.erase(checkLoop.find(checkStr));
	return choices;


}

list<Sub> Resolution_And(list<Literal *> a, Sub s)
{
//cout << "----------------------Resolution_And---------------------"<<endl;
	list<Sub> choices;
	if(s.first == false)
		return choices;
	else{
//cout << "a size :" << a.size() <<endl;
		if(a.size() == 0)
		{
			
			choices.push_back(s);
//cout << "----------------------And---------------------"<<endl;
			return choices;
		}
		else{
			list<Sub>  candidates = Resolution_Or(Subst(s, a.front()), s);
//cout << candidates.size() <<endl;
			a.pop_front();
			for(list<Sub>::iterator it = candidates.begin();it != candidates.end();it++)
			{
				list<Sub> part = Resolution_And(a, *it);
				choices.insert(choices.end(), part.begin(), part.end());
			}
//cout << choices.size() <<endl;
			return choices;
		}

	}	
	
}

Literal * Subst(Sub s, Literal * first)
{
//remain Literal * first, use another literal to finish this substitution( do not change KB) 
	Literal *out = new Literal;
	out->argHead = new ArgNode;
	out->argHead->next = NULL;
	out->pre = first->pre;
	out->isYes = first->isYes;
	unordered_map<string, string>::iterator hit;
	ArgNode *before, *after;
	for(before = first->argHead->next, after = out->argHead; before != NULL;before = before->next, after = after->next)
	{
		after->next = new ArgNode;
		hit = s.second.find(before->name) ;
		if(hit != s.second.end())
		{
			after->next->name = hit->second;
			
			if(isVar(hit->second))
				after->next->type = TYPE_VAR;
			else
				after->next->type = TYPE_CONS;
		}
		else{
			after->next->type = before->type;
			after->next->name = before->name; 
		}
		after->next->next = NULL;
	}
	return out;
}












