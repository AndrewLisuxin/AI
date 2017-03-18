#include"node.h"

//break apart conjunction and maintain a hash table for each rule

 long long varIndex;//variable standard, void name conflict

//fetch table

unordered_map<string,list<Clause *>> KB;// the first list is A(x), the second is ~A(x)
list<Literal *> queries;
/*rule

vector<pair<vector<vector<pair<pair<string, vector<string>>, bool>>>>, unordered_map <pair<string, bool>, vector<int>>>>;

*/

/*literal

vector<pair<pair<string, vector<string>>, bool>>>

*/

/*predicate location index table

unordered_map <pair<string, bool>, vector<int>>

*/

queue<Node *> que;

unordered_map<string,list<Clause *>>::iterator cla_it ;
unordered_map<string, vector<int>>::iterator iter;
unordered_map<string, string>::iterator it;

string key;
int number;
Clause *ac;
Node *q;
Literal *al;
bool repeat;

//void exeRoot(Node *);
void tell(Node *p)
{
	// break apart a sentence into clauses and store them into KB
	if(p->name == "&")
	{
		tell(p->uni.object.left);
		tell(p->uni.object.right);
	}
	else{// one clause
		ac = new Clause;
		ac->varIndexRange = pair<long long,long long>(-1,-2);
		unordered_map<string, string> varNameTrans;// for this clause
		number = 0;
		
		// BFS gets the whole clause
		que.push(p);
		
		while(!que.empty())
		{
			q = que.front();
			que.pop();
			if(q->name == "~" || q->type == TYPE_PRE)
			{
				// one literal
				al = new Literal;
				if(q->name == "~")
				{
					al->isYes = 0;
					q = q->uni.object.left;
				}  	
				else 
					al->isYes = 1;

				//predicate
				al->pre = q->name;

				
				// construct key
				key = al->pre + to_string(al->isYes);
			//cout<<key<<endl;
				iter = ac->index.find(key);
				//the position of this literal in the clause
				
				if(iter == ac->index.end())
				{
					ac->index.insert(pair<string, vector<int>>(key,vector<int>{number++}));
					repeat  = false;
				}
				else{
					iter->second.push_back(number++);
					repeat = true;
				}
				//which clauses contain this literal
				
				
				cla_it = KB.find(key);
				if(cla_it == KB.end())
					KB.insert(pair<string,list<Clause *>>(key,list<Clause *>{ac}));
				else
					if(!repeat)
						cla_it->second.push_back(ac);
			//cout<<al->pre<<endl;					


				//arguments
				ArgNode *arg = q->uni.arg, *tail;
				al->argHead = new ArgNode;
				tail = al->argHead;
				ArgNode *a;
				//cout<<"----------------------hello--------------------"<<endl;
				
				
				while(arg != NULL)
				{
				        a = new ArgNode;
					a->type = arg->type;
					if(arg->type == TYPE_VAR)
					{
					
						it = varNameTrans.find(arg->name);
						if(it != varNameTrans.end())
							a->name = it->second;
						else{
							if(ac-> varIndexRange.first == -1)
								ac-> varIndexRange.first = varIndex;
							a->name ="x" + to_string(varIndex);
							ac-> varIndexRange.second = varIndex;
							varIndex++;
							varNameTrans.insert(pair<string,string>(arg->name,a->name));

						}
//cout<<"----------------------here1--------------------"<<endl;
					}
					else
						a->name = arg->name;

					a->next = NULL;
    					tail->next = a;
					tail = a;

					arg = arg->next;
//cout << arg << endl;
				}

				ac->literals.push_back(al);
				ac->literals2.push_back(al);
//cout<<"----------------------here2--------------------"<<endl;
			}
			else{//  "|"
				que.push(q->uni.object.left);
				que.push(q->uni.object.right);
//cout<<"----------------------hello--------------------"<<endl;
			}
		}	
		varNameTrans.clear();	
	}
	
}

void query()
{
//cout<<"----------------------query--------------------"<<endl;
		//		 exeRoot(aQuery);
	
				Literal *al = new Literal;
				Node *q = aQuery, *p;
				if(q->name == "~")
				{
					al->isYes = 0;
					p = q;
					q = p->uni.object.left;
					delete(p);
				}  	
				else 
					al->isYes = 1;

				//predicate
				al->pre = q->name;
				
				

				//arguments
				al->argHead = new ArgNode;
				al->argHead->next = q->uni.arg;
				
				delete(q);
				queries.push_back(al);

}

