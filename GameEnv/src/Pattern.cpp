#include"Pattern.h"



/*

void PatternSet::writePattern(vector<PatternSet> inpP)
{
	std::string filename = "patterns\\patterns_" + to_string(inpP.size()) + ".txt";

	std::fstream fs;
	fs.open(filename);

	for (auto A : inpP)
	{
		string mapP = "";
		mapP += to_string(A.patternLength) + "\t" + to_string(A.patterns.size()) + "\n";
		for (auto p : A.patterns)
		{
			for (auto s : p.first)
				mapP += to_string(s) + " ";
			mapP += "\t";
			mapP += to_string(p.second) + "\n";
		}
		fs << mapP << endl;
	}
	fs.close();
}

vector<PatternSet> PatternSet:: readPattern(int n)
{
	vector<PatternSet> res(n);
	std::string filename = "patterns\\patterns_" + to_string(n) + ".txt";

	std::ifstream fs;
	fs.open(filename);
	if (!fs.is_open())
	{
		cout << "file open error\n";
		return res;
	}
	int size;
	for (auto& B : res)
	{
		int plen;
		fs >> plen >> size;
		PatternSet A = PatternSet(plen);
		//temp = PatternWeight(A.patternLength);
		for (auto& p : A.patterns)
		{
			vector<PatternState> ps;
			for (int j = 0; j < plen; j++)
			{
				int t;
				fs >> t;
				// ps.push_back(t);
			}
			fs >> p.second;
		}
		B = A;
	}
	return res;
}

vector<PatternState> PatternSet::randomPattern(int len)
{
	vector<PatternState> p;
	for (int i = 0; i < len; i++)
		p.push_back(ALL_PATTERN_STATES[rand() % ALL_PATTERN_STATES.size()]);
	return p;
}
*/