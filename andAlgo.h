#ifndef ANDALGO_H
#define ANDALGO_H

#include <vector>
using namespace std;

// memory initialization -- we do it just once to save time
void initDPArray(int maxWordLength);
void deleteDPArray(int maxWordLength);

// main algorithms for conjectures checking
int findMaxSquareSubsequence(const vector<bool> & word, bool inverse);
int findMaxPalindrome(const vector<bool> & word, bool checkPalindrome);

int longestCommonSubseq(const vector<bool> & word1, const vector<bool> & word2);

// functions for working with cyclic word
typedef vector<bool>::const_iterator bIter;
inline bool lexicographicalCompare(bIter first1, bIter last1, 
                                   bIter first2, bIter last2);
bool theWordIsTheLeast(const vector<bool> & word);

#endif // ANDALGO_H
