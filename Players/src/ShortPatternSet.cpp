#include "ShortPatternSet.h"

ShortPatternSet::ShortPatternSet(int patternLength) {
	this->patternLength = patternLength;


	vector<vector<PatternState>> allPatterns = generatePatterns(patternLength);
	for (auto pattern : allPatterns)
		this->weightedPatterns[pattern] = (rand() % 100 - 50) * 0.1; //-5.0 to 5.0

}

vector<Pattern> ShortPatternSet::generatePatterns(int patternLength)
{
	vector<Pattern> temp, nextPatterns;
	if (patternLength > 1)
	{
		temp = generatePatterns(patternLength - 1);
		for (auto pattern : temp)
			for (auto state : ALL_PATTERN_STATES)
			{
				Pattern currentPattern = pattern;
				currentPattern.insert(currentPattern.begin(), state);
				nextPatterns.push_back(currentPattern);
			}
	}
	else
	{
		for (auto state : ALL_PATTERN_STATES)
		{
			Pattern currentPattern;
			currentPattern.insert(currentPattern.begin(), state);
			nextPatterns.push_back(currentPattern);
		}
	}
	return nextPatterns;
}



void ShortPatternSet::savePatternSets(vector<ShortPatternSet> patternSets, string series)
{
	std::string filename = "patterns\\" + series + "\\patterns_" + to_string(patternSets.size()) + "x" + to_string(patternSets[0].patternLength) + ".txt";
	cout << filename << endl;
	std::fstream fs;

	fs.open(filename, std::fstream::out);

	for (auto patternSet : patternSets)
	{
		string mapP = "";
		mapP += to_string(patternSet.patternLength) + "\t" + to_string(patternSet.weightedPatterns.size()) + "\n";
		for (auto p : patternSet.weightedPatterns)
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

void ShortPatternSet::saveSinglePatternSet(ShortPatternSet patternSet, string fileSufix, string series)
{
	std::string filename = "patterns\\" + series + "\\single_" + to_string(patternSet.patternLength)+"_" + fileSufix + ".txt";
	cout << filename << endl;
	std::fstream fs;

	fs.open(filename, std::fstream::out);


	string mapP = "";
	mapP += to_string(patternSet.patternLength) + "\t" + to_string(patternSet.weightedPatterns.size()) + "\n";
	for (auto p : patternSet.weightedPatterns)
	{
		for (auto s : p.first)
			mapP += to_string(s) + " ";
		mapP += "\t";
		mapP += to_string(p.second) + "\n";
	}
	fs << mapP << endl;

	fs.close();
}

vector<ShortPatternSet> ShortPatternSet::loadPatternSets(int n, int patternLength, string series)
{
	vector<ShortPatternSet> res(n);
	std::string filename = "patterns\\" + series + "\\patterns_" + to_string(n) + "x" + to_string(patternLength) + ".txt";

	std::ifstream fs;
	fs.open(filename, std::fstream::in);
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
		ShortPatternSet A = ShortPatternSet(plen);

		for (auto& p : A.weightedPatterns)
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

ShortPatternSet ShortPatternSet::loadSinglePatternSet(int patternLength, string fileSufix, string series)
{

	std::string filename = "patterns\\" + series + "\\single_" + to_string(patternLength) + "_" + fileSufix + ".txt";


	std::ifstream fs;
	fs.open(filename, std::fstream::in);
	if (!fs.is_open())
	{
		cout << "file open error\n";
		return ShortPatternSet();
	}
	int size;


	int plen;
	fs >> plen >> size;
	ShortPatternSet A = ShortPatternSet(plen);

	for (auto& p : A.weightedPatterns)
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
	return A;
}

int ShortPatternSet::getNumPatterns()
{
	return weightedPatterns.size();
}
