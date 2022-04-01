/*****************************************************
COMP4160 Assignment 2
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include "BitFile.hpp"
#include "Dict.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

vector<uint8_t> readFile(string inputFileName);

vector<int> sortingMatrix(vector<uint8_t>& input);

vector<uint8_t> concat(vector<uint8_t> &OgString, uint8_t cutOff);

vector<uint8_t> getTuple(vector<int> &sorted, vector<uint8_t>& input, int &position);

int main() {
  vector<uint8_t> inputInVector = readFile("test");
  vector<int> sorted = sortingMatrix(inputInVector);
  int OgPosition = 0;
  vector<uint8_t> lastColumn = getTuple(sorted, inputInVector, OgPosition);

  return 1;
}

vector<uint8_t> getTuple(vector<int> &sorted, vector<uint8_t>& input,int &position) {
  vector<uint8_t> result;
  for(int i = 0 ; i < sorted.size() ; i++){
    int rotation = sorted[i];
    if(rotation == 0){
      position = i;
    }
    result[i] = input[rotation-1];
  }
  return result;
}

vector<uint8_t> concat(vector<uint8_t> &OgString,int cutOff) {
  vector<uint8_t> result;
  vector<uint8_t> firstHalf =
      vector<uint8_t>(OgString.begin() + cutOff, OgString.end());
  vector<uint8_t> secHalf =
      vector<uint8_t>(OgString.begin(), OgString.begin() + cutOff - 1);

  result.insert(result.end(), firstHalf.begin(), firstHalf.end());
  result.insert(result.end(), secHalf.begin(), secHalf.end());
  return result;
}

vector<int> sortingMatrix(vector<uint8_t> &input) {
  vector<int> sortedIndices;
  for (int i = 0; i < input.size(); i++) {
    sortedIndices[i] = i;
  }
  for (int j = 1; j < input.size(); j++) {
    int value = sortedIndices[j];
    vector<uint8_t> key = concat(input, value);

    int i = j - 1;
    int cValue = sortedIndices[i];
    vector<uint8_t> cStr = concat(input, cValue);

    while (i >= 0 && lexicographical_compare(key.begin(), key.end(),
                                             cStr.begin(), cStr.end())) {
      sortedIndices[i + 1] = sortedIndices[i];
      i--;
      cValue = sortedIndices[i];
      cStr = concat(input, cValue);
    }
    sortedIndices[i + 1] = value;
  }
  return sortedIndices;
}

vector<uint8_t> readFile(string inputFileName) {
  vector<uint8_t> result;
  BitFile input; // init files
  input.openBitFile(inputFileName.c_str(), "rb"); // open files
  uint8_t temp = 0;
  while (input.readByte(&temp) != -1) {
    result.push_back(temp);
  }
  return result;
}
