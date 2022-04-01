/*****************************************************
COMP4160 Assignment 2
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include <cstdint>
#include <cstdio>
#include <map>
#include <unordered_map>
#include <vector>

void printRow(std::vector<uint8_t> row);
class Dict {
private:
  std::vector<std::vector<uint8_t>> table;
  std::map<std::vector<uint8_t>, int> reverseTable;
  int pos;

public:
  bool checkPattern(std::vector<uint8_t>);
  bool checkIndex(uint32_t);
  int reverseLookup(std::vector<uint8_t>);
  std::vector<uint8_t> index(uint32_t);
  void insert(std::vector<uint8_t>);
  void printTable();
  Dict();
  int bitLength();
  void addPos();
};
