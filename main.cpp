#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "andUtils.h"
using namespace std;

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


typedef vector<bool>::const_iterator bIter;

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

void checkBruteforce(int minLength, int maxLength, bool checkSquares, bool checkPalindromes = true)
{
    double totalTime1 = get_wall_time();
    
    vector<bool> word;
    
    vector<int> minPalForLength;
    minPalForLength.push_back(0);
    
    for (int wordLength = minLength; wordLength <= maxLength; wordLength += 2)
    {
        if (!checkSquares)
        {
            // can cause problems with periodic words
            double estimatedMinPalLength = ((double)minPalForLength[(wordLength - minLength) / 2]) / wordLength;
            double conjFactor;
            if (checkPalindromes)
                conjFactor = (double)3/4;
            else
                conjFactor = (double)2/3;
            
            if (estimatedMinPalLength >= conjFactor)
            {
                cout << wordLength << " skipped with factor at least" << " " 
                     << (double)minPalForLength[(wordLength - minLength) / 2] / wordLength << endl;
                minPalForLength.push_back(minPalForLength[(wordLength - minLength) / 2]);
                continue;
            }
        }
        
        double time1 = get_wall_time();
        
        int minPalLength = 100;
        word.resize(wordLength);
        
        for (int iLetter = 0; iLetter < wordLength / 2; iLetter++)
        {
            word[iLetter] = 0;
        }
        for (int iLetter = wordLength / 2; iLetter < wordLength; iLetter++)
        {
            word[iLetter] = 1;
        }
        
        if (checkSquares)
        {
            do {
                if (theWordIsTheLeast(word))
                {
//                    numLyndonWords++;
                    int squareLength = 0;
                    
//                    int a1 = 0, a2 = 0;
                    for (int shift = 1; shift < word.size() + 1; shift++)
                    {
                        // for linear words -- counterex for 28
                        squareLength = max(squareLength, findMaxSquareSubsequence(word, true));
                        squareLength = max(squareLength, findMaxSquareSubsequence(word, false));
                        
    //                    a1 = max(a1, findMaxSquareSubsequence(word, true));
    //                    a2 = max(a2, findMaxSquareSubsequence(word, false));
                        
                        rotate(word.begin(), word.begin() + 1, word.end());
                    }
                    
    //                if (((double)a1 / word.size() <= 0.667)
    //                        && ((double)a2 / word.size() <= 0.667))
    //                {
    //                    cout << a1 << " " << a2 << " " << word << endl;
    //                    //exit(0);
    //                }
                    
                    minPalLength = min(minPalLength, squareLength);
                }
            } while (next_permutation(word.begin(), word.end()));
        }
        else
        {
            int numLyndonWords = 0;
            do {
                if (theWordIsTheLeast(word))
                {
                    //numLyndonWords++;
                    int palindromeLength;
                    
                    // is max-of-two true for linear words?
                    palindromeLength = findMaxPalindrome(word, checkPalindromes);
                    
                    // bound is achieved at the same time?
                    //int palindromeLength1 = findMaxPalindrome(word);
                    //int palindromeLength2 = findMaxAntiPalindrome(word);
                    //palindromeLength = max(palindromeLength1, palindromeLength2);
                    
                    minPalLength = min(minPalLength, palindromeLength);
                }
            }
            while (next_permutation(word.begin(), word.end()));
        }
        
        minPalForLength.push_back(minPalLength);
        cout << wordLength << " " << (double)minPalLength / word.size() << " " << minPalLength << endl;
        double time2 = get_wall_time();
        double seconds = time2 - time1;
        
        cout << "took " << seconds << " seconds" << endl;
//        cout << numLyndonWords << " Lyndon words" << endl;
        
    }
    
    double totalTime2 = get_wall_time();
    double totalSeconds = totalTime2 - totalTime1;
    
    cout << "Total time is " << totalSeconds << endl;
}

void checkAtRandom(int wordLength, int numTries, bool checkPalindromes)
{
    vector<bool> word;
    word.resize(wordLength);
    
    int epsilon = 0;
    
    srand(218);
    int minLength = 100;
    int numChecked = 0;
    for (int i = 0; i < numTries; i++)
    {
        int numOnes = 0;
        for (int iLetter = 0; iLetter < wordLength; iLetter++)
        {
            word[iLetter] = rand() % 2;
            numOnes += word[iLetter];
        }
        
        if ((numOnes >= wordLength / 2 - epsilon)
            && numOnes <= wordLength / 2 + epsilon)
        {
            numChecked++;
            
            int palindromeLength;
            
            palindromeLength = findMaxPalindrome(word,checkPalindromes);
            
            minLength = min(minLength, palindromeLength);
        }
    }
    
    cout << numChecked << " " << (double)minLength / word.size() << " " << minLength << endl;
}

int main()
{
    const int maxWordLength = 80;
    initDPArray(maxWordLength);
    
    bool checkPalindromes = false;
    bool checkSquares = true;
    cout << "checkPalindromes = " << checkPalindromes << endl;
    
    checkBruteforce(28, 28, checkSquares, checkPalindromes);
    
    //checkAtRandom(100, 10000, checkPalindromes);
    
    deleteDPArray(maxWordLength);
    return 0;
}

