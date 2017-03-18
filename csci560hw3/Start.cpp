//#include"node.h"
//#include<stdlib.h>
//#include<stdarg.h>
//#include"yacchw3"
//#include<string>
//using namespace std;
#include"node.h"
void read_to_buffer(const char*s);
void  parse();
//void exeRoot(Node* root);
Node *aRule;
Node *aQuery;
void query();
void tell(Node *);
bool kb_or_query;// 0: query ; 1: kb 
bool Resolution(Literal *query);

void read()
{
	int num_qu, num_kb, i, length;
	string line, rule;
	ifstream fin("input.txt");


	fin >> num_qu;
	getline(fin, line);
	kb_or_query = false;
	while(num_qu-- > 0)
	{
		getline(fin, line);
		//preprocess: eliminate all spaces one pass
		length = line.length();
	
		rule.clear();
		for(i = 0;i < length;i++)
			if(line[i] != '\40')
				rule.push_back(line[i]); 
		///////////////

		read_to_buffer(rule.c_str());

		parse();

		query();
	}
	
	fin >> num_kb;
	getline(fin, line);
	kb_or_query = true;
	varIndex = 0;
	while(num_kb-- > 0)
	{
		getline(fin, line);
		//preprocess: eliminate all spaces one pass
		length = line.length();
		
		rule.clear();
		for(i = 0;i < length;i++)
			if(line[i] != '\40')
				rule.push_back(line[i]); 
		///////////////
		read_to_buffer(rule.c_str());
		parse();
		tell(aRule);
	}
	cout << varIndex<<endl;	
}
void write(vector<string> &out)
{
	ofstream fout("output.txt");
	for(vector<string>::iterator iterOut = out.begin(); iterOut != out.end(); iterOut++)
		fout<< *iterOut <<endl;
	


}
int main()
{

	read();
	
	//exeRoot(aRule);
	cout << "------------reasoning results------------------"<<endl;
	//cout << queries.size()<<endl;
	
	vector<string> out;
	for(list<Literal*>::iterator it = queries.begin();it != queries.end(); it++)
		if(Resolution(*it))
			out.push_back("TRUE");
		else
			out.push_back("FALSE");

	write(out);
	return 0;

}

