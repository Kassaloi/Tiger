                                                            /* -*- C++ -*- */
%option c++
%option nounput
%option debug
%option batch

%{

#include <cerrno>
#include <climits>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include <misc/contract.hh>
  // Using misc::escape is very useful to quote non printable characters.
  // For instance
  //
  //    std::cerr << misc::escape('\n') << '\n';
  //
  // reports about `\n' instead of an actual new-line character.
#include <misc/escape.hh>
#include <misc/symbol.hh>
#include <parse/parsetiger.hh>
#include <parse/tiger-parser.hh>

std::string grown_string;
#include <iostream>

// Convenient shortcuts.
#define TOKEN_VAL(Type, Value)                  \
  parser::make_ ## Type(Value, tp.location_)

#define TOKEN(Type)                             \
  parser::make_ ## Type(tp.location_)


// Flex uses `0' for end of file.  0 is not a token_type.
#define yyterminate() return TOKEN(EOF)

# define CHECK_EXTENSION()                              \
  do {                                                  \
    if (!tp.enable_extensions_p_)                       \
      tp.error_ << misc::error::error_type::scan        \
                << tp.location_                         \
                << ": invalid identifier: `"            \
                << misc::escape(yytext) << "'\n";       \
  } while (false)

YY_FLEX_NAMESPACE_BEGIN
%}

%x SC_COMMENT SC_STRING

/* Abbreviations.  */
int             [0-9]+
SPACE           [ \t]
ID              [a-zA-Z][0-9a-zA-Z_]*
EOL             (\n\r|\r\n|\r|\n)
NUM             \\[0-3][0-7][0-7]
XNUM            \\x[0-7][0-9A-F]
BACK            \\[^\\abfnrtv]
%%
%{
  int nbc = 0;
  // Each time yylex is called.
  tp.location_.step();
%}

 /* The rules.  */

<SC_COMMENT>{
<<eof>>   {
            tp.error_ << misc::error::error_type::scan << tp.location_;
            tp.error_ << ": invalid identifier: `" << misc::escape(yytext) << "'\n";
            BEGIN(INITIAL);
          }
{EOL}     {
          }
"/*"      { nbc++; }
"*/"      {
            nbc--;
            if (nbc <= 0)
            {
              nbc = 1;
              BEGIN(INITIAL);
            }
          }
.         { }
}
<SC_STRING>{
<<eof>>   {
            tp.error_ << misc::error::error_type::scan << tp.location_;
            tp.error_ << ": invalid identifier: `" << misc::escape(yytext) << "'\n";
            BEGIN(INITIAL);
          }
{NUM}     {
            grown_string += strtol(yytext + 1, 0, 8);
          }
{XNUM}    {
            grown_string += strtol(yytext + 2, 0, 16);
          }

{EOL}     {
            tp.location_.lines(yyleng);
            tp.location_.step();
          }
{SPACE}   {
            grown_string += yytext;
          }
"\\,"     {
            tp.error_ << misc::error::error_type::scan << tp.location_;
            tp.error_ << ": invalid identifier: `" << misc::escape(yytext) << "'\n";
            BEGIN(INITIAL);
          }
"\\\""   {
            grown_string += "\"";
            }
"\\\\"   {
            grown_string += "\\";
            }
"\\n"   {
            grown_string += "\n";
            }
"\\a"   {
            grown_string += "\a";
            }
"\\b"   {
            grown_string += "\b";
            }
"\\f"   {
            grown_string += "\f";
            }
"\\r"   {
            grown_string += "\r";
            }
"\\t"   {
            grown_string += "\t";
            }
"\\v"   {
            grown_string += "\v";
            }
"\""      {
            BEGIN(INITIAL);
            return TOKEN_VAL(STRING, grown_string);
          }
{BACK}    {
            tp.error_ << misc::error::error_type::scan << tp.location_;
            tp.error_ << ": invalid identifier: `" << misc::escape(yytext) << "'\n";
            BEGIN(INITIAL);
          }
.         {
            // on ajoute le reste au string
            grown_string += yytext;
          }
}


<<eof>>       { return TOKEN(EOF); }
"\""          {
                grown_string.clear();
                BEGIN(SC_STRING);
              }
"/*"          {
                nbc = 1;
                BEGIN(SC_COMMENT);
              }
"array"|"if"|"then"|"else"|"while"|"for"|"to"|"do"|"let"|"in"|"end"|"of"|"break"|"nil"|"function"|"var"|"type"|"import"|"primitive"    {
                std::string s(yytext);
                if (s == "array")
                {
                    return TOKEN(ARRAY);
                }
                else if (s == "if")
                {
                    return TOKEN(IF);
                }
                else if (s == "then")
                {
                    return TOKEN(THEN);
                }
                else if (s == "else")
                {
                    return TOKEN(ELSE);
                }
                else if (s == "while")
                {
                    return TOKEN(WHILE);
                }
                else if (s == "for")
                {
                    return TOKEN(FOR);
                }
                else if (s == "to")
                {
                    return TOKEN(TO);
                }
                else if (s == "do")
                {
                    return TOKEN(DO);
                }
                else if (s == "let")
                {
                    return TOKEN(LET);
                }
                else if (s == "in")
                {
                    return TOKEN(IN);
                }
                else if (s == "end")
                {
                    return TOKEN(END);
                }
                else if (s == "of")
                {
                    return TOKEN(OF);
                }
                else if (s == "break")
                {
                    return TOKEN(BREAK);
                }
                else if (s == "nil")
                {
                    return TOKEN(NIL);
                }
                else if (s == "function")
                {
                    return TOKEN(FUNCTION);
                }
                else if (s == "var")
                {
                    return TOKEN(VAR);
                }
                else if (s == "type")
                {
                    return TOKEN(TYPE);
                }
                else if (s == "import")
                {
                    return TOKEN(IMPORT);
                }
                else
                {
                    return TOKEN(PRIMITIVE);
                }
              }

