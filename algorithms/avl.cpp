#include<iostream>
#include<stack>
#include<vector>
using namespace std;

class AVL{
private:
	struct Node{
		int val;	
		int height;
		Node* left;
		Node* right;
		Node(int v): val(v), height(1), left(NULL), right(NULL){};

	};
	
	int replace;	
public:
	Node * dummy;
	AVL()
	{
		dummy = new Node(0);
	}

Node* rotation(Node* root)
{
	if(!root)
		return root;
	int l = root->left ? root->left->height : 0;
	int r = root->right ? root->right->height : 0;
	if(l == r + 2)
	{
		return left_part(root);
	} 
	else{
		if(l + 2 == r)
		{
			return right_part(root);
		}
	}
	return root;
}

Node* left_part(Node* root)
{	
	int l_l = root->left->left? root->left->left->height : 0;
	int l_r = root->left->right? root->left->right->height : 0;
	if(l_l < l_r)
	{
		Node* tmp = root->left;
		root->left = tmp->right;
		tmp->right = root->left->left;
		root->left->left = tmp;
		tmp->height--;
		root->left++;
	}
	Node* tmp = root->left;
	root->left = tmp->right;
	tmp->right = root;
	root->height -= 2;
	return tmp;
}
Node* right_part(Node* root)
{
	int r_l = root->right->left? root->right->left->height : 0;
	int r_r = root->right->right ? root->right->right->height : 0;
	if(r_l > r_r)
	{
		Node* tmp = root->right;
		root->right = tmp->left;
		tmp->left = root->right->right;
		root->right->right = tmp;
		tmp->height--;
		root->right++;
	}
	Node* tmp = root->right;
	root->right = tmp->left;
	tmp->left = root;
	root->height -= 2;
	return tmp;
}


Node* insert(Node* root, int i)
{
	if(!root)
		return new Node(i);
	else{
		if(root->val > i)
		{
			root->left = insert(root->left, i);
			root->height = max(root->left->height + 1, root->height);
		}
		else
		{
			root->right = insert(root->right, i);
			root->height = max(root->right->height + 1, root->height);
		}
		return rotation(root);
	}
}
		
Node* deletion(Node* root, int i, bool find)
{	
	if(!find)
	{
		if(!root)
			return root;
		if(root->val > i)
			root->left = deletion(root->left, i, find);
		else{
			if(root->val < i)
				root->right = deletion(root->right, i, find);
			else{
				Node* tmp;
				if(!root->left)
				{	
					tmp = root->right;
					delete root;
					return tmp;
				}
				else{
					if(!root->right)
					{
						tmp = root->left;
						delete root;
						return tmp;
					}
					else{
						root->right = deletion(root->right, i, true);
						root->val = replace;
					
								
					}
						
				}
			}
		}
	}
	else{
		if(!root->left)
		{
			replace = root->val;
			Node* tmp = root->right;
			delete root;
			return tmp;
		}
		else{
			root->left = deletion(root->left, i, find);
		}
	}
	return rotation(root);
}


void display()
{
	vector<int> res;
	stack<pair<Node*, bool>> rec;
	if(dummy->left)
		rec.emplace(dummy->left, false);
	pair<Node*, bool> cur;
	while(!rec.empty())
	{
		cur = rec.top();
		rec.pop();
		if(cur.second)
			res.push_back(cur.first->val);
		else{
			if(cur.first->right)
				rec.emplace(cur.first->right, false);
			rec.emplace(cur.first, true);
			if(cur.first->left)
				rec.emplace(cur.first->left, false);
		}
	}
	for(auto& i : res)
		cout << i << " " ;
}
};

int main()
{
	AVL *a = new AVL();
	for(int i = 0; i < 20; i++)
		a->dummy->left = a->insert(a->dummy->left, i);
	vector<int>de{2,4,6,7,88,4,6,21,6,8};
	for(auto& i : de)
	a->dummy->left = a->deletion(a->dummy->left, i, false);
	
	a->display();	
	return 0;
}



