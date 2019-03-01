#include "CSVParse.h"
#include "LUT.h"

#include <algorithm>
#include <iostream>

LUT::LUT(const std::string& fileName)
{
  try
  {
    data = CSVParse::ReadFile(fileName);
    sort(0);
  }
  catch(const BadCSVError& e)
  {
    std::cerr << "ERROR: Incorrect CSV file layout (not rectangular)\n";
  }
  catch(const EmptyCSVError& e)
  {
    std::cerr << "ERROR: Empty CSV file, make sure path is correctly specified\n";
  }
}

std::vector<float> LUT::lookup(const float& value) const
{
  std::vector<float> comparisonVec(data[0].size());

  for(size_t i = 0; i < comparisonVec.size(); ++i)
  {
    if(i == lastSortedColumn)
    {
      comparisonVec[i] = value;
    }
    else
    {
      comparisonVec[i] = 0;
    }
  }

  // Find first vector which has the value equal to or greater than the searched value at the specified column
  auto const it = std::lower_bound(data.begin(), data.end(), comparisonVec, [this](std::vector<float> a, std::vector<float> b)
  {
    return a[lastSortedColumn] < b[lastSortedColumn];
  });

  if(!it->empty())
  {
    auto greaterVector = *it;

    if(greaterVector[lastSortedColumn] == value)
    {
      return greaterVector;
    }

    if (it == data.end())
    {
      return data[data.size()-1];
    }
    if (it == data.begin())
    {
      return data[0];
    }

    auto lowerVector = *(it-1);
    return interpolate(value, greaterVector, lowerVector);
  }
  else
  {
    return data[data.size()-1];
  }
}

std::vector<float> LUT::interpolate(const float& value, const std::vector<float>& greater, const std::vector<float>& lower) const
{
  std::vector<float> output;
  float x2 = greater[lastSortedColumn];
  float x1 = lower[lastSortedColumn];

  // interpolate based on the sorted column
  for(size_t i = 0; i < greater.size(); ++i)
  {
    if(i != lastSortedColumn)
    {
      float y2 = greater[i];
      float y1 = lower[i];
      output.push_back(y2 - ((x2-value)*(y2-y1)/(x2-x1)));
    }
    else
    {
      output.push_back(value);
    }
  }

  return output;
}

void LUT::sort(const size_t& column)
{
  std::sort(data.begin(), data.end(), [column](std::vector<float> a, std::vector<float> b)
  {
    return a[column] < b[column];
  });

  lastSortedColumn = column;
}