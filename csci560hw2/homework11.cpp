#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include <sstream>
#include <cstdlib>

#include<list>
#include<limits>
using namespace std;
#define MAX numeric_limits<int>::max()
#define MIN numeric_limits<int>::min()

vector<vector<int>> values;
int N;
string mode;
char player;
int depthLimit;
vector<vector<int>> initialState;
int initialScore;
//solution////////////////////
vector<vector<int>>optimalNext;
string optimalAction;
pair<int, int> optimalLocation;

//////////////////////////////
int minValue(vector<vector<int>>&state, int currentValue, int depth);
int maxValue(vector<vector<int>>&state, int currentValue, int depth);
void preprocessing()
{
	
	string line;
	ifstream in("input.txt");
	int i, j;
	in >> N;//get the size of board
	getline(in, line);//receive enter

	getline(in, mode);// get mode

	in >> player;
	getline(in, line);

	in >> depthLimit;
	getline(in, line);

	values = vector<vector<int>>(N, vector<int>(N));
	
	for (i = 0; i < N;i++)
	{
		getline(in, line);
		istringstream str(line);
		for (j = 0; j < N; j++)
			str >> values[i][j];
	}

	initialState = vector<vector<int>>(N, vector<int>(N));
	initialScore = 0;
	for (i = 0; i < N; i++)
	{
		getline(in, line);
		for (j = 0; j < N; j++)
		{
			if (line[j] == player)
				initialState[i][j] = 1;
			else{
				if (line[j] == '.')
					initialState[i][j] = 0;
				else
					initialState[i][j] = -1;
			}
			
			//initialScore += initialState[i][j] * values[i][j];
		}

	}
	in.close();


}
vector<pair<int, int>> direction = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

void result_stake(vector<vector<int>>&state, int i, int j, int currentValue, int role, pair<string, int>&nextNode)
{
	state[i][j] = role;
	nextNode.first = "Stake";
	nextNode.second = currentValue + role * values[i][j];
}
bool result_raid(vector<vector<int>>&state, int i, int j, int currentValue, int role, pair<string, int>&nextNode, vector<pair<int,int>>&raid)
{
	string action;
	int score = currentValue;
	int tmp = 0;
	bool A = false, B = false;
	//vector<pair<int, int>> raid;
	
	raid.push_back(pair<int, int>(i,j));
	score += role * values[i][j];
	int x, y;
	
	for (auto a : direction)
	{
		x = i + a.first;
		y = j + a.second;
		if (x >= 0 && x < N && y >= 0 && y < N && state[x][y] != 0)
		{
			if (state[x][y] * role == -1)
			{
				tmp += 2 * state[x][y] * values[x][y];//2��
				raid.push_back(pair<int, int>(x, y));
				B = true;
			}
			else
				A = true;
		}
	}
	//raid
	if (A && B)
	{
		action = "Raid";
		vector<pair<int, int>>::iterator it;
		score -= tmp;
		for (it = raid.begin(); it != raid.end(); it++)
			state[it->first][it->second] = role;
		nextNode.first = action;
		nextNode.second = score;
		return true;
	}
	else
	{	
		//raid.clear();
		return false;
	}

		//action = "Stake";
	
	
	

}

void miniMax()
{
	
	int max = MIN;
	int tmp; 

	
	vector<vector<int>> next;
	pair<string, int> nextNode;

	int depth = 0;
	
	int i, j;
	for (i = 0; i < N;i++)
	for (j = 0; j < N; j++)
	{
		//scan with stake
		if (initialState[i][j] == 0)
		{

			//next = initialState;
			result_stake(initialState, i, j, initialScore, 1, nextNode);
			
			tmp = minValue(initialState, nextNode.second, depth + 1);
			
			if (tmp > max)
			{
				max = tmp;
				optimalNext = initialState;
				optimalAction = nextNode.first;
				optimalLocation = pair<int, int>(i, j);
			}
			initialState[i][j] = 0;
			if(i == 2 && j == 4)
			
				cout << "Stake "<< i << " " << j <<" : " << tmp << endl;

			
		}
	}
	vector<pair<int, int>>raid;
	vector<pair<int, int>>::iterator it;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (!initialState[i][j] && result_raid(initialState, i, j, initialScore, 1, nextNode, raid))
		{
			tmp = minValue(initialState, nextNode.second, depth + 1);
			if (tmp > max)
			{
				max = tmp;
				optimalNext = initialState;
				optimalAction = nextNode.first;
				optimalLocation = pair<int, int>(i, j);
			}
			//if(i == 1 && j == 2)
			
			//	cout << "Raid "<< i << " " << j <<" : " << tmp << endl;
			initialState[raid[0].first][raid[0].second] = 0;
			for (it = raid.begin()+1; it != raid.end(); it++)
			{
				initialState[it->first][it->second] = -1;
			}
			
		}
		
		raid.clear();
	}

}

