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

int findMaxPalindrome(const vector<bool> & word, bool palFlag = true)
{
    //vector<vector <int> > maxPal;
    int wordLength = word.size();
    //int * maxPal;
    //maxPal = new int[wordLength];
    
    int** maxPal;
    maxPal = new int * [wordLength];
    for (int i = 0; i < wordLength; i++)
        maxPal[i] = new int[wordLength];
    //maxPal.resize(word.size());
    //for (int i = 0; i < maxPal.size(); i++)
    //{
    //    maxPal[i].resize(word.size());
    //}
    
    for (int j = 0; j < word.size(); j++)
    {
        for (int i = j; i < word.size(); i++)
            maxPal[i][j] = 0;
    }
    
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
    
    int answer = maxPal[0][word.size() - 1];
    
    for (int i = 0; i < wordLength; i++)
        delete[] maxPal[i];
    delete[] maxPal;
    
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

int main()
{
    int maxWordLength = 26;
    vector<bool> word;
    
    
    for (int wordLength = 2; wordLength <= maxWordLength; wordLength += 2)
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
            //cout << word << endl;
            double maxPalFactor = 0;
            for (int shift = 0; shift <= wordLength; shift++)
            {
                int palindromeLength = findMaxPalindrome(word, false);
                maxPalFactor = max((double)palindromeLength / word.size(), maxPalFactor);
                rotate(word.begin(), word.begin() + 1, word.end());
            }
            //rotate(word.begin(), word.begin() + 1, word.end());
            minPalFactor = min(minPalFactor, maxPalFactor);
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
    return 0;
}

