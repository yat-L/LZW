#include "BitFile.hpp"
#include "Dict.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

void encode(string, string);
void decode(string inputFile, string outputFile);
int getIndex(BitFile &file, int bitLen);
void outputString(vector<uint8_t> str, BitFile &file, int &actual);
int de_bitLength(int num);

int main(int argc, char **argv) {
  if (argc < 4) { // if too little arguments
    cout << "Please provide command." << endl;
    return 1;
  }
  if (*argv[1] == 'c') {      // if with 'c' flag, compress
    encode(argv[2], argv[3]); // argv[2] = input, argv[3] = output
    return 0;
  }
  if (*argv[1] == 'd') {      // if with 'd' flag, decompress
    decode(argv[2], argv[3]); // argv[2] = input, argv[3] = output
    return 0;
  } else { // if nothing apply
    cout << "Wrong Command." << endl;
    return 1;
  }
}

// Encode inputFile to outpuFile
void encode(string inputFile, string outputFile) {
  int insertTimes = 0; // count insert times
  Dict dict; // init dictionary
  BitFile input; // init files
  BitFile output;
  input.openBitFile(inputFile.c_str(), "rb"); // open files
  output.openBitFile(outputFile.c_str(), "wb");

  int inputSize = input.fileSize(); // check data file size
  cout << "Number of bytes in data file: " << inputSize << endl;
  output.write32Bits(inputSize);
  int numberOfComBits = 0; // count number of compressed bits

  uint8_t newSymbol = 0;
  vector<uint8_t> cumulativeSybol;
  input.readByte(&newSymbol);
  cumulativeSybol.push_back(newSymbol);
  while (input.readByte(&newSymbol) != 1) { // loop until no more symbol
    vector<uint8_t> temp = cumulativeSybol;
    temp.push_back(newSymbol);
    if (dict.checkPattern(temp)) {
      cumulativeSybol = temp; // if string found, continuou
    } else { // if not found, output index and insert to dict
      int indexToOutput = dict.reverseLookup(cumulativeSybol);
      int lengthOfCode = dict.bitLength();
      output.writeBits(indexToOutput, lengthOfCode);
      numberOfComBits += lengthOfCode; // increment for report
      // cout << "Enc output: " << indexToOutput << endl;
      dict.insert(temp);
      insertTimes++;
      // cout << "Insert Called: " << insertTimes << endl;
      vector<uint8_t> reset = {newSymbol};
      cumulativeSybol = reset;
    }
  }
  output.writeBits(dict.reverseLookup(cumulativeSybol), dict.bitLength()); // write last bits

  input.closeBitFile(); // close files
  output.closeBitFile();
  cout << "Number of bits in the compressed file: " << numberOfComBits << endl; // report data
  double ratio = (double)(inputSize * 8) / (double)numberOfComBits;
  cout << "Compression Ratio: " << ratio << endl;
}

// decode inputFile to outputFile
void decode(string inputFile, string outputFile) {
  // this function ise de_bitLength() instead of dict.bitLength()
  // this is because dict.bitLength() will lag behind 1 , cause crash
  int insertTimes = 256; // number of times inserted
  Dict dict; // init dictionary
  BitFile input;
  BitFile output;
  input.openBitFile(inputFile.c_str(), "rb");
  output.openBitFile(outputFile.c_str(), "wb");

  uint32_t countSymbol = 0;
  input.read32Bits(&countSymbol);
  cout << "Number of symbols/bytes in decompressed file: " << countSymbol
       << endl;
  int actualOutputSym = 0; // count number of symbol outputted

  int oldIndex = getIndex(input, de_bitLength(insertTimes)); // get first index
  vector<uint8_t> oldStr = dict.index(oldIndex);
  outputString(oldStr, output, actualOutputSym); // output symbols
  insertTimes++;
  int newIndex = getIndex(input, de_bitLength(insertTimes));
  while (newIndex != -1) { // if compressed file is not empty
    vector<uint8_t> newString;
    if (!dict.checkIndex(newIndex)) {
      newString = dict.index(oldIndex);
      uint8_t s0 = newString[0];
      newString.push_back(s0);
    } else {
      newString = dict.index(newIndex);
    }
    outputString(newString, output, actualOutputSym); // output
    vector<uint8_t> temp = oldStr;
    temp.push_back(newString[0]);
    dict.insert(temp); // insert to dict
    insertTimes++;
    oldIndex = newIndex;  // reset variables
    oldStr = dict.index(oldIndex);

    newIndex = getIndex(input, de_bitLength(insertTimes)); // to next index
  }

  //report data
  cout << "Actual number of symbols/bytes decoded: " << actualOutputSym << endl;

  input.closeBitFile();
  output.closeBitFile();
}

// output a whole string, also count the number of symbol outputed
void outputString(vector<uint8_t> str, BitFile &file, int &actual) {
  for (int i = 0; i < str.size(); i++) {
    file.writeByte(str[i]);
    actual++;
  }
}

// read bits in compressed file, output the index, -1 if empty
int getIndex(BitFile &file, int bitLen) {
  int result = 0;
  for (int i = 0; i < bitLen; i++) {
    int currentBit = file.readBit();
    if (currentBit < 0) {
      return -1;
    }

    result |= (currentBit << i);
  }
  return result;
}

// make sure the number of bits to read is correct.
int de_bitLength(int num) {
  if (num < 512) {
    return 9;
  }
  if (num < 1024) {
    return 10;
  }
  if (num < 2048) {
    return 11;
  }
  if (num < 4096) {
    return 12;
  }
  return 12;
}