int maxValue(vector<vector<int>>&state, int currentValue, int depth)
{
	//cout << depth << endl;
	//terminal check 1:reach the depth limit
	if (depth == depthLimit)
		return currentValue;

//	vector<vector<int>> next;
	pair<string, int> nextNode;

	int max = MIN;
	int tmp;

	int i, j;
	//stake scan
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (state[i][j] == 0)
		{

			//next = state;
			result_stake(state, i, j, currentValue, 1, nextNode);
			tmp = minValue(state, nextNode.second, depth + 1);
			if (tmp > max)
				max = tmp;

			state[i][j] = 0;
		}
	}
	//raid scan
	vector<pair<int, int>>raid;
	vector<pair<int, int>>::iterator it;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (!state[i][j] && result_raid(state, i, j, currentValue, 1, nextNode, raid))
		{
			tmp = minValue(state, nextNode.second, depth + 1);
			if (tmp > max)
				max = tmp;
			//if(i == 0 && j == 3 )
			
			//	cout << "Raid "<< i << " " << j <<" : " << max << endl;
			state[raid[0].first][raid[0].second] = 0;
			for (it = raid.begin() + 1; it != raid.end(); it++)
			{
				state[it->first][it->second] = -1;
			}
		}
		
		raid.clear();
	}
	//teminal chaeck 2: full
	return max != MIN ? max : currentValue;
}

int minValue(vector<vector<int>>&state, int currentValue, int depth)
{
	//terminal check 1
	if (depth == depthLimit)
		return currentValue;

	//vector<vector<int>> next;
	pair<string, int> nextNode;
	 
	int min = MAX;
	int tmp;

	int i, j;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (state[i][j] == 0)
		{

			//next = state;
			result_stake(state, i, j, currentValue, -1, nextNode);
			tmp = maxValue(state, nextNode.second, depth + 1);
		
			if (tmp < min)
				min = tmp;

			state[i][j] = 0;
				
		}
	}
	//raid scan
	vector<pair<int, int>>raid;
	vector<pair<int, int>>::iterator it;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (!state[i][j] && result_raid(state, i, j, currentValue, -1, nextNode, raid))
		{
			tmp = maxValue(state, nextNode.second, depth + 1);
			if (tmp < min)
				min = tmp;

			state[raid[0].first][raid[0].second] = 0;
			for (it = raid.begin() + 1; it != raid.end(); it++)
			{
				state[it->first][it->second] = 1;
			}
		}
		
		raid.clear();
	}
	// terminal check 2
	return min != MAX? min : currentValue;
}

int alphaBeta_min(vector<vector<int>>&state, int currentValue, int depth, int alpha, int beta);
int alphaBeta_max(vector<vector<int>>&state, int currentValue, int depth, int alpha, int beta);

void alphaBeta()
{
	//alpha records the max in mins
	//beta records the min in maxs
	int alpha = MIN, beta = MAX;
	int max = MIN;
	int tmp;
	//vector<vector<int>> next;
	pair<string, int> nextNode;
	int i, j;
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++)
	{
		if (initialState[i][j] == 0)
		{
			//next = initialState;
			result_stake(initialState, i, j, initialScore, 1, nextNode);
			tmp = alphaBeta_min(initialState, nextNode.second, 1, alpha, beta);
			
			if (tmp > max)
			{
				max = tmp;
				
				//alpha = max;
				optimalNext = initialState;
				optimalAction = nextNode.first;
				optimalLocation = pair<int, int>(i, j);
			}
			if (max > alpha) alpha = max;

			initialState[i][j] = 0;
		}
	}
	vector<pair<int, int>>raid;
	vector<pair<int, int>>::iterator it;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (!initialState[i][j] && result_raid(initialState, i, j, initialScore, 1, nextNode, raid))
		{
			tmp = alphaBeta_min(initialState, nextNode.second, 1, alpha, beta);

			

			if (tmp > max)
			{

				max = tmp;

				//alpha = max;
				optimalNext = initialState;
				optimalAction = nextNode.first;
				optimalLocation = pair<int, int>(i, j);
			}
			if (max > alpha) alpha = max;
			initialState[raid[0].first][raid[0].second] = 0;
			for (it = raid.begin() + 1; it != raid.end(); it++)
			{
				initialState[it->first][it->second] = -1;
			}
			
		}
		
		raid.clear();
	}
}

