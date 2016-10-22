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
const int maxWordLength = 40;

void initDPArray()
{
    maxPal = new int * [maxWordLength];
    for (int i = 0; i < maxWordLength; i++)
        maxPal[i] = new int[maxWordLength];
}

int findMaxPalindrome(const vector<bool> & word, bool palFlag = true)
{
    //vector<vector <int> > maxPal;
    int wordLength = word.size();
    //int * maxPal;
    //maxPal = new int[wordLength];
    

//    maxPal = new int * [wordLength];
//    for (int i = 0; i < wordLength; i++)
//        maxPal[i] = new int[wordLength];
    //maxPal.resize(word.size());
    //for (int i = 0; i < maxPal.size(); i++)
    //{
    //    maxPal[i].resize(word.size());
    //}
    
//    for (int j = 0; j < word.size(); j++)
//    {
//        for (int i = j; i < word.size(); i++)
//            maxPal[i][j] = 0;
//    }
    
    for (int i = 0; i < word.size() - 1; i++)
        maxPal[i][i + 1] = 0;
    
    for (int i = 0; i < word.size(); i++)
    {
        if (palFlag)
            maxPal[i][i] = 1;
        else
            maxPal[i][i] = 0;
    }
    
    for (int sum = 1; sum < word.size(); sum++)
    {
        for (int i = 0; i < word.size(); i++)
        {
            int j = i + sum;
            if (j >= word.size())
                break;
            
            if ((word[i] == word[j] && palFlag)
                    || (word[i] != word[j] && !palFlag))
                maxPal[i][j] = maxPal[i+1][j-1] + 2;
            else
            {
                maxPal[i][j] = max(maxPal[i + 1][j], maxPal[i][j - 1]);
            }
            
        }
    }
    
    int answer = maxPal[0][word.size() - 1];
    
//    for (int i = 0; i < wordLength; i++)
//        delete[] maxPal[i];
//    delete[] maxPal;
    
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

double palFactorForCyclicWord(vector<bool> & word, bool checkPalindromes)
{
    double maxPalFactor = 0;
    for (int shift = 0; shift < word.size(); shift++)
    {
        int palindromeLength = findMaxPalindrome(word, checkPalindromes);
        maxPalFactor = max((double)palindromeLength / word.size(), maxPalFactor);
        rotate(word.begin(), word.begin() + 1, word.end());
    }
    //cout << word << " " << maxPalFactor << endl;
//    if (maxPalFactor < 0.72)
//        cout << word << endl;
    
    return maxPalFactor;
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
      return false;
}

bool theWordIsTheLeast(const vector<bool> & word)
{
    if (word[0] == 1)
        return false;
    
    bool isTheLeast = true;
    for (int shift = 1; shift < word.size()/* - 1*/; shift++)
    {
        if (!lexicographicalCompare(word.begin(), word.end(),
                                    word.begin() + shift, word.end()))
        {
            isTheLeast = false;
            break;
        }
    }
    
    return isTheLeast;
}

void checkBruteforce(int minLength, int maxLength, bool checkPalindromes)
{
    vector<bool> word;
    for (int wordLength = minLength; wordLength <= maxLength; wordLength += 2)
    {
        //time_t timer1;
        //time(&timer1);
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
                minPalFactor = min(minPalFactor, palFactorForCyclicWord(word, checkPalindromes));
            }
        } while (next_permutation(word.begin(), word.end()));
        
        cout << wordLength << " " << minPalFactor << endl;
        //time_t timer2;
        //time(&timer2);
        double time2 = get_wall_time();
        //double seconds = difftime(timer2, timer1);
        double seconds = time2 - time1;
        
        cout << "took " << seconds << " seconds" << endl;
        
    }
    
    //cout << "minPalFactor =" << " " << minPalFactor << endl;
    
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
            
            double f = palFactorForCyclicWord(word, checkPalindromes);
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
    
    checkBruteforce(2, 22, checkPalindromes);
    
    //checkAtRandom(100, 10000, checkPalindromes);
    return 0;
}

