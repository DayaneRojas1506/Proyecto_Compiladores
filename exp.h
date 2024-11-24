//
// Created by Jazmin on 6/11/2024.
//

#ifndef COMPILADORES_EXP_H
#define COMPILADORES_EXP_H

#include <string>
#include "imp_value.hh"
#include "imp_type.hh"
#include <unordered_map>
#include <list>
#include "visitor.h"

using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP,LT_OP, LE_OP, EQ_OP , GREATER_OP};
enum UnaryOp {PLUSPLUS_OP};

class Body;
class ImpValueVisitor;

class Visitor;

class Exp {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    virtual ImpValue accept(ImpValueVisitor* v) = 0;
    static string binopToChar(BinaryOp op);
    static string unaryopToChar(UnaryOp op);

};
class IFExp : public Exp {
public:
    Exp *left, *right;
    Exp* cond;

    IFExp(Exp* cond, Exp* l, Exp* r);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* v);
    ~IFExp();
};

class UnaryExp : public Exp {
public:
    string unary;
    UnaryOp op;
    UnaryExp(string u, UnaryOp op);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~UnaryExp();
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* v) ;
    ~BinaryExp();


};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor) override;
    ImpValue accept(ImpValueVisitor* v);
    ~NumberExp();
};

class BoolExp : public Exp {
public:
    int value;
    BoolExp(bool v);
    int accept(Visitor* visitor);
     ImpValue accept(ImpValueVisitor* v) ;
    ~BoolExp();
};

class IdentifierExp : public Exp {
public:
    std::string name;
    IdentifierExp(const std::string& n);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~IdentifierExp();
};

class FCallExp : public Exp {
public:
    string name;
    list<Exp*> args;
    FCallExp(string& name, list<Exp*> args);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~FCallExp();
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
    virtual void accept(ImpValueVisitor* v)=0;
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~AssignStatement();
};

//Modificar
class PrintStatement : public Stm {
public:
    Exp* e;
    string format;
    PrintStatement(string format,  Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~PrintStatement();
};


class IfStatement : public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStatement(Exp* condition, Body* then, Body* els);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~IfStatement();
};
class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* b;
    WhileStatement(Exp* condition, Body* b);
    int accept(Visitor* visitor);
     void accept(ImpValueVisitor* v);
    ~WhileStatement();
};

class ForStatement : public Stm {
public:
    // en caso que sea int x = 10 o x=10
    string type{};
    string name{};
    Exp* start;
    Exp* end;
    Exp* step;
    Body* b;
    ForStatement(string type, string name, Exp* start, Exp* end, Exp* step, Body* b);
    int accept(Visitor* visitor);
     void accept(ImpValueVisitor* v);
    ~ForStatement();
};



class VarDec {
public:
    string type;
    list<string> vars;
    VarDec(string type, list<string> vars);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~VarDec();
};

class VarDecList{
public:
    list<VarDec*> vardecs;
    VarDecList();
    void add(VarDec* vardec);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~VarDecList();
};

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm* stm);
     void accept(ImpValueVisitor* v);
    int accept(Visitor* visitor);
    ~StatementList();
};


class Body{
public:
    VarDecList* vardecs;
    StatementList* slist;
    Body(VarDecList* vardecs, StatementList* stms);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~Body();
};

class FunDec {
public:
    string fname, rtype;
    list<string> vars;
    list<string> types;
    Body *body;

    FunDec(string fname, list<string> types, list<string> vars, string rtype, Body *body);

    int accept(Visitor *v);

    void accept(ImpValueVisitor *v);

    ~FunDec();
};

class FunDecList {
public:
    list<FunDec*> fundecs;
    FunDecList();
    void add(FunDec* fundec);
    int accept(Visitor* visitor);
   void accept(ImpValueVisitor* v);
    ~FunDecList();
};



class ReturnStatement : public Stm {
public:
    Exp* returnExp;
    ReturnStatement(Exp* returnExp): returnExp(returnExp){}
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~ReturnStatement(){ delete returnExp;}
};

class FCallStatement : public Stm {
public:
    FCallExp* funcCall;
    // Constructor que recibe un FCallExp
    FCallStatement(FCallExp* funcCall) : funcCall(funcCall) {}
    int accept(Visitor* v);
    void accept(ImpValueVisitor* v);
    ~FCallStatement() {
        delete funcCall;
    }

};


class Program {
public:
    Body* body;
    FunDecList* fundecs;
    Program( FunDecList* fundecs);
    int accept(Visitor* visitor);
    void accept (ImpValueVisitor* v);
    ~Program();
};




#endif //COMPILADORES_EXP_H
