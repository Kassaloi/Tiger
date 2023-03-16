                                                                // -*- C++ -*-
%require "3.0"
%language "C++"
// Set the namespace name to `parse', instead of `yy'.
%define api.prefix {parse}
%define api.value.type variant
%define api.token.constructor

// We use a GLR parser because it is able to handle Shift-Reduce and
// Reduce-Reduce conflicts by forking and testing each way at runtime. GLR
// permits, by allowing few conflicts, more readable and maintainable grammars.
%glr-parser
%skeleton "glr2.cc"

// In TC, we expect the GLR to resolve one Shift-Reduce and zero Reduce-Reduce
// conflict at runtime. Use %expect and %expect-rr to tell Bison about it.

%define parse.error verbose
%defines
%debug
// Prefix all the tokens with TOK_ to avoid colisions.
%define api.token.prefix {TOK_}

/* We use pointers to store the filename in the locations.  This saves
   space (pointers), time (no deep copy), but leaves the problem of
   deallocation.  This would be a perfect job for a misc::symbol
   object (passed by reference), however Bison locations require the
   filename to be passed as a pointer, thus forcing us to handle the
   allocation and deallocation of this object.

   Nevertheless, all is not lost: we can still use a misc::symbol
   object to allocate a flyweight (constant) string in the pool of
   symbols, extract it from the misc::symbol object, and use it to
   initialize the location.  The allocated data will be freed at the
   end of the program (see the documentation of misc::symbol and
   misc::unique).  */
%define api.filename.type {const std::string}
%locations

// The parsing context.
%param { ::parse::TigerParser& tp }

/*---------------------.
| Support for tokens.  |
`---------------------*/
%code requires
{
#include <string>
#include <misc/algorithm.hh>
#include <misc/separator.hh>
#include <misc/symbol.hh>
#include <parse/fwd.hh>

  // Pre-declare parse::parse to allow a ``reentrant'' parsing within
  // the parser.
  namespace parse
  {
    ast_type parse(Tweast& input);
  }
}

%code provides
{
  // Announce to Flex the prototype we want for lexing (member) function.
  # define YY_DECL_(Prefix)                               \
    ::parse::parser::symbol_type                          \
    (Prefix parselex)(::parse::TigerParser& tp)
  # define YY_DECL YY_DECL_(yyFlexLexer::)
}

%printer { yyo << $$; } <int> <std::string> <misc::symbol>;

%token <std::string>    STRING "string"
%token <misc::symbol>   ID     "identifier"
%token <int>            INT    "integer"


/*--------------------------------.
| Support for the non-terminals.  |
`--------------------------------*/

%code requires
{
# include <ast/fwd.hh>
// Provide the declarations of the following classes for the
// %destructor clauses below to work properly.
# include <ast/exp.hh>
# include <ast/var.hh>
# include <ast/ty.hh>
# include <ast/name-ty.hh>
# include <ast/field.hh>
# include <ast/field-init.hh>
# include <ast/function-dec.hh>
# include <ast/type-dec.hh>
# include <ast/var-dec.hh>
# include <ast/chunk.hh>
# include <ast/chunk-list.hh>
}


/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/scantiger.hh>
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>
# include <ast/all.hh>
# include <ast/libast.hh>
# include <parse/tiger-driver.hh>

  namespace
  {

    /// Get the metavar from the specified map.
    template <typename T>
    T*
    metavar(parse::TigerParser& tp, unsigned key)
    {
      parse::Tweast* input = tp.input_;
      return input->template take<T>(key);
    }

  }

  /// Use our local scanner object.
  inline
  ::parse::parser::symbol_type
  parselex(parse::TigerParser& tp)
  {
    return tp.scanner_->parselex(tp);
  }
}

