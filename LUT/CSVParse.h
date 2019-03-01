#pragma once

#include <string>
#include <vector>
#include <stdexcept>


class CSVParse
{
  public:
    static std::vector<std::vector<float>> ReadFile(const std::string& fileName);
    static bool ConvertToFloat(float& integer, const std::string& string);
};

class BadCSVError : public std::runtime_error
{
  public:
    BadCSVError(const std::string& what_arg);
};

class EmptyCSVError : public std::runtime_error
{
  public:
    EmptyCSVError(const std::string& what_arg);
};