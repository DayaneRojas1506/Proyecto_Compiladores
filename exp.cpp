//
// Created by Jazmin on 6/11/2024.
//

#include "exp.h"
#include <iostream>
#include "exp.h"
using namespace std;
IFExp::IFExp(Exp* cond,Exp* l, Exp* r): cond(cond),left(l),right(r){}
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
UnaryExp::UnaryExp(Exp *u, UnaryOp op): unary(u), op(op) {}
NumberExp::NumberExp(int v):value(v) {}
BoolExp::BoolExp(bool v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
UnaryExp::~UnaryExp(){ delete unary;}
IFExp::~IFExp() {delete cond, delete left; delete right; }
NumberExp::~NumberExp() { }
BoolExp::~BoolExp() { }
IdentifierExp::~IdentifierExp() { }

AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(string format, Exp* e):format(format) , e(e) {}
PrintStatement::~PrintStatement() {
    delete e;
}

IfStatement::IfStatement(Exp* c, Body* t, Body* e): condition(c), then(t), els(e) {}
IfStatement::~IfStatement() {
    delete condition;
    delete then;
    delete els;
}
WhileStatement::WhileStatement(Exp* c, Body* t): condition(c), b(t) {}
WhileStatement::~WhileStatement() {
    delete condition;
    delete b;
}
ForStatement::ForStatement(Exp* s, Exp* e, Exp* st, Body* b): start(s), end(e), step(st), b(b) {}
ForStatement::~ForStatement() {
    delete start;
    delete end;
    delete step;
    delete b;
}


VarDec::VarDec(string type, list<string> vars): type(type), vars(vars) {}
VarDec::~VarDec() {}

VarDecList::VarDecList(): vardecs() {}
void VarDecList::add(VarDec* v) {
    vardecs.push_back(v);
}
VarDecList::~VarDecList() {
    for (auto v: vardecs) {
        delete v;
    }
}

StatementList::StatementList(): stms() {}
void StatementList::add(Stm* s) {
    stms.push_back(s);
}

StatementList::~StatementList() {
    for (auto s: stms) {
        delete s;
    }
}
FunDec::FunDec(string type, string name, list<pair<string,string>> params, Body* body):type(type), name(name), params(params), body(body){}
FunDec::~FunDec() {
    delete body;
}

Body::Body(VarDecList* v, StatementList* s): vardecs(v), slist(s) {}
Body::~Body() {
    delete vardecs;
    delete slist;
}

FunDecList::FunDecList():fundecs(){}
void FunDecList::add(FunDec* f) {
    fundecs.push_back(f);
}
FunDecList::~FunDecList() {
    for (auto f: fundecs) {
        delete f;
    }
}

FCallExp::FCallExp(std::string &name, list<Exp*> args): name(name) , args(args) {}
FCallExp::~FCallExp(){
    for(auto arg : args){
        delete arg;
    }
}


LibraDecList::LibraDecList(): librarys() {}

void LibraDecList::add(LibraDec *libradec){
    librarys.push_back(libradec);
}

LibraDecList::~LibraDecList() {
    for(auto lib : librarys){
        delete lib;
    }
}

Program::Program(LibraDecList* libradecs , VarDecList* vardecs, FunDecList* fundecs): libradecs(libradecs), vardecs(vardecs), fundecs(fundecs) {}

Program::~Program() {
    delete libradecs;
    delete vardecs;
    delete fundecs;
}
Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op) {
    string  c;
    switch(op) {
        case PLUS_OP: c = "+"; break;
        case MINUS_OP: c = "-"; break;
        case MUL_OP: c = "*"; break;
        case DIV_OP: c = "/"; break;
        case LT_OP: c = "<"; break;
        case LE_OP: c = "<="; break;
        case EQ_OP: c = "=="; break;
        default: c = "$";
    }
    return c;
}

string Exp::unaryopToChar(UnaryOp op) {
    string  c;
    switch(op) {
        case PLUSPLUS_OP: c = "++"; break;
        default: c = "$";
    }
    return c;
}