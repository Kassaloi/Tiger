* README

Tiger Abstract Syntax Tree nodes with their principal members.
Incomplete classes are tagged with a `*'.

/Ast/               (Location location)
  /Dec/             (symbol name)
    FunctionDec     (VarChunk formals, NameTy result, Exp body)
      MethodDec     ()
    TypeDec         (Ty ty)
    VarDec          (NameTy type_name, Exp init)

  /Exp/             ()
    /Var/           ()
*     FieldVar      (DONE)
      SimpleVar     (symbol name)
      SubscriptVar  (Var var, Exp index)

*   ArrayExp (DONE)
*   AssignExp (DONE)
*   BreakExp (DONE)
*   CallExp (DONE)
*     MethodCallExp (DONE)
    CastExp         (Exp exp, Ty ty)
    ForExp          (VarDec vardec, Exp hi, Exp body)
*   IfExp           (DONE)
    IntExp          (int value)
*   LetExp          (DONE)
    NilExp          ()
*   ObjectExp       (DONE)
    OpExp           (Exp left, Oper oper, Exp right)
*   RecordExp       (DONE)
*   SeqExp          (DONE)
*   StringExp       (DONE)
    WhileExp        (Exp test, Exp body)

  /Ty/              ()
    ArrayTy         (NameTy base_type)
    ClassTy         (NameTy super, ChunkList chunks)
    NameTy          (symbol name)
*   RecordTy        (DONE)

  ChunkList         (list_type chunks)

  Field             (symbol name, NameTy type_name)

  FieldInit         (symbol name, Exp init)


Some of these classes also inherit from other classes.

/Escapable/
  VarDec            (NameTy type_name, Exp init)

/Typable/
  /Dec/             (symbol name)
  /Exp/             ()
  /Ty/              ()

/TypeConstructor/
  /Ty/              ()
  FunctionDec       (VarChunk formals, NameTy result, Exp body)
  NilExp            ()
  TypeDec           (Ty ty)

