#include<omp.h>

#include "Evolution.h"

#include"Eigen/Dense"


using namespace Eigen;

vector<int> Evolution::selectNextGenIdx(vector<pair<double, int>> rating)
{
    vector<int> nextGenIdx;

    for (int i = 0; i < evoConf.numParents; i++)
    {
        nextGenIdx.push_back(rating[i].second);
    }
    return nextGenIdx;
}

vector<pair<double, int>> Evolution::ratePlayers(vector<vector<int>> tournamentTable)
{
    vector<pair<double, int>> rating;
    int highRatingCnt = 0;
    int numPlayers = tournamentTable.size();
    for (int row = 0; row < numPlayers; row++)
    {
        vector<int> vDif;
        ArrayXi tempResults = ArrayXi(numPlayers);

        for (int j = 0; j < numPlayers; j++)
        {
            tempResults(j) = MIN_VALUE;

            if (max(tournamentTable[row][j], tournamentTable[j][row]) != 0)
                tempResults(j) = ((tournamentTable[row][j] - tournamentTable[j][row]) * 3 + tournamentTable[row][j]);
        }

        int gamesCntr = (tempResults > MIN_VALUE).count();

        double avgScore = tempResults.sum() / gamesCntr;

        rating.push_back({ avgScore ,row });
    }


    sort(rating.begin(), rating.end());
    reverse(rating.begin(), rating.end());

    return rating;

}

vector<ShortPatternSet> Evolution::repopulate(vector<vector<int>> tournamentTable, vector<ShortPatternSet> oldPopulation, int logSelectedIndividuals)
{
    vector<pair<double, int>> rating = ratePlayers(tournamentTable);
    vector<int> selectedIdx = selectNextGenIdx(rating);
    
    if (logSelectedIndividuals)
        writeVectorToCSV(selectedIdx, SELECTED_LOG_FILENAME);

    vector<ShortPatternSet> nextGeneration;

    //add selected 
    for (int i : selectedIdx)
    {
        nextGeneration.push_back(oldPopulation[i]);
    }

    //add mutation
    for (int i : selectedIdx)
    {
        nextGeneration.push_back(mutate(oldPopulation[i]));
    }
    //crossover
//#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < evoConf.numCrossovers; i++)
    {
        int a, b;
        a = rand() % evoConf.numParents;
        do
        {
            b = rand() % evoConf.numParents;
        } while (b == a);

        ShortPatternSet offspring = crossover(oldPopulation[a], oldPopulation[b]);
//#pragma omp critical
        {
            nextGeneration.push_back(offspring);
        }
    }
    return nextGeneration;
}

ShortPatternSet Evolution::mutate(ShortPatternSet originalPatterns)
{
    int numPatterns = originalPatterns.getNumPatterns();
    int numMutations = evoConf.mutatedValuesShare * numPatterns;
  
  
    // create a vector of pairs to store the elements of map 
    vector<pair<Pattern, double> > pWeights;


    copy(originalPatterns.weightedPatterns.begin(), originalPatterns.weightedPatterns.end(), back_inserter(pWeights));


    for (int i = 0; i < numMutations; i++)
    {
        int mutIdx = rand() % numPatterns;
        double currentWeight = pWeights[mutIdx].second;

        double range = abs(currentWeight) * evoConf.mutationRangePercent;
        range = max(range, evoConf.minMutaionRange);
        range = min(range, evoConf.maxMutaionRange);
        double newWeight = (rand() % 200 - 100) * 0.01 * range;

        originalPatterns.weightedPatterns[pWeights[mutIdx].first] += newWeight;
        originalPatterns.weightedPatterns[pWeights[mutIdx].first] = max(originalPatterns.weightedPatterns[pWeights[mutIdx].first], evoConf.minWeight);
        originalPatterns.weightedPatterns[pWeights[mutIdx].first] = min(originalPatterns.weightedPatterns[pWeights[mutIdx].first], evoConf.maxWeight);

    }
    return originalPatterns;
}

ShortPatternSet Evolution::crossover(ShortPatternSet parrentA, ShortPatternSet parrentB)
{
    int numPatterns = parrentA.getNumPatterns();

    vector<pair<Pattern, double> > weightsA, weightsB;
    ShortPatternSet offspring = parrentA;
    double alpha = (rand() % 150 - 25) * 0.01;

    copy(parrentA.weightedPatterns.begin(), parrentA.weightedPatterns.end(), back_inserter(weightsA));
    copy(parrentB.weightedPatterns.begin(), parrentB.weightedPatterns.end(), back_inserter(weightsB));
    for (int i=0;i<numPatterns;i++){
        Pattern p = weightsA[i].first;
        offspring.weightedPatterns[p] += alpha * (weightsB[i].second - weightsA[i].second);
        offspring.weightedPatterns[p] = max(offspring.weightedPatterns[p], evoConf.minWeight);
        offspring.weightedPatterns[p] = min(offspring.weightedPatterns[p], evoConf.maxWeight);

    }

    return offspring;
}

Evolution::Evolution(EvoConfig evolutionConfig)
{
    this->evoConf = evolutionConfig;
}
