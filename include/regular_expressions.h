/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * ----------------------------------------------------------------------------
 */

#ifndef INCLUDE_REGULAR_EXPRESSIONS_H_
#define INCLUDE_REGULAR_EXPRESSIONS_H_

#include <graph_queries_basic.h>
#include <libk2tree_basic.h>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <array>

namespace regular_expressions {

template<typename OpTag> struct multy_op;
template<typename OpTag> struct unary_op;
struct concat;
struct alternation;
struct kleene;
struct converse;

typedef boost::variant<
          uint,
          boost::recursive_wrapper<multy_op<concat>>,
          boost::recursive_wrapper<multy_op<alternation>>,
          boost::recursive_wrapper<unary_op<kleene>>
        > RegExp;

void print_expression(const RegExp &expr);
void push_expr(RegExp &expr, RegExp &&val);



template<typename OpTag>
struct multy_op {
  multy_op(RegExp &&exp) noexcept{
    children.push_back(std::move(exp));
  }
  multy_op(multy_op &&rhs) noexcept: children() {
    children.swap(rhs.children);
  }
  multy_op(const multy_op &rhs) = delete;
  std::vector<RegExp> children;
};

template<typename OpTag>
struct unary_op {
  unary_op(RegExp &&e) noexcept: expr(std::move(e)) {}
  unary_op(unary_op &&rhs) noexcept: expr(std::move(rhs.expr)) {}
  unary_op(const unary_op &rhs) = delete;
  RegExp expr;
};



}  // nameespace regular_expressions

/*
namespace std {
template<>
vector<regular_expressions::RegExp>::vector(
    const vector<regular_expressions::RegExp> &rhs) = delete;

}
*/
#endif  // INCLUDE_REGULAR_EXPRESSIONS_H_
