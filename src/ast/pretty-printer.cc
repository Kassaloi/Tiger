/**
 ** \file ast/pretty-printer.cc
 ** \brief Implementation of ast::PrettyPrinter.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <ast/pretty-printer.hh>
#include <misc/escape.hh>
#include <misc/indent.hh>
#include <misc/separator.hh>

#include <type/class.hh>

namespace ast
{
  bool PrettyPrinter::is_meth = false;
  // Anonymous namespace: these functions are private to this file.
  namespace
  {
    /// Output \a e on \a ostr.
    inline std::ostream& operator<<(std::ostream& ostr, const Escapable& e)
    {
      if (escapes_display(ostr))
        ostr << "/* escaping */ ";

      return ostr;
    }

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      if (bindings_display(ostr))
        ostr << " /* " << &e << " */";
      return ostr;
    }
  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  void PrettyPrinter::operator()(const SimpleVar& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  void PrettyPrinter::operator()(const FieldVar& e)
  {
    ostr_ << misc::deref << e.get_var() << '.';
    if (!PrettyPrinter::is_meth)
      ostr_ << e.get_name();
  }

  /* Foo[10]. */
  void PrettyPrinter::operator()(const SubscriptVar& e)
  {
    ostr_ << e.var_get() << '[' << misc::incindent << e.index_get()
          << misc::decindent << ']';
  }

  void PrettyPrinter::operator()(const CastExp& e)
  {
    ostr_ << "_cast(" << e.exp_get() << ", " << e.ty_get() << ")";
  }

  void PrettyPrinter::operator()(const VarChunk& e)
  {
    size_t i = 0;
    while (i < e.decs_get().size())
      {
        this->operator()(e.decs_get()[i]);
        i++;
      }
  }

  void PrettyPrinter::operator()(const FunctionChunk& e)
  {
    size_t i = 0;
    while (i < e.decs_get().size())
      {
        this->operator()(e.decs_get()[i]);
        i++;
      }
  }

  void PrettyPrinter::operator()(const TypeChunk& e)
  {
    size_t i = 0;
    while (i < e.decs_get().size())
      {
        this->operator()(e.decs_get()[i]);
        i++;
      }
  }

  void PrettyPrinter::operator()(const ArrayExp& e)
  {
    ostr_ << e.get_type().name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.get_type().def_get() << " */ ";

    ostr_ << "[" << e.get_lhs() << "] of " << e.get_rhs();
  }
  /*
   * for i := 0 to x
   * (
   *    exp
   * )
   */
  void PrettyPrinter::operator()(const ForExp& e)
  {
    ostr_ << "for ";
    if (bindings_display(ostr_))
      ostr_ << "/* " << &e << " */ ";

    ostr_ << e.vardec_get().name_get();

    if (bindings_display(ostr_))
      ostr_ << " /* " << &e.vardec_get() << " */ ";

    ostr_ << " := " << *e.vardec_get().init_get() << " to " << e.hi_get()
          << " do";
    ostr_ << misc::incendl;
    ostr_ << e.body_get();
    ostr_ << misc::decendl;
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << "array of " << e.base_type_get();
  }

  void PrettyPrinter::operator()(const AssignExp& e)
  {
    ostr_ << e.get_var() << " := " << e.get_exp();
  }

  void PrettyPrinter::operator()(const BreakExp& e)
  {
    ostr_ << "break";
    if (bindings_display(ostr_))
      ostr_ << "/* " << e.get_loop() << " */";
  }

  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.get_func() << " */";

    ostr_ << "(";
    for (auto& s : e.get_seq())
      {
        if (s != e.get_seq().back())
          ostr_ << *s << ", ";
        else
          ostr_ << *s;
      }
    ostr_ << ")";
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    ostr_ << "class ";
    ostr_ << " extends " << e.super_get().name_get();
    ostr_ << misc::incendl << "{" << misc::iendl;
    for (auto* v : e.chunks_get().chunks_get())
      {
        if (v != nullptr)
          ostr_ << *v << misc::iendl;
      }
    ostr_ << misc::decendl << "}";
  }

  void PrettyPrinter::operator()(const Field& e) { ostr_ << e.name_get(); }

  void PrettyPrinter::operator()(const FieldInit& e)
  {
    ostr_ << e.name_get() << " = " << e.init_get();
  }

  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    if (e.body_get() != nullptr)
      ostr_ << "function ";
    else
      ostr_ << "primitive ";

    ostr_ << e << "(";
    for (auto& v : e.formals_get().decs_get())
      {
        ast::Dec& dec = *v;
        ostr_ << dec;
        if (v->type_name_get() != nullptr)
          ostr_ << " : " << *v->type_name_get();
        if (v->init_get() != nullptr)
          ostr_ << " := " << *v->init_get();
        if (v != e.formals_get().decs_get().back())
          ostr_ << ", ";
      }
    ostr_ << ")";
    if (e.result_get())
      ostr_ << " : " << *e.result_get();
    if (e.body_get())
      ostr_ << " = " << misc::iendl << "(" << misc::incendl << *e.body_get()
            << misc::decendl << ")";
    ostr_ << misc::iendl;
  }

  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if " << e.get_exp() << misc::incendl << "then " << e.get_then();
    if (e.get_else() != nullptr)
      ostr_ << misc::iendl << "else " << *e.get_else();
    ostr_ << misc::decendl;
  }

  void PrettyPrinter::operator()(const IntExp& e) { ostr_ << e.value_get(); }

  void PrettyPrinter::operator()(const LetExp& e)
  {
    ostr_ << "let" << misc::incendl << e.get_chunk() << misc::decendl << "in"
          << misc::incendl << e.get_exp() << misc::decendl << "end";
  }

  void PrettyPrinter::operator()(const NameTy& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  void PrettyPrinter::operator()(const NilExp&) { ostr_ << "nil"; }

  void PrettyPrinter::operator()(const OpExp& e)
  {
    ostr_ << "(" << e.left_get() << " " << str(e.oper_get()) << " "
          << e.right_get() << ")";
  }

  void PrettyPrinter::operator()(const RecordExp& e)
  {
    ostr_ << e.get_type().name_get();

    if (bindings_display(ostr_))
      ostr_ << " /* " << e.get_type().def_get() << " */ ";

    ostr_ << " { ";
    for (const auto& exp : e.get_field())
      {
        ostr_ << exp->name_get() << " = " << exp->init_get();
        if (exp != e.get_field().back())
          ostr_ << " , ";
      }
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    ostr_ << "{ ";
    for (auto& f : e.get_field())
      {
        ostr_ << *f << " : " << f->type_name_get();
        if (f != e.get_field().back())
          ostr_ << " , ";
      }
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const StringExp& e)
  {
    ostr_ << '\"' << misc::escape(e.get_var()) << '\"';
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << "type " << e << " = " << e.ty_get() << misc::iendl;
  }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    const ast::Dec& dec = e;
    ostr_ << "var " << dec;
    if (e.type_name_get() != nullptr)
      ostr_ << " : " << *e.type_name_get();
    if (e.init_get() != nullptr)
      ostr_ << " := " << *e.init_get();
    ostr_ << misc::iendl;
  }

  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while ";

    if (bindings_display(ostr_))
      ostr_ << "/* " << &e << " */ ";

    ostr_ << e.test_get() << " do" << misc::incendl << misc::incendl
          << e.body_get() << misc::decendl << misc::decendl;
  }

  void PrettyPrinter::operator()(const SeqExp& e)
  {
    if (!e.get_seq().empty())
      {
        if (e.get_seq().size() == 1)
          {
            ostr_ << *e.get_seq().front();
            return;
          }
        ostr_ << "(" << misc::incendl;
        for (auto& s : e.get_seq())
          {
            if (s != e.get_seq().back())
              ostr_ << *s << ";" << misc::iendl;
            else
              ostr_ << *s << misc::decendl;
          }
        ostr_ << ")";
      }
    else
      ostr_ << "()";
  }

  void PrettyPrinter::operator()(const MethodCallExp& e)
  {
    PrettyPrinter::is_meth = true;
    ostr_ << e.get_var();
    PrettyPrinter::is_meth = false;
    ostr_ << e.name_get() << misc::incendl << "(" << misc::incendl;
    for (auto& a : e.get_seq())
      ostr_ << a;
    ostr_ << misc::decendl << ")" << misc::decindent;
  }

  void PrettyPrinter::operator()(const MethodDec& e)
  {
    ostr_ << "method ";
    ostr_ << e.name_get() << "(" << e.formals_get() << ")";
    if (e.result_get())
      ostr_ << " : " << *e.result_get();
    if (e.body_get())
      ostr_ << " = " << misc::incendl << *e.body_get() << misc::decendl;
  }

} // namespace ast
