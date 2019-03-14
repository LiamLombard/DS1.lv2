#pragma once

#include <string>
#include <vector>
#include <filesystem>

class LUT
{
  public:
    LUT(const std::string& fileName);
    std::vector<float> lookup(const float& value) const;
    std::vector<float> interpolate(const float& value, const std::vector<float>& greater, const std::vector<float>& lower) const;
    void sort(const size_t& column);


  private:
    std::vector<std::vector<float>> data;
    size_t lastSortedColumn = 0;
};
