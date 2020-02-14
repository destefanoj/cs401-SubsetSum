#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

// seenBefore: determines whether or not we have a distinct vector
bool seenBefore (vector< vector <int> > minSubsets, vector<int> s){
    // checks if we have seen a subset previously
        // if we have, we return true
    for (int i = 0; i < minSubsets.size(); i++){
        if (minSubsets[i] == s){
            return true;
        }
    }
    return false;
}

// minSubset: builds a vector of minimum subsets (a vector of vectors)
    // Also computes the total number of minimum subsets and upsets the distinct minimum count
void minSubset (vector<int> subset, vector< vector<int> > &minSubsets,
                unsigned long &currMin, unsigned long &minCount, unsigned long &distMinCount)
{
    // If the size of the vector is smaller than the current minimum,
     // then we have found a smaller subset sum.
    if (subset.size() < currMin){
        // reset all the values
        currMin = subset.size();
        minCount = 1;
        distMinCount = 1;

        // clear the vector that contains all the minimum subsets,
            // and add the new minimum subset to it
        minSubsets.clear();
        minSubsets.push_back(subset);
    }

    // if the values are equal, then we have found another minimum value
    else if (subset.size() == currMin){
        minCount++;

        // check if we have seen this subset before
            // if we haven't, we increment the distinct minimum count
        if (!seenBefore(minSubsets, subset)){
            distMinCount++;
            minSubsets.push_back(subset);
        }
    }

    //** Note: We do nothing for the subsets that are greater than the currMin **//
}

void findAllSubsetSumHelper(bool **subproblem, vector<int> a, int i, int target, vector<int> &subsets,
                            vector < vector<int> > &minSubsets, unsigned long &totalCount,
                            unsigned long &currMin, unsigned long &minCount, unsigned long &distMinCount)
{
    // ** Base if the target sum is not 0, and the dynamic array is true
    if (i == 0 && target != 0 && subproblem[0][target])
    {
        subsets.push_back(a[i]);
        totalCount++; // Counts the total subsets

        // counts the minimum subsets and determines the distinct minsubsets
        minSubset(subsets, minSubsets, currMin, minCount, distMinCount);
        return;
    }

    // **Base if the target sum hits zero due to the second recurrsive call
    if (i == 0 && target == 0)
    {
        totalCount++; // Counts the total subsets

        // counts the minimum subsets and determines the distinct minsubsets
        minSubset(subsets, minSubsets, currMin, minCount, distMinCount);
        return;
    }

    if (subproblem[i-1][target])
    {
        vector<int> newSubset = subsets;
        findAllSubsetSumHelper(subproblem, a, i - 1, target, newSubset, minSubsets, totalCount, currMin, minCount, distMinCount);
    }

    if (target >= a[i] && subproblem[i-1][target-a[i]])
    {
        subsets.push_back(a[i]); // pushes back single element
        findAllSubsetSumHelper(subproblem, a, i - 1, target - a[i], subsets, minSubsets, totalCount, currMin, minCount,
                               distMinCount);
    }
}

// findAllSubsetSum: implements the standard subset sum algorithm. However, instead of returning true or
    // false, a recurrsive helper function is called that builds the different subsets to add to target
    // True or false is returned only in determining if building the subset was feasible.
bool findAllSubsetSum(bool** subproblem, vector<int> a, vector<vector<int> > &minSubsets,
                      unsigned long int sizeOfA, int target, unsigned long &totalCount,
                      unsigned long &currMin, unsigned long &minCount, unsigned long &distMinCount)
{
    if (sizeOfA == 0 || target < 0)
        return false;

    for (int i = 0; i < sizeOfA; i++){
        subproblem[i][0] = true;
    }

    // sum of 0 is true
    if (a[0] <= target)
        subproblem[0][a[0]] = true;

    // builds the true and false table as per
        // the standard subset-sum algorithm
    for (int i = 1; i < sizeOfA; ++i) {
        for (int j = 0; j < target + 1; ++j) {
            if (a[i] <= j){
                subproblem[i][j] = subproblem[i-1][j] || subproblem[i-1][j-a[i]];
            }
            else {
                subproblem[i][j] = subproblem[i-1][j];
            }
        }
    }
    if (!subproblem[sizeOfA - 1][target])
    {
        cout << "Not feasible\n";
        return false;
    }

    // We will call a recursive function which stores the valid sums into a
        // subsets vector
    vector<int> subsets;
    findAllSubsetSumHelper(subproblem, a, sizeOfA - 1, target, subsets, minSubsets, totalCount, currMin, minCount, distMinCount);
    return true;
}


int main(int argc, char *argv[]) {
    int val = 0;
    string fileLine;
    int T;

    vector<int> votes;

    if (argc <= 2) { cerr << "Not all required information was entered" << endl; }
    else if (argc > 3) { cerr << "Too much information entered" << endl; }

    T = atoi(argv[1]);

    // Read in the datafile
    // Because we only need to provide the sizes of the arrays, I chose to only read
        // in the number of votes for simplicity.
    ifstream inFile(argv[2]);
    if (inFile.is_open()){
        while (getline(inFile, fileLine)){
            istringstream iStream(fileLine);
            iStream >> val;
            votes.push_back(val);
        }
    }
    else {
        cout << "Failed to open file." << endl;
        exit (-1);
    }

    unsigned long int size = votes.size();

    // Create a subproblem array
        // Dynamically created because we don't know what the size
        // of the input is, and we don't know what T is
    bool **subproblem;

    subproblem = new bool*[size];
    for (int i = 0; i < size; i++){
        subproblem[i] = new bool[T+1];
    }

    // Variable to track the total count of subsets
    unsigned long totalCount = 0;

    // We will start by assuming that the smallest subset sum is the size of the entire vector
    unsigned long int currMin = size;

    // Keep track of the total number of subsets sums of minimum size
    unsigned long minCount = 0;

    // Variable to keep track of distinct minimum subsets
    unsigned long distMinCount = 0;

    vector <vector<int> > subsetsOfMinSize;

    if (findAllSubsetSum(subproblem, votes, subsetsOfMinSize, size, T, totalCount, currMin, minCount, distMinCount)){
        cout << "\nYay, there exist subsets that satisfy the subset-sum problem!" << endl;

        if (totalCount == 1) { cout << "There is " << totalCount << " subset that that adds to " << T << "." << endl; }
        else { cout << "There are " << totalCount << " total subsets that add to " << T << "." << endl; }

        cout << "The size of the smallest subset is " << currMin << "." << endl;

        if (distMinCount == 1) { cout << "There is " << distMinCount << " distinct subset that that adds to " << T << " of size " << currMin << "." << endl; }
        else { cout << "There are " << distMinCount << " total distinct subsets that that add to " << T << " of size " << currMin << ".\n" << endl; }

    }
    else { cout << "\nSorry, but this is not feasible with given target.\n" << endl; }

    return 0;
}
