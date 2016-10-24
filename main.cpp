#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include "andUtils.h"
#include "andAlgo.h"
using namespace std;

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

