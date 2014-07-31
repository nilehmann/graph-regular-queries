/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * ----------------------------------------------------------------------------
 */

#ifndef INCLUDE_GRAPH_H_
#define INCLUDE_GRAPH_H_

#include <graph_queries_basic.h>
#include <engine/dictionary_encoding.h>
#include <regexp.h>
#include <nfa.h>
#include <fstream>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

#include <compressed_hybrid.h>
class GraphDB {
 public:
  GraphDB(fs::path db_name);

  /**
   * Compute the regular expression query starting from the given node.
   * It first encode the node and regular expression using the
   * database dictionary.
   *
   * @param n Number representing the node in the StringTable
   * @param exp Regular expression to compute the query.
   */
  void Compute(std::string n, regexp::RegExp<std::string> &exp) const;

  
  /**
   * Compute the respective query for the given starting node and regular
   * expression.
   *
   * @param n Number representing a node in the database dictionary.
   * @param exp Regular expression to compute the query. Each symbol is
   * a number representing a node in the database predicate encoding dictionary.
   */
  void Compute(uint n, const regexp::RegExp<uint> &exp) const;


  /**
   * Compute the respective query for the given starting node and regular
   * expression corresponding to the given NFA.
   *
   * @param n Number representing a node in the database dictionary.
   * @param nfa NFA corresponding to a regular expression. Each symbol is a
   * number representing a node in the database predicate eincoding dictionary.
   */
  void Compute(uint n, const NFA::NFA &nfa) const;

 private:
  std::vector<libk2tree::CompressedHybrid> k2trees_;
  DictionaryEncoding SO_;
  DictionaryEncoding P_;
};
#endif  // INCLUDE_GRAPH_H_