/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * ----------------------------------------------------------------------------
 */

#ifndef INCLUDE_SYMBOL_TABLE_H_
#define INCLUDE_SYMBOL_TABLE_H_

#include <unordered_map>
#include <string>
#include <iostream>

typedef unsigned int uint;
class SymbolTable {
 public:
  void SetSymbol(uint symbol, uint val) {
    map[symbol] = val;
  }

  uint LookupSymbol(uint symbol) {
    return map.at(symbol);
  }

  bool Contains(uint symbol) {
    return map.count(symbol);
  }

 private:
  std::unordered_map<uint, uint> map;
  
};
#endif  // INCLUDE_SYMBOL_TABLE_H_

