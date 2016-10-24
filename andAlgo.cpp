#include "andAlgo.h"

#include <algorithm>
#include <iterator>
#include <cmath>

int** maxPal;

void initDPArray(int maxWordLength)
{
    maxPal = new int * [maxWordLength];
    for (int i = 0; i < maxWordLength; i++)
        maxPal[i] = new int[maxWordLength];
}

void deleteDPArray(int maxWordLength)
{
    for (int i = 0; i < maxWordLength; i++)
        delete[] maxPal[i];
    delete[] maxPal;
}

int longestCommonSubseq(const vector<bool> & word1, const vector<bool> & word2)
{
    if (word1[0] == word2[0])
        maxPal[0][0] = 1;
    else
        maxPal[0][0] = 0;
    
    for (int i = 1; i < word1.size(); i++)
    {
        if (word1[i] == word2[0])
            maxPal[i][0] = 1;
        else
            maxPal[i][0] = maxPal[i - 1][0];
    }
    
    for (int j = 1; j < word2.size(); j++)
    {
        if (word1[0] == word2[j])
            maxPal[0][j] = 1;
        else
            maxPal[0][j] = maxPal[0][j - 1];
    }
    
    for (int sum = 2; sum < word1.size() + word2.size() - 1; sum++)
    {
        int minI = max(1, sum - (int)word2.size() + 1);
        for (int i = minI; i < word1.size(); i++)
        {
            int j = sum - i;
            if (j < 1)
                break;
            if (word1[i] == word2[j])
                maxPal[i][j] = maxPal[i - 1][j - 1] + 1;
            else
                maxPal[i][j] = max(maxPal[i - 1][j], maxPal[i][j - 1]);
        }
    }
    
    return maxPal[word1.size() - 1][word2.size() - 1];
}

int findMaxSquareSubsequence(const vector<bool> & word, bool inverse)
{
    int maxSquareLength = 0;
    for (int iMiddle = 1; iMiddle < word.size() - 1; iMiddle++)
    {
        vector<bool> part1(word.begin(), word.begin() + iMiddle),
                part2(word.begin() + iMiddle, word.end());
        if (inverse)
        {
            for (int i = 0; i < part2.size(); i++)
            {
                part2[i] = 1 - part2[i];
            }
        }
        maxSquareLength = max(maxSquareLength, longestCommonSubseq(part1, part2));
    }
    
    return 2 * maxSquareLength;
}

int findMaxPalindrome(const vector<bool> & word, bool checkPalindrome)
{
    vector<bool> doubledWord;
    doubledWord.insert(doubledWord.end(), word.begin(), word.end());
    doubledWord.insert(doubledWord.end(), word.begin(), word.end());
    
    for (int i = 1; i < doubledWord.size(); i++)
        maxPal[i][i - 1] = 0;
    
    if (checkPalindrome)
    {
        for (int i = 0; i < doubledWord.size(); i++)
        {
            maxPal[i][i] = 1;
        }
    }
    else
    {
        for (int i = 0; i < doubledWord.size(); i++)
        {
            maxPal[i][i] = 0;
        }
    }
    
    if (checkPalindrome)
    {
        for (int sum = 1; sum < word.size(); sum++)
        {
            for (int i = 0; i < doubledWord.size(); i++)
            {
                int j = i + sum;
                if (j >= doubledWord.size())
                    break;
                
                if ((doubledWord[i] == doubledWord[j]))
                    maxPal[i][j] = maxPal[i+1][j-1] + 2;
                else
                {
                    maxPal[i][j] = max(maxPal[i + 1][j], maxPal[i][j - 1]);
                }
            }
        }
    }
    else
    {
        for (int sum = 1; sum < word.size(); sum++)
        {
            for (int i = 0; i < doubledWord.size(); i++)
            {
                int j = i + sum;
                if (j >= doubledWord.size())
                    break;
                
                if ((doubledWord[i] != doubledWord[j]))
                    maxPal[i][j] = maxPal[i+1][j-1] + 2;
                else
                {
                    maxPal[i][j] = max(maxPal[i + 1][j], maxPal[i][j - 1]);
                }
            }
        }
    }
    
    int answer = 0;
    for (int iStart = 0; iStart < word.size(); iStart++)
    {
        answer = max(answer, maxPal[iStart][iStart + word.size() - 1]);
    }
    return answer;
}

inline bool lexicographicalCompare(bIter first1, bIter last1,
              bIter first2, bIter last2)
{
      bIter i2 = first2;
      for (bIter i1 = first1; (i1 != last1) && (i2 != last2); i1++)
      {
          if (*i1 < *i2)
              return true;
          else if (*i1 == *i2)
              i2++;
          else
              return false;
      }
      // the word is periodic
      // a periodic word can not be a minimum counterexample -- return false
      // (for both palindromes and antipalindromes)
      // but causes problems with estimation
      return true;
}

bool theWordIsTheLeast(const vector<bool> & word)
{
    if (word[0] == 1)
        return false;
    
    for (int shift = 1; shift < word.size(); shift++)
    {
        if (!lexicographicalCompare(word.begin(), word.end(),
                                    word.begin() + shift, word.end()))
        {
            return false;
        }
    }
    
    return true;
}

