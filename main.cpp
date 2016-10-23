#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <sys/time.h>
using namespace std;

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "]";
  }
  return out;
}

int** maxPal;
const int maxWordLength = 80;

void initDPArray()
{
    maxPal = new int * [maxWordLength];
    for (int i = 0; i < maxWordLength; i++)
        maxPal[i] = new int[maxWordLength];
}

int findMaxAntiPalindrome(const vector<bool> & word)
{
    vector<bool> doubledWord;
    doubledWord.insert(doubledWord.end(), word.begin(), word.end());
    doubledWord.insert(doubledWord.end(), word.begin(), word.end());
    
    for (int i = 0; i < doubledWord.size() - 1; i++)
        maxPal[i][i + 1] = 0;
    
    for (int i = 0; i < doubledWord.size(); i++)
    {
            maxPal[i][i] = 0;
    }
    
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
    
    int answer = 0;
    for (int iStart = 0; iStart < word.size(); iStart++)
    {
        answer = max(answer, maxPal[iStart][iStart + word.size() - 1]);
    }
    return answer;
}

int findMaxPalindrome(const vector<bool> & word)
{
    vector<bool> doubledWord;
    doubledWord.insert(doubledWord.end(), word.begin(), word.end());
    doubledWord.insert(doubledWord.end(), word.begin(), word.end());
    
    for (int i = 0; i < doubledWord.size() - 1; i++)
        maxPal[i][i + 1] = 0;
    
    for (int i = 0; i < doubledWord.size(); i++)
    {
        maxPal[i][i] = 1;
    }
    
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
    
    int answer = 0;
    for (int iStart = 0; iStart < word.size(); iStart++)
    {
        answer = max(answer, maxPal[iStart][iStart + word.size() - 1]);
    }
    return answer;
}

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
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
      // a periodic word can not be a minimum counterexample
      // (for both palindromes and antipalindromes)
      return false;
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

void checkBruteforce(int minLength, int maxLength, bool checkPalindromes)
{
    vector<bool> word;
    for (int wordLength = minLength; wordLength <= maxLength; wordLength += 2)
    {
        double time1 = get_wall_time();
        
        double minPalFactor = 1;
        word.resize(wordLength);
        
        for (int iLetter = 0; iLetter < wordLength / 2; iLetter++)
        {
            word[iLetter] = 0;
        }
        for (int iLetter = wordLength / 2; iLetter < wordLength; iLetter++)
        {
            word[iLetter] = 1;
        }
        
        do {
            if (theWordIsTheLeast(word))
            {
                int palindromeLength;
                
                if (checkPalindromes)
                    palindromeLength = findMaxPalindrome(word);
                else
                    palindromeLength = findMaxAntiPalindrome(word);
                
                minPalFactor = min(minPalFactor, (double)palindromeLength / word.size());
            }
        } while (next_permutation(word.begin(), word.end()));
        
        cout << wordLength << " " << minPalFactor << endl;
        double time2 = get_wall_time();
        double seconds = time2 - time1;
        
        cout << "took " << seconds << " seconds" << endl;
        
    }
}

void checkAtRandom(int wordLength, int numTries, bool checkPalindromes)
{
    vector<bool> word;
    word.resize(wordLength);
    
    int epsilon = 0;
    
    srand(218);
    double minFactor = 1;
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
            
            if (checkPalindromes)
                palindromeLength = findMaxPalindrome(word);
            else
                palindromeLength = findMaxAntiPalindrome(word);
            
            double f = (double)palindromeLength / word.size();
            minFactor = min(minFactor, f);
        }
    }
    
    cout << numChecked << " " << minFactor << endl;
}

int main()
{
    initDPArray();
    
    bool checkPalindromes = false;
    cout << "checkPalindromes = " << checkPalindromes << endl;
    
    checkBruteforce(2, 24, checkPalindromes);
    
    //checkAtRandom(100, 10000, checkPalindromes);
    return 0;
}

