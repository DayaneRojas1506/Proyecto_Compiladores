
#ifndef IMP_VALUE_VISITOR
#define IMP_VALUE_VISITOR

#include "exp.h"
#include "imp_value.hh"

class ImpValueVisitor {
public:
  virtual void visit(Program* p) = 0;
  virtual void visit(Body* b) = 0; 
  virtual void visit(VarDecList* e) = 0;
  virtual void visit(VarDec* e) = 0;
  virtual void visit(FunDecList* e) = 0;
  virtual void visit(FunDec* e) = 0;
  virtual void visit(StatementList* e) = 0;
  virtual void visit(AssignStatement* e) = 0;
  virtual void visit(PrintStatement* e) = 0;
  virtual void visit(IfStatement* e) = 0;
  virtual void visit(WhileStatement* e) = 0;
  virtual void visit(ReturnStatement* e) = 0;
  virtual void visit(ForStatement* e) = 0;
 
  virtual void visit(FCallStatement* e) = 0;
  virtual ImpValue visit(BinaryExp* e) = 0;
  virtual ImpValue visit(BoolExp* e) = 0;
  virtual ImpValue visit(NumberExp* e) = 0;
  virtual ImpValue visit(IdentifierExp* e) = 0;
  virtual ImpValue visit(IFExp* e) = 0;
  virtual void visit(FCallExp* e) = 0;
 
};

class ConcreteImpCODE : public ImpCODE {
public:
    void visit(Program* p) override {
        // Implementación específica para Program
        std::cout << "Visitando Program" << std::endl;
    }

    void visit(Body* b) override {
        // Implementación específica para Body
        std::cout << "Visitando Body" << std::endl;
    }

    void visit(VarDecList* e) override {
        // Implementación específica para VarDecList
        std::cout << "Visitando VarDecList" << std::endl;
    }

    void visit(VarDec* e) override {
        // Implementación específica para VarDec
        std::cout << "Visitando VarDec" << std::endl;
    }

    void visit(FunDecList* e) override {
        // Implementación específica para FunDecList
        std::cout << "Visitando FunDecList" << std::endl;
    }

    void visit(FunDec* e) override {
        // Implementación específica para FunDec
        std::cout << "Visitando FunDec" << std::endl;
    }

    void visit(StatementList* e) override {
        // Implementación específica para StatementList
        std::cout << "Visitando StatementList" << std::endl;
    }

    void visit(AssignStatement* e) override {
        // Implementación específica para AssignStatement
        std::cout << "Visitando AssignStatement" << std::endl;
    }

    void visit(PrintStatement* e) override {
        // Implementación específica para PrintStatement
        std::cout << "Visitando PrintStatement" << std::endl;
    }

    void visit(IfStatement* e) override {
        // Implementación específica para IfStatement
        std::cout << "Visitando IfStatement" << std::endl;
    }

    void visit(WhileStatement* e) override {
        // Implementación específica para WhileStatement
        std::cout << "Visitando WhileStatement" << std::endl;
    }

    void visit(ReturnStatement* e) override {
        // Implementación específica para ReturnStatement
        std::cout << "Visitando ReturnStatement" << std::endl;
    }

    void visit(ForStatement* e) override {
        // Implementación específica para ForStatement
        std::cout << "Visitando ForStatement" << std::endl;
    }

    void visit(FCallStatement* e) override {
        // Implementación específica para FCallStatement
        std::cout << "Visitando FCallStatement" << std::endl;
    }

    ImpValue visit(BinaryExp* e) override {
        // Implementación específica para BinaryExp
        std::cout << "Visitando BinaryExp" << std::endl;
        return ImpValue(); // Retorno ejemplo
    }

    ImpValue visit(BoolExp* e) override {
        // Implementación específica para BoolExp
        std::cout << "Visitando BoolExp" << std::endl;
        return ImpValue(); // Retorno ejemplo
    }

    ImpValue visit(NumberExp* e) override {
        // Implementación específica para NumberExp
        std::cout << "Visitando NumberExp" << std::endl;
        return ImpValue(); // Retorno ejemplo
    }

    ImpValue visit(IdentifierExp* e) override {
        // Implementación específica para IdentifierExp
        std::cout << "Visitando IdentifierExp" << std::endl;
        return ImpValue(); // Retorno ejemplo
    }

    ImpValue visit(IFExp* e) override {
        // Implementación específica para IFExp
        std::cout << "Visitando IFExp" << std::endl;
        return ImpValue(); // Retorno ejemplo
    }

    void visit(FCallExp* e) override {
        // Implementación específica para FCallExp
        std::cout << "Visitando FCallExp" << std::endl;
    }

    void interpret(Program* program) override {
        // Implementación del método interpret para ejecutar el programa
        std::cout << "Interpretando el programa..." << std::endl;
        program->accept(this); // Recorre el programa usando el patrón visitor
    }
};

#endif