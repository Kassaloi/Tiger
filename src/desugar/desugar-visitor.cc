/**
 ** \file desugar/desugar-visitor.cc
 ** \brief Implementation of desugar::DesugarVisitor.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <desugar/desugar-visitor.hh>
#include <misc/algorithm.hh>
#include <misc/symbol.hh>
#include <parse/libparse.hh>
#include <parse/tweast.hh>

namespace desugar
{
  DesugarVisitor::DesugarVisitor(bool desugar_for_p, bool desugar_string_cmp_p)
    : super_type()
    , desugar_for_p_(desugar_for_p)
    , desugar_string_cmp_p_(desugar_string_cmp_p)
  {}

  /*-----------------------------.
  | Desugar string comparisons.  |
  `-----------------------------*/
  void DesugarVisitor::operator()(const ast::OpExp& e)
  {
    /*if (e.left_get().get_type() == &type::String::instance()
        && e.right_get().get_type() == &type::String::instance())
      {
        if (e.oper_get() == ast::OpExp::Oper::eq)
          {
            auto res = parse::parse(
              parse::Tweast() <<"streq(" << recurse(e.left_get()) << "," << recurse(e.right_get()) << ")");
            result_ = boost::get<ast::Exp*>(res);
          }
        else
          {
            auto res = parse::parse(
              parse::Tweast() <<"strcmp(" << recurse(e.left_get()) << "," << recurse(e.right_get()) << ") " << str(e.oper_get()) << " 0");
            result_ = boost::get<ast::Exp*>(res);
          }
      }
    else
      super_type::operator()(e);*/
  }

  /*----------------------.
  | Desugar `for' loops.  |
  `----------------------*/

  /*<<-
    Desugar `for' loops as `while' loops:

       for i := lo to hi do
         body

     is transformed as:

       let
         var _lo := lo
         var _hi := hi
         var i := _lo
       in
         if i <= _hi then
           while 1 do
             (
               body;
               if i = _hi then
                 break;
               i := i + 1
             )
       end

     Notice that:

     - a `_hi' variable is introduced so that `hi' is evaluated only
       once;

     - a `_lo' variable is introduced to prevent `i' from being in the
       scope of `_hi';

     - a first test is performed before entering the loop, so that the
       loop condition becomes `i < _hi' (instead of `i <= _hi'); this
       is done to prevent overflows on INT_MAX.
       ->>*/

  void DesugarVisitor::operator()(const ast::ForExp& e)
  {
    auto res = parse::parse(
      parse::Tweast() <<"let " << " var lo := " <<
      recurse(e.vardec_get().init_get()) << " var "<< e.vardec_get().name_get() <<
      " := lo var hi := " << recurse(e.hi_get()) <<
      "in if "<< e.vardec_get().name_get() << " <= hi then while 1 do (" << recurse(e.body_get()) <<
      "; if "<< e.vardec_get().name_get() << " = hi then break; "<< e.vardec_get().name_get() <<
      " := "<< e.vardec_get().name_get() << " + 1 ) end");

    //result_ = boost::get<ast::Exp*>(res);
  }

} // namespace desugar
