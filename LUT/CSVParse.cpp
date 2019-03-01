#include "CSVParse.h"

#include <sstream>
#include <fstream>
#include <iostream>

std::vector<std::vector<float>> CSVParse::ReadFile(const std::string& fileName)
{
  std::ifstream input(fileName);

  if(!input)
  {
    throw EmptyCSVError("Error Reading CSV File.");
  }

  std::vector<std::vector<float>> output;

  std::string cell, line;

  while(std::getline(input, line, '\n'))
  {
    if(line.empty())
    {
      break;
    }

    std::vector<float> cells;
    size_t position = line.find(",");

    while(position != std::string::npos)
    {
      float number;
      CSVParse::ConvertToFloat(number, line.substr(0, position));
      cells.push_back(number);
      line = line.substr(position+1, line.size());
      position = line.find(",");
      if(!line.empty() && position == std::string::npos)
      {
        CSVParse::ConvertToFloat(number, line);
        cells.push_back(number);
      }
    }


    // Make sure all rows have the same number of entries
    if(output.empty() || cells.size() == output[0].size())
    {
      output.push_back(cells);
    }
    else
    {
      throw BadCSVError("Error Reading CSV File.");
    }

  }

  return output;
}

bool CSVParse::ConvertToFloat(float& value, const std::string& string)
{
  size_t size_check;
  value = std::stof(string, &size_check);
  if(size_check == string.size())
  {
    return true;
  }
  else
  {
    throw BadCSVError("Error Reading CSV File.");
  }
}

BadCSVError::BadCSVError(const std::string& what_arg)
: std::runtime_error(what_arg)
{
}

EmptyCSVError::EmptyCSVError(const std::string& what_arg)
: std::runtime_error(what_arg)
{
}