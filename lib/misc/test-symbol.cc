/**
 ** Testing the symbols and the symbol tables.
 */

#include <ostream>

#include <misc/contract.hh>
#include <misc/symbol.hh>

using misc::symbol;

int main()
{
  const symbol toto1("toto");
  const symbol toto2("toto");
  const symbol titi1("titi");

  // Checking symbol.
  // FIXED
  assertion(toto1.get() == "toto");

  assertion(toto1 == "toto");
  assertion(toto1 != "titi");

  assertion(toto1 == toto2);
  assertion(toto1 != titi1);

  std::string junk = "tata";
  const symbol tata1(junk);
  junk = "toto";
  assertion(tata1 == "tata");
}
