#ifndef GENCODE
#define GENCODE

#include <unordered_map>

#include "exp.h"
#include "imp_value_visitor.hh"
#include "environment.hh"

using namespace std;

class ImpCODE : public ImpValueVisitor {
private:
  Environment<ImpValue> env;

public:
  int etiquetas=0;
  void interpret(Program*);
  void visit(Program*);
  void visit(Body*);
  void visit(VarDecList*);
  void visit(VarDec*);
  void visit(StatementList*);
  void visit(AssignStatement*);
  void visit(PrintStatement*);
  void visit(IfStatement*);
  void visit(WhileStatement*);
  void visit(ForStatement* e);
  ImpValue visit(BinaryExp* e);
 // ImpValue visit (UnaryExp* e);
  ImpValue visit(BoolExp* e);
  ImpValue visit(NumberExp* e);
  ImpValue visit(IdentifierExp* e);
  ImpValue visit(IFExp* e);
};


#endif