// Definition of the tokens, and their pretty-printing.
%token AND          "&"
       ARRAY        "array"
       ASSIGN       ":="
       BREAK        "break"
       CAST         "_cast"
       CLASS        "class"
       COLON        ":"
       COMMA        ","
       DIVIDE       "/"
       DO           "do"
       DOT          "."
       ELSE         "else"
       END          "end"
       EQ           "="
       EXTENDS      "extends"
       FOR          "for"
       FUNCTION     "function"
       GE           ">="
       GT           ">"
       IF           "if"
       IMPORT       "import"
       IN           "in"
       LBRACE       "{"
       LBRACK       "["
       LE           "<="
       LET          "let"
       LPAREN       "("
       LT           "<"
       MINUS        "-"
       METHOD       "method"
       NE           "<>"
       NEW          "new"
       NIL          "nil"
       OF           "of"
       OR           "|"
       PLUS         "+"
       PRIMITIVE    "primitive"
       RBRACE       "}"
       RBRACK       "]"
       RPAREN       ")"
       SEMI         ";"
       THEN         "then"
       TIMES        "*"
       TO           "to"
       TYPE         "type"
       VAR          "var"
       WHILE        "while"
       EOF 0        "end of file"


%precedence THEN
%precedence ELSE
%precedence DO
%precedence OF
%precedence ASSIGN
%left OR
%left AND
%nonassoc LE LT NE EQ GT GE
%left PLUS MINUS
%left TIMES DIVIDE

%type <ast::Exp*>             exp
%type <ast::ChunkList*>       chunks

%type <ast::TypeChunk*>       tychunk
%type <ast::TypeDec*>         tydec
%type <ast::NameTy*>          typeid
%type <ast::Ty*>              ty

%type <ast::Field*>           tyfield
%type <ast::fields_type*>     tyfields tyfields.1

%type <ast::fieldinits_type*> recarray
%type <ast::exps_type*> recfunccall
%type <ast::exps_type*> exps
%type <ast::Var*> lvalue
%type <ast::FunctionDec*> fundec
%type <ast::VarDec*> vardec
%type <ast::VarChunk*> tyvar
%type <ast::FunctionChunk*> tyfunc

// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.

%precedence CHUNKS
%precedence TYPE
%precedence VAR
%precedence FUNCTION
%precedence PRIMITIVE

%start program

%%
program:
  /* Parsing a source program.  */
  exp

   { tp.ast_ = $1; }
| /* Parsing an imported file.  */
  chunks
   { tp.ast_ = $1; }
;


recarray: // [ ID "=" exp { "," ID "=" exp } ]
  ID "=" exp { auto *var = tp.td_.make_fieldinits_type(); var->insert(var->begin(), tp.td_.make_FieldInit(@1, $1, $3)); $$ = var; }
| ID "=" exp "," recarray { auto *var = $5; var->insert(var->begin(), tp.td_.make_FieldInit(@1, $1, $3)); $$ = var; }
;

recfunccall: // [ exp { "," exp } ]
  exp { $$ = tp.td_.make_exps_type(); $$->insert($$->begin(), $1); }
| exp "," recfunccall { auto *var = $3; var->insert(var->begin(), $1); $$ = var; }
;

exps: // [ exp { ";" exp } ]
  exp { auto* var = tp.td_.make_exps_type(); var->insert(var->begin(), $1); $$ = var; }
| exp ";" exps { auto* var = $3; var->insert(var->begin(), $1); $$ = var; }
;

// (* === Expressions. === *)
%token EXP "_exp";
exp:
// (* Literals. *)
  INT {$$ = tp.td_.make_IntExp(@$ , $1);}
| STRING {$$ = tp.td_.make_StringExp(@$ , $1);}
| NIL {$$ = tp.td_.make_NilExp(@$);}

// (* Array and record creations. *)
| ID "[" exp "]" "of" exp {$$ = tp.td_.make_ArrayExp(@$ , tp.td_.make_NameTy(@$, $1), $3, $6);}
| typeid "{" recarray "}" {$$ = tp.td_.make_RecordExp(@$ , $1, $3);}
| typeid "{" "}" {$$ = tp.td_.make_RecordExp(@$ , $1, tp.td_.make_fieldinits_type());}

// (* Variables, field, elements of an array. *)
| lvalue { $$ = $1; }

// (* Function call. *)
| ID "(" recfunccall ")" { $$ = tp.td_.make_CallExp(@$, $1, $3); }
| ID "(" ")" { $$ = tp.td_.make_CallExp(@$, $1, tp.td_.make_exps_type()); }