"class"|"extends"|"method"|"new"       {
                if (!tp.enable_object_extensions_p_)
                  tp.error_ << misc::error::error_type::scan
                  << tp.location_
                  << ": invalid identifier: `"
                  << misc::escape(yytext) << "'\n";
                else if (std::string(yytext) == "class")
                  return TOKEN(CLASS);
                else if (std::string(yytext) == "extends")
                  return TOKEN(EXTENDS);
                else if (std::string(yytext) == "method")
                  return TOKEN(METHOD);
                else
                  return TOKEN(NEW);
              }

","|":"|";"|"("|")"|"["|"]"|"{"|"}"|"."|"+"|"-"|"*"|"/"|"="|"<>"|"<"|"<="|">"|">="|"&"|"|"|":="           {
                std::string s(yytext);
                //std::cout << "s = " << s << std::endl;
                if (s == ",")
                {
                    return TOKEN(COMMA);
                }
                else if (s == ":")
                {
                    return TOKEN(COLON);
                }
                else if (s == ";")
                {
                    return TOKEN(SEMI);
                }
                else if (s == "(")
                {
                    return TOKEN(LPAREN);
                }
                else if (s == ")")
                {
                    return TOKEN(RPAREN);
                }
                else if (s == "[")
                {
                    return TOKEN(LBRACK);
                }
                else if (s == "]")
                {
                    return TOKEN(RBRACK);
                }
                else if (s == "{")
                {
                    return TOKEN(LBRACE);
                }
                else if (s == "}")
                {
                    return TOKEN(RBRACE);
                }
                else if (s == ".")
                {
                    return TOKEN(DOT);
                }
                else if (s == "+")
                {
                    return TOKEN(PLUS);
                }
                else if (s == "-")
                {
                    return TOKEN(MINUS);
                }
                else if (s == "*")
                {
                    return TOKEN(TIMES);
                }
                else if (s == "/")
                {
                    return TOKEN(DIVIDE);
                }
                else if (s == "=")
                {
                    return TOKEN(EQ);
                }
                else if (s == "<>")
                {
                    return TOKEN(NE);
                }
                else if (s == "<")
                {
                    return TOKEN(LT);
                }
                else if (s == "<=")
                {
                    return TOKEN(LE);
                }
                else if (s == ">")
                {
                    return TOKEN(GT);
                }
                else if (s == ">=")
                {
                    return TOKEN(GE);
                }
                else if (s == "&")
                {
                    return TOKEN(AND);
                }
                else if (s == "|")
                {
                    return TOKEN(OR);
                }
                else
                {
                    return TOKEN(ASSIGN);
                }
              }
"_main"       {
                misc::symbol sy(yytext);
                return TOKEN_VAL(ID, sy);
              }
"_exp"|"_cast"|"_lvalue"|"_chunks" {
                auto s = std::string(yytext);
                if (s == "_exp")
                {
                    return TOKEN(EXP);
                }
                else if (s == "_cast")
                {
                    return TOKEN(CAST);
                }
                else if (s == "_lvalue")
                {
                    return TOKEN(LVALUE);
                }
                else
                {
                    return TOKEN(CHUNKS);
                }
              }
{SPACE}       {
                tp.location_.step();
                continue;
              }
{ID}          {
                misc::symbol sy(yytext);
                return TOKEN_VAL(ID, sy);
              }
{EOL}         { tp.location_.lines(yyleng);}
{int}         {
                int val = 0;
                val = strtoul(yytext, nullptr, 10);
                if (errno == ERANGE)
                {
                  tp.error_ << misc::error::error_type::scan
                  << tp.location_
                  << ": invalid identifier: `"
                  << misc::escape(yytext) << "'\n";
                }
                else
                  return TOKEN_VAL(INT, val);
              }
.             {
                tp.error_ << misc::error::error_type::scan << tp.location_;
                tp.error_ << ": invalid identifier: `" << misc::escape(yytext) << "'\n";
              }

%%

// Do not use %option noyywrap, because then flex generates the same
// definition of yywrap, but outside the namespaces, so it defines it
// for ::yyFlexLexer instead of ::parse::yyFlexLexer.
int yyFlexLexer::yywrap() { return 1; }

void
yyFlexLexer::scan_open_(std::istream& f)
{
  yypush_buffer_state(YY_CURRENT_BUFFER);
  yy_switch_to_buffer(yy_create_buffer(&f, YY_BUF_SIZE));
}

void
yyFlexLexer::scan_close_()
{
  yypop_buffer_state();
}

YY_FLEX_NAMESPACE_END
