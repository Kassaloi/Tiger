/**
 ** \file type/type-checker.cc
 ** \brief Implementation for type/type-checker.hh.
 */

#include <memory>

#include <ast/all.hh>
#include <range/v3/view/iota.hpp>
#include <type/type-checker.hh>
#include <type/types.hh>

namespace type
{
  namespace
  {
    // Try to cast the type into a nil type.
    // If it's not actually a `type::Nil`, return `nullptr`.
    // This allows us to write more clear code like:
    // if (auto nil = to_nil(e.type_get())
    // ...
    const Nil* to_nil(const Type& type)
    {
      auto bl = dynamic_cast<const Nil*>(&type.actual());
      return bl;
    }

  } // namespace

  TypeChecker::TypeChecker()
    : super_type()
    , error_()
  {}

  const Type* TypeChecker::type(ast::Typable& e)
  {
    auto res = e.get_type();
    if (!res)
      {
        e.accept(*this);
      }
    return res;
  }

  const Record* TypeChecker::type(const ast::fields_type& e)
  {
    std::vector<Field> fields;
    size_t curs = 0;
    while (curs < e.size())
      {
        auto& f = e[curs];
        fields.emplace_back(f->name_get(), *type(f->type_name_get()));
        curs++;
      }
    auto res = new Record;
    for (const auto& field : fields)
      {
        res->field_add(field);
      }
    return res;
  }

  const Record* TypeChecker::type(const ast::VarChunk& e)
  {
    auto res = new Record;
    for (const auto& var : e)
      {
        auto& var_type = *type(*var);
        res->field_add(var->name_get(), var_type);
      }
    return res;
  }

