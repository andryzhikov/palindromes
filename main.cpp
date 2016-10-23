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

void deleteDPArray()
{
    for (int i = 0; i < maxWordLength; i++)
        delete[] maxPal[i];
    delete[] maxPal;
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

//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time(){
    LARGE_INTEGER time,freq;
    if (!QueryPerformanceFrequency(&freq)){
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)){
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time(){
    FILETIME a,b,c,d;
    if (GetProcessTimes(GetCurrentProcess(),&a,&b,&c,&d) != 0){
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return
            (double)(d.dwLowDateTime |
            ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    }else{
        //  Handle error
        return 0;
    }
}

//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif

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
    
    vector<int> minPalForLength;
    minPalForLength.push_back(0);
    
    for (int wordLength = minLength; wordLength <= maxLength; wordLength += 2)
    {
        double estimatedMinPalLength = ((double)minPalForLength[(wordLength - minLength) / 2] / wordLength);
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
        
        do {
            if (theWordIsTheLeast(word))
            {
                int palindromeLength;
                
                if (checkPalindromes)
                    palindromeLength = findMaxPalindrome(word);
                else
                    palindromeLength = findMaxAntiPalindrome(word);
                
                minPalLength = min(minPalLength, palindromeLength);
            }
        } while (next_permutation(word.begin(), word.end()));
        
        minPalForLength.push_back(minPalLength);
        cout << wordLength << " " << (double)minPalLength / word.size() << " " << minPalLength << endl;
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
            
            if (checkPalindromes)
                palindromeLength = findMaxPalindrome(word);
            else
                palindromeLength = findMaxAntiPalindrome(word);
            
            minLength = min(minLength, palindromeLength);
        }
    }
    
    cout << numChecked << " " << (double)minLength / word.size() << " " << minLength << endl;
}

int main()
{
    initDPArray();
    
    bool checkPalindromes = false;
    cout << "checkPalindromes = " << checkPalindromes << endl;
    
    checkBruteforce(2, 30, checkPalindromes);
    
    //checkAtRandom(100, 10000, checkPalindromes);
    
    deleteDPArray();
    return 0;
}

