#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;



#define CVPLOT_HEADER_ONLY

#include "CvPlot/cvplot.h"

using namespace std;
const string SELECTED_LOG_FILENAME = "selected_individuals_ga.csv";
const string SHORT_GA_LEARNING_SCORES = "short_ga_learning_scores.csv";
const string NN_LEARNING_SCORES = "nn_learning_scores.csv";



void writeVectorToCSV(const std::vector<int>& data, const std::string& filename);
void plotGameScores(vector<int> scrPlayer1, vector<int>scrPlayer2, vector<int>iters, string title = "");