// (* Operations. *)
| "(" exps ")"  { $$ = tp.td_.make_SeqExp(@$, $2); }
| "(" ")" { $$ = tp.td_.make_SeqExp(@$, tp.td_.make_exps_type()); }
| "-" exp { $$ = tp.td_.make_OpExp(@$, tp.td_.make_IntExp(@$, 0), ast::OpExp::Oper::sub, $2); }
| exp "+" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::add, $3); }
| exp "-" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::sub, $3); }
| exp "*" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::mul, $3); }
| exp "/" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::div, $3); }
| exp "=" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::eq, $3); }
| exp ">=" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::ge, $3); }
| exp "<>" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::ne, $3); }
| exp "<=" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::le, $3); }
| exp "&" exp { $$ = tp.td_.make_IfExp(@$, $1, tp.td_.make_OpExp(@$, $3, ast::OpExp::Oper::ne, tp.td_.make_IntExp(@$, 0)), tp.td_.make_IntExp(@$, 0)); }
| exp "|" exp { $$ = tp.td_.make_IfExp(@$, $1, tp.td_.make_IntExp(@$, 1), tp.td_.make_OpExp(@$, $3, ast::OpExp::Oper::ne, tp.td_.make_IntExp(@$, 0))); }
| exp "<" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::lt, $3); }
| exp ">" exp { $$ = tp.td_.make_OpExp(@$, $1, ast::OpExp::Oper::gt, $3); }

// (* Assignment. *)
| lvalue ":=" exp { $$ = tp.td_.make_AssignExp(@$, $1, $3); }

// (* Control structures. *)
| "if" exp "then" exp "else" exp { $$ = tp.td_.make_IfExp(@$, $2, $4, $6); }
| "if" exp "then" exp { $$ = tp.td_.make_IfExp(@$, $2, $4, nullptr); }
| "while" exp "do" exp { $$ = tp.td_.make_WhileExp(@$, $2, $4); }
| "for" ID ":=" exp "to" exp "do" exp { $$ = tp.td_.make_ForExp(@$, tp.td_.make_VarDec(@2, $2, nullptr, $4), $6, $8); }
| "break" { $$ = tp.td_.make_BreakExp(@$); }
| "let" chunks "in" exps "end" { $$ = tp.td_.make_LetExp(@$, $2, tp.td_.make_SeqExp(@$, $4)); }

// (* Cast of an expression to a given type *)
| "_cast" "(" exp "," ty ")" {  $$ = tp.td_.make_CastExp(@$ , $3, $5); }
// (* An expression metavariable *)
| "_exp" "(" INT ")" { $$ = metavar<ast::Exp>(tp, $3); }
;

%token LVALUE "_lvalue";
lvalue:
  ID { $$ = tp.td_.make_SimpleVar(@1, $1); }
// (* Record field access. *)
| lvalue "." ID { $$ = tp.td_.make_FieldVar(@$, $1, $3); }
// (* Array subscript. *)
| lvalue "[" exp "]" { $$ = tp.td_.make_SubscriptVar(@$, $1, $3); }
| "_lvalue" "(" INT ")" { $$ = metavar<ast::Var>(tp, $3); } // ADDED
;

/*---------------.
| Declarations.  |
`---------------*/

%token CHUNKS "_chunks";
chunks:
  /* Chunks are contiguous series of declarations of the same type
     (TypeDec, FunctionDec...) to which we allow certain specfic behavior like
     self referencing.
     They are held by a ChunkList, that can be empty, like in this case:
        let
        in
            ..
        end
     which is why we end the recursion with a %empty. */

  %empty                  { $$ = tp.td_.make_ChunkList(@$); }
| tychunk   chunks        { $$ = $2; $$->push_front($1); }
| "_chunks" "(" INT ")" chunks { auto *var = metavar<ast::ChunkList>(tp, $3); $$ = $5; $$->splice_front(*var);}
| tyfunc    chunks  { $$ = $2; $$->push_front($1); }
| tyvar    chunks  { $$ = $2; $$->push_front($1); }
| "import" STRING chunks { auto *var = tp.parse_import($2, @2); if (var) $3->splice_front(*var); $$ = $3; }

;