int alphaBeta_min(vector<vector<int>>&state, int currentValue, int depth, int alpha, int beta)
{
	//alpha records the max in mins
	//beta records the min in maxs
	if (depth == depthLimit)
		return currentValue;

	//vector<vector<int>> next;
	pair<string, int> nextNode;
	 
	int min = MAX;
	int tmp;
	int i, j;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (state[i][j] == 0)
		{
			//next = state;
			result_stake(state, i, j, currentValue, -1, nextNode);
			tmp = alphaBeta_max(state, nextNode.second, depth + 1, alpha, beta);
			state[i][j] = 0;
			if (tmp < min)
			{
				min = tmp;
				if (min <= alpha)
					return min;
				//else
				//	beta = min;
			}
			if (min < beta) beta = min;

		}
	}
	vector<pair<int, int>>raid;
	vector<pair<int, int>>::iterator it;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (!state[i][j] && result_raid(state, i, j, currentValue, -1, nextNode, raid))
		{
			tmp = alphaBeta_max(state, nextNode.second, depth + 1, alpha, beta);

			state[raid[0].first][raid[0].second] = 0;
			for (it = raid.begin() + 1; it != raid.end(); it++)
			{
				state[it->first][it->second] = 1;
			}
			if (tmp < min)
			{
				min = tmp;
				if (min <= alpha)
				{
					
					return min;
				}
				//else
				//	beta = min;
			}
			if (min < beta) beta = min;
		}
		
		raid.clear();
	}
	//terminal check
	return min != MAX ? min : currentValue;

}

int alphaBeta_max(vector<vector<int>>&state, int currentValue, int depth, int alpha, int beta)
{
	//alpha records the max in mins
	//beta records the min in maxs
	if (depth == depthLimit)
		return currentValue;

	//vector<vector<int>> next;
	pair<string, int> nextNode;

	int max = MIN;
	int tmp;
	int i, j;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (state[i][j] == 0)
		{
			//next = state;
			result_stake(state, i, j, currentValue, 1, nextNode);
			tmp = alphaBeta_min(state, nextNode.second, depth + 1, alpha, beta);
			state[i][j] = 0;
			if (tmp > max)
			{
				max = tmp;
				if (max >= beta)
					return max;
				//else
				//	alpha = max;
			}
			if (max > alpha) alpha = max;

		}
	}
	vector<pair<int, int>>raid;
	vector<pair<int, int>>::iterator it;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (!state[i][j] && result_raid(state, i, j, currentValue, 1, nextNode, raid))
		{
			tmp = alphaBeta_min(state, nextNode.second, depth + 1, alpha, beta);

			state[raid[0].first][raid[0].second] = 0;
			for (it = raid.begin() + 1; it != raid.end(); it++)
			{
				state[it->first][it->second] = -1;
			}

			if (tmp > max)
			{
				max = tmp;
				if (max >= beta)
				{
					return max;
				}
				//else
				//	alpha = max;
			}
			if (max > alpha) alpha = max;
		}
		
		raid.clear();
	}
	//terminal check
	return max != MIN ? max : currentValue;
}

void write()
{
	ofstream out("output.txt");
	string move;
	char opponent = player == 'X' ? 'O': 'X';
	move.push_back(optimalLocation.second + 65);
	move.push_back(optimalLocation.first + 49);
	out << move << ' ' << optimalAction << endl;
	vector<string> strategy(N);
	int i, j;
	for (i = 0; i < N; i++)
	for (j = 0; j < N; j++)
	{
		if (optimalNext[i][j] == 1)
			strategy[i].push_back(player);
		else{
			if (optimalNext[i][j] == -1)
				strategy[i].push_back(opponent);
			else
				strategy[i].push_back('.');
		}
	}
	for (int i = 0; i < N; i++)
		out << strategy[i] << endl;
	out.close();
}

int main()
{

	
	preprocessing();
	if (mode == "MINIMAX")
		miniMax();
	else
		alphaBeta();
	
	
	write();
	
	
	return 0;
}