  const misc::error& TypeChecker::error_get() const { return error_; }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  void TypeChecker::error(const ast::Ast& ast, const std::string& msg)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << std::endl;
  }

  void TypeChecker::type_mismatch(const ast::Ast& ast,
                                  const std::string& exp1,
                                  const Type& type1,
                                  const std::string& exp2,
                                  const Type& type2)
  {
    error_ << misc::error::error_type::type << ast.location_get()
           << ": type not matching" << misc::incendl << exp1
           << " type: " << type1 << misc::iendl << exp2 << " type: " << type2
           << misc::decendl;
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                const Type& type1,
                                const std::string& exp2,
                                const Type& type2)
  {
    const Type& ret2 = type2.actual();
    auto bl = type1.compatible_with(ret2);
    if (!bl)
      {
        type_mismatch(ast, exp1, type1, exp2, type2);
      }
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                ast::Typable& type1,
                                const std::string& exp2,
                                ast::Typable& type2)
  {
    const auto a = type(type1);
    const auto b = type(type2);
    const Type& retb = b->actual();
    auto bl = a->compatible_with(retb);
    if (!bl)
      {
        type_mismatch(ast, exp1, *a, exp2, *b);
      }
  }

  /*--------------------------.
  | The core of the visitor.  |
  `--------------------------*/

  /*-----------------.
  | Visiting /Var/.  |
  `-----------------*/

  void TypeChecker::operator()(ast::SimpleVar& e)
  {
    if (!e.def_get() && !e.def_get()->get_type())
      {
        if (e.name_get() == "string")
          {
            type_default(e, &String::instance());
          }
        else if (e.name_get() == "int")
          {
            type_default(e, &Int::instance());
          }
      }
    else if (e.def_get())
      {
        type(*e.def_get());
        type_default(e, e.def_get()->get_type());
      }
    else
      {
        type_default(e, e.def_get()->get_type());
      }
  }

  void TypeChecker::operator()(ast::FieldVar& e)
  {
    type(e.get_var());
    type_default(e, &Void::instance());
    auto got = &e.get_var().get_type()->actual();
    auto got_rec = dynamic_cast<const Record*>(got);
    if (!got_rec || !got_rec->field_type(e.get_name()))
      {
        error(e, "field error: " + e.get_name().get());
      }
    e.set_type(got_rec->field_type(e.get_name()));
  }

  void TypeChecker::operator()(ast::SubscriptVar& e)
  {
    type(e.var_get());
    type(e.index_get());
    check_type(e.index_get(), "not matching expected int", Int::instance());
    auto got = &e.var_get().get_type()->actual();
    auto got_arr = dynamic_cast<const Array*>(got);
    if (!got_arr)
      {
        error(e, " not array");
        return;
      }
    e.set_type(&got_arr->type_get());
  }

  /*-----------------.
  | Visiting /Exp/.  |
  `-----------------*/

  void TypeChecker::operator()(ast::NilExp& e)
  {
    auto ptr = std::make_unique<Nil>();
    type_default(e, ptr.get());
    created_type_default(e, ptr.release());
  }

  void TypeChecker::operator()(ast::IntExp& e) { e.set_type(&Int::instance()); }

  void TypeChecker::operator()(ast::StringExp& e)
  {
    e.set_type(&String::instance());
  }

  // Complex values.

  void TypeChecker::operator()(ast::RecordExp& e)
  {
    // If no error occured, check for nil types in the record initialization.
    // If any error occured, there's no need to set any nil types.
    // If there are any record initializations, set the `record_type_`
    // of the `Nil` to the expected type.
    e.get_type().accept(*this);
    auto name = dynamic_cast<const Named*>(e.get_type().def_get()->get_type());
    if (dynamic_cast<const Record*>(name->type_get())->fields_get().size()
        != e.get_field().size())
      {
        error(e, "record nb fields not matching");
      }
    e.set_type(dynamic_cast<const Record*>(name->type_get()));
    for (auto elm : e.get_field())
      {
        type(elm->init_get());
      }
  }

  void TypeChecker::operator()(ast::OpExp& e)
  {
    type(e.left_get());
    type(e.right_get());
    auto right = e.right_get().get_type();
    auto left = e.left_get().get_type();

    if (to_nil(*left) && to_nil(*right))
      {
        type_mismatch(e, "right op", *(right), "and left op", *(left));
      }

    auto r_nil = to_nil(*e.right_get().get_type());
    auto l_nil = to_nil(*e.left_get().get_type());
    if (r_nil || l_nil)
      {
        if (r_nil)
          {
            l_nil = to_nil(*e.right_get().get_type());
            r_nil = to_nil(*e.left_get().get_type());
            left = e.right_get().get_type();
            right = e.left_get().get_type();
          }
        if (r_nil->compatible_with(left->actual()))
          {
            r_nil->record_type_set(*left);
          }
        else
          {
            type_mismatch(e, "op", *(right), "given was", *(left));
          }
      }
    if (!error_)
      {
        if (!left->compatible_with(*right))
          type_mismatch(e, "op", *(right), "expected", *(left));
      }
    e.set_type(&Int::instance());
  }

  void TypeChecker::operator()(ast::AssignExp& e)
  {
    type(e.get_var());
    type(e.get_exp());
    e.set_type(&Void::instance());

    auto r_var = dynamic_cast<ast::SimpleVar*>(&e.get_var());
    if (!r_var && var_read_only_.find(r_var->def_get()) != var_read_only_.end())
      {
        error(e, "var of loop is 0111");
        return;
      }
    auto exp = e.get_exp().get_type();
    auto var = e.get_var().get_type();
    auto r_nil = to_nil(*exp);
    auto l_nil = to_nil(var->actual());
    if (r_nil)
      {
        r_nil->record_type_set(var->actual());
        if (l_nil)
          {
            r_nil->record_type_set(l_nil->record_type_get()->actual());
          }
      }
    else if (!var->actual().compatible_with(exp->actual()))
      {
        type_mismatch(e, "assigned", *(var), "but expect", *(exp));
      }
  }

  void TypeChecker::operator()(ast::IfExp& e)
  {
    type(e.get_exp());
    check_type(e.get_exp(), "type not matching", Int::instance());

    if (!e.get_else())
      {
        type(e.get_then());
        check_types(e, "then type ", *(e.get_then().get_type()), "else type",
                    Void::instance());
      }
    else
      {
        check_types(e, "then type ", e.get_then(), "else type", *e.get_else());
      }

    e.set_type(e.get_then().get_type());
  }

  void TypeChecker::operator()(ast::ArrayExp& e)
  {
    type(e.get_rhs());
    type(e.get_lhs());
    type(e.get_type());
    auto& r_act = e.get_type().def_get()->get_type()->actual();
    auto res = dynamic_cast<const Array*>(&(r_act));
    if (!res)
      {
        error(e, " not an array");
        e.set_type(res);
        return;
      }

    check_type(e.get_lhs(), "type not matching array", Int::instance());
    check_type(e.get_rhs(), "type not matching array",
               res->type_get().actual());
    e.set_type(res);
  }

  void TypeChecker::operator()(ast::CallExp& e)
  {
    auto type = e.get_func()->get_type();
    auto arg = dynamic_cast<const Function*>(type);
    if (!arg || arg->formals_get().fields_get().size() != e.get_seq().size())
      {
        error(e, "function not declared");
      }

    auto f_beg = arg->formals_get().begin();
    auto f_end = arg->formals_get().end();

    auto e_beg = e.get_seq().begin();
    auto e_end = e.get_seq().end();

    for (; f_beg != f_end && e_beg != e_end; ++f_beg, ++e_beg)
      {
        auto exp_type = (*e_beg)->get_type();
        (*e_beg)->accept(*this);
        if (f_beg->type_get().compatible_with(exp_type->actual())
            && f_beg != f_end)
          {
            continue;
          }
        type_mismatch(e, "assigned", *exp_type, "but expect",
                      f_beg->type_get());
      }
    e.set_type(&arg->result_get());
  }

  void TypeChecker::operator()(ast::LetExp& e)
  {
    e.get_chunk().accept(*this);
    e.get_exp().accept(*this);
    e.set_type(e.get_exp().get_type());
  }

  void TypeChecker::operator()(ast::SeqExp& e)
  {
    auto exp = e.get_seq();
    for (auto& elm : exp)
      {
        elm->accept(*this);
      }
    if (!exp.empty())
      {
        e.set_type(exp.back()->get_type());
        return;
      }
    e.set_type(&Void::instance());
  }

  void TypeChecker::operator()(ast::ForExp& e)
  {
    auto& var = e.vardec_get();
    auto& hi = e.hi_get();
    auto& body = e.body_get();
    type(var);
    type(hi);
    check_type(var, "vardec type not matching ", Int::instance());
    check_type(hi, "loop type not matching ", Int::instance());
    var_read_only_.insert(&var);
    type(body);
    check_type(body, "body type not void", Void::instance());
    type_default(e, &Void::instance());
  }

  void TypeChecker::operator()(ast::BreakExp& e)
  {
    e.set_type(&Void::instance());
  }

  void TypeChecker::operator()(ast::WhileExp& e)
  {
    type(e.test_get());
    check_type(e.test_get(), "condition in while not int", Int::instance());
    type(e.body_get());
    check_type(e.body_get(), "while body not void", Void::instance());
    e.set_type(&Void::instance());
  }

  /*-----------------.
  | Visiting /Dec/.  |
  `-----------------*/

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void TypeChecker::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  void TypeChecker::operator()(ast::FunctionDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store the type of this function.
  template <>
  void TypeChecker::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    Function* func = new Function(type(e.formals_get()), Void::instance());
    if (e.result_get())
      {
        func = new Function(type(e.formals_get()), *type(*e.result_get()));
      }
    e.set_type(func);
    e.set_created_type(func);
  }

  // Type check this function's body.
  template <>
  void TypeChecker::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get())
      {
        visit_routine_body<Function>(e);

        // Check for Nil types in the function body.
        if (!error_)
          {
            auto body_type = e.body_get()->get_type();
            auto bt_nil = to_nil(*body_type);
            if (bt_nil)
              {
                bt_nil->record_type_set(*e.result_get()->get_type());
              }
          }
      }
  }

  /*---------------.
  | Visit VarDec.  |
  `---------------*/

  void TypeChecker::operator()(ast::VarDec& e)
  {
    auto i_get = e.init_get();
    if (i_get)
      {
        i_get->accept(*this);
        if (!e.type_name_get())
          {
            if (!to_nil(*i_get->get_type()))
              {
                e.set_type(i_get->get_type());
              }
            else
              {
                error(e, "initialization is nil no type is specified");
              }
          }
        else
          {
            auto def = e.type_name_get()->def_get();
            auto name = e.type_name_get()->name_get();
            if (!def)
              {
                if (name == "int")
                  {
                    e.set_type(&Int::instance());
                  }
                else if (name == "string")
                  {
                    e.set_type(&String::instance());
                  }
                if (!e.get_type()->compatible_with(*i_get->get_type()))
                  {
                    type_mismatch(e, "var dec", *(e.get_type()), "var init",
                                  *(i_get->get_type()));
                  }
              }
            else if (def->get_type()->compatible_with(*i_get->get_type()))
              {
                auto i_nil = to_nil(*i_get->get_type());
                if (i_nil)
                  {
                    i_nil->record_type_set(def->get_type()->actual());
                  }
                e.set_type(i_get->get_type());
              }
            else
              {
                type_mismatch(e, "var dec", *def->get_type(), "var init",
                              *(i_get->get_type()));
              }
          }
      }
    else if (e.type_name_get())
      {
        e.type_name_get()->accept(*this);
        e.set_type(e.type_name_get()->get_type());
      }
    else
      {
        e.set_type(&Void::instance());
      }
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/

  void TypeChecker::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
  }

  void TypeChecker::operator()(ast::TypeDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store this type.
  template <> void TypeChecker::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    // We only process the head of the type declaration, to set its
    // name in E.  A declaration has no type in itself; here we store
    // the type declared by E.
    Named* ptr = new Named(e.name_get());
    e.set_type(ptr);
    e.set_created_type(ptr);
  }

  // Bind the type body to its name.
  template <> void TypeChecker::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    auto n = dynamic_cast<const type::Named*>(e.get_type());
    n->type_set(type(e.ty_get()));
    if (!n->sound())
      error(e, "infinite recursion");
  }

  /*------------------.
  | Visiting /Chunk/. |
  `------------------*/

  template <class D> void TypeChecker::chunk_visit(ast::Chunk<D>& e)
  {
    for (auto chunk : e.decs_get())
      visit_dec_header<D>(*chunk);

    for (auto chunk : e.decs_get())
      visit_dec_body<D>(*chunk);
  }

  /*-------------.
  | Visit /Ty/.  |
  `-------------*/

  void TypeChecker::operator()(ast::NameTy& e)
  {
    if (e.name_get() != "int" && e.name_get() != "string")
      e.set_type(e.def_get()->get_type());
    else if (e.name_get() == "string")
      e.set_type(&String::instance());
    else
      e.set_type(&Int::instance());
  }

  void TypeChecker::operator()(ast::RecordTy& e)
  {
    auto p = type(e.get_field());
    e.set_type(p);
    e.set_created_type(p);
  }

  void TypeChecker::operator()(ast::ArrayTy& e)
  {
    auto& b = e.base_type_get();
    b.accept(*this);
    auto a = new Array(*b.get_type());
    type_default(e, a);
  }

} // namespace type