/*--------------------.
| Type Declarations.  |
`--------------------*/
tyfunc:
  fundec %prec CHUNKS  { $$ = tp.td_.make_FunctionChunk(@1); $$->push_front(*$1); }
| fundec tyfunc        { $$ = $2; $$->push_front(*$1); }
;

tyvar:
  vardec %prec CHUNKS  { $$ = tp.td_.make_VarChunk(@1); $$->push_front(*$1); }
| vardec tyvar         { $$ = $2; $$->push_front(*$1); }
;

fundec:
  "function" ID "(" tyfields ")" ":" typeid "=" exp { auto* v = tp.td_.make_VarChunk(@$);
                                                          for (auto& i : *$4)
                                                            {
                                                              ast::VarDec* vd = tp.td_.make_VarDec((*i).location_get(), (*i).name_get(),
                                                                             &(*i).type_name_get(), nullptr);
                                                              v->emplace_back(*vd);
                                                            }
                                                          $$ = tp.td_.make_FunctionDec(@$, $2, v, $7, $9); }
| "function" ID "(" tyfields ")" "=" exp { auto* v = tp.td_.make_VarChunk(@$);
                                                          for (auto& i : *$4)
                                                            {
                                                              ast::VarDec* vd = tp.td_.make_VarDec((*i).location_get(), (*i).name_get(),
                                                                             &(*i).type_name_get(), nullptr);
                                                              v->emplace_back(*vd);
                                                            }
                                                          $$ = tp.td_.make_FunctionDec(@$, $2, v, nullptr, $7); }
| "primitive" ID "(" tyfields ")" ":" typeid { auto* v = tp.td_.make_VarChunk(@$);
                                                          for (auto& i : *$4)
                                                            {
                                                              ast::VarDec* vd = tp.td_.make_VarDec((*i).location_get(), (*i).name_get(),
                                                                             &(*i).type_name_get(), nullptr);
                                                              v->emplace_back(*vd);
                                                            }
                                                          $$ = tp.td_.make_FunctionDec(@$, $2, v, $7, nullptr); }
| "primitive" ID "(" tyfields ")" { auto* v = tp.td_.make_VarChunk(@$);
                                                          for (auto& i : *$4)
                                                            {
                                                              ast::VarDec* vd = tp.td_.make_VarDec((*i).location_get(), (*i).name_get(),
                                                                             &(*i).type_name_get(), nullptr);
                                                              v->emplace_back(*vd);
                                                            }
                                                          $$ = tp.td_.make_FunctionDec(@$, $2, v, nullptr, nullptr); }
;

vardec:
  "var" ID ":" typeid ":=" exp    { $$ = tp.td_.make_VarDec(@$, $2, $4, $6); }
| "var" ID ":=" exp                 { $$ = tp.td_.make_VarDec(@$, $2, nullptr, $4); }
;

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  { $$ = tp.td_.make_TypeChunk(@1); $$->push_front(*$1); }
| tydec tychunk       { $$ = $2; $$->push_front(*$1); }
;

tydec:
  "type" ID "=" ty { $$ = tp.td_.make_TypeDec(@$, $2, $4); }
;

ty:
  typeid               { $$ = $1; }
| "{" tyfields "}"     { $$ = tp.td_.make_RecordTy(@$, $2); }
| "array" "of" typeid  { $$ = tp.td_.make_ArrayTy(@$, $3); }
;

tyfields:
  %empty               { $$ = tp.td_.make_fields_type(); }
| tyfields.1           { $$ = $1; }
;

tyfields.1:
  tyfields.1 "," tyfield { $$ = $1; $$->emplace_back($3); }
| tyfield                { $$ = tp.td_.make_fields_type($1); }
;

tyfield:
  ID ":" typeid     { $$ = tp.td_.make_Field(@$, $1, $3); }
;

%token NAMETY "_namety";
typeid:
  ID                    { $$ = tp.td_.make_NameTy(@$, $1); }
  /* This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse. */
| NAMETY "(" INT ")"    { $$ = metavar<ast::NameTy>(tp, $3); }
;

%%

void
parse::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << m << std::endl;
  tp.error_ << misc::error::error_type::parse << l;
}
