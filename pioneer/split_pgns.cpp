#include "split_pgns.h"
#include "utils/split.h"

using namespace std;

vector<string> splitPgns(const string& pgns) 
{
	vector<string> splits = split(pgns, "\n\n");
	vector<string> pgn_s;

	for (int i = 0; i < splits.size(); i++) 
	{
		if ((i & 0b1) == 1) 
		{
			pgn_s.push_back(splits[i]);
		}
	}

	return pgn_s;
}
