#ifndef ANDUTILS_H
#define ANDUTILS_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v)
{
    if (!v.empty())
    {
        out << '[';
        std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "]";
    }
    return out;
}

bool nextVector(std::vector<bool>::iterator begin, std::vector<bool>::iterator end);

double get_wall_time();
double get_cpu_time();

#endif // ANDUTILS_H
