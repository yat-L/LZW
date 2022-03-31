#include "Dict.hpp"
#include <cstdint>
#include <vector>

Dict::Dict() {
  pos = 0;
  for (int i = 0; i <= 255; i++) {
    std::vector<uint8_t> test = {(uint8_t)i};
    insert(test);
  }
}

int Dict::reverseLookup(std::vector<uint8_t> key) { return reverseTable[key]; }

void Dict::insert(std::vector<uint8_t> entry) {
  if (pos < 4096) {
    table.push_back(entry);
    reverseTable[entry] = pos;
    pos++;
  }
}

void Dict::printTable() {
  for (int i = 0; i < table.size(); i++) {
    printf("%d: ", i);
    printRow(table[i]);
  }
}

void printRow(std::vector<uint8_t> row) {
  printf("{");
  for (int i = 0; i < row.size(); i++) {
    if (i == row.size() - 1) {
      printf("%d", row[i]);
    } else {
      printf("%d, ", row[i]);
    }
  }
  printf("}\n");
}

std::vector<uint8_t> Dict::index(uint32_t i) { return table[i]; }

bool Dict::checkPattern(std::vector<uint8_t> key) {
  return reverseTable.find(key) != reverseTable.end();
}

int Dict::bitLength() {
  if (pos < 512) {
    return 9;
  }
  if (pos < 1024) {
    return 10;
  }
  if (pos < 2048) {
    return 11;
  }
  if (pos < 4096) {
    return 12;
  }
  return 12;
}

bool Dict::checkIndex(uint32_t index){
  return index < pos;
}

void Dict::addPos(){
  pos++;
}

