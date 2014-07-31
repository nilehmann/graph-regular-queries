/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * ----------------------------------------------------------------------------
 */

#include <engine/dictionary_encoding.h>
using kyotocabinet::HashDB;
using boost::filesystem::path;


DictionaryEncoding::DictionaryEncoding(const path &base_file,
                                       bool trunc)
    : db_(),
      inverse_db_() {
      //array_(base_file.native() + ".arr", 201, trunc){
  int opts = HashDB::OWRITER | HashDB::OCREATE;
  if (trunc)
    opts |= HashDB::OTRUNCATE;
  db_.open(base_file.native() + "s2i.kch", opts);
  inverse_db_.open(base_file.native() + "i2s.kch", opts);
}

bool DictionaryEncoding::Add(const std::string &s) {
  if (db_.check(s) != -1)
    return false;
  //uint pos = array_.PushBack(s) - 1;
  uint pos = db_.count();
  db_.add(s.data(), s.size(), reinterpret_cast<char*>(&pos), sizeof(uint));
  inverse_db_.add(reinterpret_cast<char*>(&pos), sizeof(uint),
                  s.data(), s.size());
  return true;
}
bool DictionaryEncoding::Encode(const std::string &key, uint *val) const {
  return const_cast<HashDB&>(db_).get(
      key.data(), key.size(),
      reinterpret_cast<char*>(val), sizeof(uint)) != -1;
}
uint DictionaryEncoding::Count() {
  return db_.count();
}

std::string DictionaryEncoding::Decode(uint key) const {
  //return array_[key];
  size_t size;
  char *arr = const_cast<HashDB&>(inverse_db_).get(
      reinterpret_cast<char*>(&key), sizeof(uint),
      &size);
  std::string s(arr);
  delete [] arr;
  return s;
}

DictionaryEncoding::~DictionaryEncoding() {
  db_.close();
}

struct encode_visitor: public boost::static_visitor<regexp::RegExp<uint>> {
  const DictionaryEncoding &P;
  encode_visitor(const DictionaryEncoding &P): P(P) {}

  result_type operator()(std::string s) const {
    uint val;
    P.Encode(s, &val);
    return val;
  }

  template<typename Op>
  result_type operator()(regexp::multy_op<Op, std::string> &multy) const {
    auto new_multy= regexp::multy_op<Op, uint>();
    for (auto &expr : multy.children)
      new_multy.push_expr(std::move(boost::apply_visitor(*this, expr)));
    return new_multy;
  }

  template<typename Op>
  result_type operator()(regexp::unary_op<Op, std::string> &unary) const {
    return regexp::unary_op<Op, uint>(
        std::move(boost::apply_visitor(*this, unary.expr)));
  }
};

regexp::RegExp<uint> DictionaryEncoding::Encode(
    regexp::RegExp<std::string> &exp) const {
  return boost::apply_visitor(encode_visitor(*this), exp);
}
