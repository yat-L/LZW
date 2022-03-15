#include "BitFile.hpp"
#include "Dict.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

void encode(string, string);
void decode(string inputFile, string outputFile);
uint32_t getIndex(BitFile file, Dict dict);

int main() {
  cout << "hello" << endl;
  encode("test.txt", "test.LZ");
  decode("test.LZ","dcom");
}

void encode(string inputFile, string outputFile) {
  Dict dict; // init dictionary
  BitFile input;
  BitFile output;
  input.openBitFile(inputFile.c_str(), "rb");
  output.openBitFile(outputFile.c_str(), "wb");

  int inputSize = input.fileSize();
  cout << "File Size: " << inputSize << " bytes." << endl;
  output.write32Bits(inputSize);

  uint8_t newSymbol = 0;
  vector<uint8_t> cumulativeSybol;
  cumulativeSybol.push_back(input.readByte(&newSymbol));
  while (input.readByte(&newSymbol) != 1) {
    vector<uint8_t> temp = cumulativeSybol;
    temp.push_back(newSymbol);
    if (dict.checkPattern(temp)) {
      cumulativeSybol = temp;
    } else {
      output.writeBits(dict.reverseLookup(cumulativeSybol), dict.bitLength());
      vector<uint8_t> temp = cumulativeSybol;
      temp.push_back(newSymbol);
      dict.insert(temp);
      vector<uint8_t> reset = {newSymbol};
      cumulativeSybol = reset;
    }
  }
  output.writeBits(dict.reverseLookup(cumulativeSybol), dict.bitLength());

  input.closeBitFile();
  output.closeBitFile();
}

void decode(string inputFile, string outputFile) {
  Dict dict; // init dictionary
  BitFile input;
  BitFile output;
  input.openBitFile(inputFile.c_str(), "rb");
  output.openBitFile(outputFile.c_str(), "wb");

  uint32_t countSymbol = 0;
  input.read32Bits(&countSymbol);

  vector<uint8_t> cumulativeSybol;
  while (countSymbol > 0) {
    uint32_t index = getIndex(input, dict);
    vector<uint8_t> str = dict.index(index);
    while (!str.empty()) {
      uint8_t firstSymbol = str[0];           // temporary var, ch in algorithm
      str.erase(str.begin());                 // remove first element
      output.writeByte(firstSymbol);          // output it
      countSymbol--;                          // decrement countSymbol
      vector<uint8_t> temp = cumulativeSybol; // p
      temp.push_back(firstSymbol);            // p + ch
      if (!dict.checkPattern(temp)) {
        dict.insert(temp);
        vector<uint8_t> futureP = {firstSymbol};
        cumulativeSybol = futureP;
      } else {
        cumulativeSybol = temp;
      }
    }
  }

  input.closeBitFile();
  output.closeBitFile();
}

uint32_t getIndex(BitFile file, Dict dict) {
  uint32_t result = 0;
  for (int i = dict.bitLength(); i >= 0; i--) {
    result <<= 1;
    result |= file.readBit();
  }
  return result;
}
