#include "andCheck.h"
#include "andAlgo.h"
#include "andUtils.h"

using namespace std;

template <typename T>
bool wordHasEqConsLettersLinear(const vector<T> & word, int numLetters)
{
    for (int iLetter = 0; iLetter < word.size() - numLetters; iLetter++)
    {
        bool hasPattern = true;
        for (int iLetPat = 0; iLetPat < numLetters - 1; iLetPat++)
        {
            if (word[iLetter + iLetPat] != word[iLetter + iLetPat + 1])
            {
                hasPattern = false;
                break;
            }
        }
        
        if (hasPattern)
            return true;
    }
    
    return false;
}

template <typename T>
bool wordHasEqConsLettersCirc(const vector<T> & word, int numLetters)
{
    for (int iLetter = 0; iLetter < word.size(); iLetter++)
    {
        bool hasPattern = true;
        for (int iLetPat = 0; iLetPat < numLetters - 1; iLetPat++)
        {
            if (word[(iLetter + iLetPat) % word.size()] != word[(iLetter + iLetPat + 1) % word.size()])
            {
                hasPattern = false;
                break;
            }
        }
        
        if (hasPattern)
            return true;
    }
    
    return false;
}

void checkCirc(int minLength, int maxLength, bool checkSquares, bool checkPalindromes)
{
    int maxWordLength = maxLength * 2 + 10;
    initDPArray(maxWordLength);
    
    double totalTime1 = get_wall_time();
    
    vector<bool> word;
    
    vector<int> minPalForLength;
    minPalForLength.push_back(0);
    
    for (int wordLength = minLength; wordLength <= maxLength; wordLength += 2)
    {
#ifdef HARD_OPT
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
#endif
        
        double time1 = get_wall_time();
        
        int minPalLength = maxWordLength + 1;
        word.resize(wordLength);
        
        word.resize(wordLength, 0);
        
//        for (int iLetter = 0; iLetter < wordLength / 2; iLetter++)
//        {
//            word[iLetter] = 0;
//        }
//        for (int iLetter = wordLength / 2; iLetter < wordLength; iLetter++)
//        {
//            word[iLetter] = 1;
//        }
        
        if (checkSquares)
        {
            do {
                if (theWordIsTheLeast(word))
                {
//                    numLyndonWords++;
                    int squareLength = 0;
                    
//                    int a1 = 0, a2 = 0;
                    //for (int shift = 1; shift < word.size() + 1; shift++)
                    //{
                        // for linear words -- counterex for 28
                        // enough to check for Lyndon words only
                        squareLength = max(squareLength, findMaxSquareSubsequence(word, true));
                        squareLength = max(squareLength, findMaxSquareSubsequence(word, false));
                        
    //                    a1 = max(a1, findMaxSquareSubsequence(word, true));
    //                    a2 = max(a2, findMaxSquareSubsequence(word, false));
                        
                        //rotate(word.begin(), word.begin() + 1, word.end());
                    //}
                    
    //                if (((double)a1 / word.size() <= 0.667)
    //                        && ((double)a2 / word.size() <= 0.667))
    //                {
    //                    cout << a1 << " " << a2 << " " << word << endl;
    //                    //exit(0);
    //                }
                    
                    if (squareLength < minPalLength)
                    {
                        cout << "new min " << squareLength << " on word " << word << endl;
                    }
//                    if (squareLength < (double)2/3 * word.size())
//                    {
//                            cout << word << endl;
//                    }
                    
                    minPalLength = min(minPalLength, squareLength);
                }
            } while (next_permutation(word.begin(), word.end()));
        }
        else
        {
            //int numLyndonWords = 0;
            do {
                if (theWordIsTheLeast(word))
                {
                    bool doIt = true;
                    
                    // bool doIt = wordHasEqConsLettersCirc(word, 3);
                    
                    if (doIt)
                    {
                    //numLyndonWords++;
                    
                    int palindromeLength;
                    
                    // is max-of-two true for linear words?
                    if (checkPalindromes)
                    {
                        palindromeLength = findMaxPalindromicSubseqCirc(word);
                    }
                    else
                    {
                        palindromeLength = findMaxAntiPalindromicSubseqCirc(word);
                    }
                    
                    // bound is achieved at the same time?
                    //int palindromeLength1 = findMaxPalindrome(word);
                    //int palindromeLength2 = findMaxAntiPalindrome(word);
                    //palindromeLength = max(palindromeLength1, palindromeLength2);
                    
                    if (palindromeLength < minPalLength)
                    {
                        cout << "new min " << palindromeLength << " on word " << word << endl;
                    }
                    minPalLength = min(minPalLength, palindromeLength);}
                }
            }
            //while (next_permutation(word.begin(), word.end()));
            while (nextVector(word.begin(), word.end()));
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
    
    deleteDPArray(maxWordLength);
}

void checkLinear(int minLength, int maxLength)
{
    int maxWordLength = maxLength + 10;
    initDPArray(maxWordLength);
    
    for(int wordLength = minLength; wordLength <= maxLength; wordLength++)
    {
        vector<bool> word(wordLength, 0);
        
        bool f = true;
        double ratio = 1;
        
        int len = wordLength;
        
        while (f){
            for (int j = wordLength - 1; j >= 0; j--){
                if (word[j] == 0)
                {
                    word[j] = 1;
                    break;
                }
                else
                {
                    word[j] = 0;
                    if (j == 0)
                        f = false;
                }
            }
            
            bool g = true;
            for (int k = 0; k < wordLength -1; k++)
            {
                if (word[k] == word[k+1]
                    && word[k] == word[k+2])
                        g = false;
            }
            if (!g) continue;
            len = min(len, findMaxPalindromicSubseq(word));
            
        }
        cout << wordLength << " " << (double) (len) / (wordLength - 2) << endl;
    }
    
    deleteDPArray(maxWordLength);
}

void checkAtRandom(int wordLength, int numTries, bool checkPalindromes)
{
    int maxWordLength = wordLength * 2 + 10;
    initDPArray(maxWordLength);
    
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
            
            //palindromeLength = findMaxReversalSubseq(word,checkPalindromes);
            
            minLength = min(minLength, palindromeLength);
        }
    }
    
    cout << numChecked << " " << (double)minLength / word.size() << " " << minLength << endl;
    
    deleteDPArray(maxWordLength);
}

vector<bool> stringToWord(string& str)
{
    vector<bool> result;
    result.resize(str.size());
    
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '0')
        {
            result[i] = 0;
        }
        else
        {
            result[i] = 1;
        }
    }
    
    return result;
}

