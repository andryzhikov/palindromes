#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
//#include <omp.h>
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

int findMaxPalindrome(const vector<bool> & word, bool palFlag = true)
{
    vector<vector <int> > maxPal;
    maxPal.resize(word.size());
    for (int i = 0; i < maxPal.size(); i++)
    {
        maxPal[i].resize(word.size());
    }
    
    for (int i = 0; i < word.size(); i++)
    {
        for (int j = 0; j < word.size(); j++)
        maxPal[i][j] = 0;
    }
    
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
                continue;
            
            if ((word[i] == word[j] && palFlag)
                    || (word[i] != word[j] && !palFlag))
                maxPal[i][j] = maxPal[i+1][j-1] + 2;
            else
            {
                maxPal[i][j] = max(maxPal[i + 1][j], maxPal[i][j - 1]);
            }
            
        }
    }
    
    return maxPal[0][word.size() - 1];
}

int main()
{
    int maxWordLength = 22;
    vector<bool> word;
    
    
    for (int wordLength = 22; wordLength <= maxWordLength; wordLength += 2)
    {
        double minPalFactor = 1;
        word.resize(wordLength);
        
        for (int wordInt = 0; wordInt < (int)pow(2, wordLength); wordInt++)
        {
            
            int numOnes = 0;
            for (int iLetter = 0; iLetter < wordLength; iLetter++)
            {
                word[iLetter] = (wordInt >> iLetter) % 2;
                if (word[iLetter] == 0)
                    numOnes++;
            }
            if (numOnes == wordLength / 2)
            {
                //cout << word << endl;
                
                double maxPalFactor = 0;
                for (int shift = 0; shift < wordLength; shift++)
                {
                    rotate(word.begin(), word.begin() + shift, word.end());
                    int palindromeLength = findMaxPalindrome(word, true);
                    //cout << palindromeLength << " " << (double)palindromeLength/word.size() << endl;
                    maxPalFactor = max((double)palindromeLength/word.size(), maxPalFactor);
                }
                minPalFactor = min(minPalFactor, maxPalFactor);
                if (maxPalFactor < 0.6)
                    cout << "this " << word << endl;
            }
        }
        
        cout << wordLength << " " << minPalFactor << endl;
    }
    
    //cout << "minPalFactor =" << " " << minPalFactor << endl;
    return 0;
}

