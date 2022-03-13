#include "BitFile.hpp"
#include <cstdint>
#include <iostream>
#include <vector>
#include "Dict.hpp"

using namespace std;
int main(){
  cout << "hello" << endl;
  Dict d;
  d.printTable();
  vector<uint8_t> test = {1, 2};
  for(int i = 0 ; i < 5000 ; i++){
    d.insert(test);
    cout << d.bitLength() << endl;
  }

}
