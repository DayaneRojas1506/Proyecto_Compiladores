//
// Created by Jazmin on 6/11/2024.
//

#include <iostream>
#include "visitor.h"
#include "exp.h"
#include <unordered_map>

using namespace std;

int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}


int UnaryExp::accept(Visitor* visitor){
    return visitor->visit(this);
}

int IFExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int ForStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int StatementList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int Body::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int Program::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int FunDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}



int FunDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}



int ReturnStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}



int FCallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FCallStatement::accept(Visitor *visitor) {
   //return 
    visitor->visit(this);
   return 0;
}



///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}
int PrintVisitor::visit(UnaryExp* exp) {
    cout<<exp->unary<< Exp::unaryopToChar(exp->op) << ' ';
    return 0;
}



int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

void PrintVisitor::visit(AssignStatement* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout << "print(";
    cout << "\"" << stm->format << "n\",";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout << "if(";
    stm->condition->accept(this);
    cout << "){" << endl;
    stm->then->accept(this);
    if(stm->els){
        printIndent();
        cout << "}else{" << endl;
        stm->els->accept(this);
    }
    printIndent();
    cout << "}";
}

void PrintVisitor::imprimir(Program* program){
    cout<<"#include<stdio.h>"<<endl;
    cout<<endl;
    program->accept(this);
}

void PrintVisitor::visit(Program* program){

    program->fundecs->accept(this);
   //program  -> vardecs ->accept (this);
};


int PrintVisitor::visit(IFExp* pepito) {
    cout<< "ifexp(";
    pepito->cond->accept(this);
    cout<< ",";
    pepito->left->accept(this);
    cout<< ",";
    pepito->right->accept(this);
    cout << ")" ;
    return 0;
}

void PrintVisitor::visit(WhileStatement* stm){
    cout << "while( ";
    stm->condition->accept(this);
    cout << "){" << endl;
    stm->b->accept(this);
    printIndent();
    cout << "}";
}

void PrintVisitor::visit(ForStatement* stm){
    cout << "for( ";
    if(stm->type != ""){
        cout<<stm->type<<" ";
    }
    cout<<stm->name<<" = ";
    stm->start->accept(this);
    cout << ";";
    stm->end->accept(this);
    cout << "; ";
    stm->step->accept(this);
    cout << "){" << endl;
    stm->b->accept(this);
    cout << "}";
}

void PrintVisitor::visit(VarDec* stm){
    //cout << "var ";
    cout << stm->type;
    cout << " ";
    for(auto i: stm->vars){
        cout << i;
        if(i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
}

void PrintVisitor::visit(VarDecList* stm){
    for(auto i: stm->vardecs){
        printIndent();
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        printIndent();
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body* stm){
    increaseIndent();
    stm->vardecs->accept(this);
    stm->slist->accept(this);
    decreaseIndent();
}

void PrintVisitor::visit(FunDec* stm){
    cout<< stm->rtype <<  " " << stm->fname << "(";
    bool first = true;
    list<string>::iterator type, name;
    for (type = stm->types.begin(), name = stm->vars.begin(); type != stm->types.end(); type++, name++) {
        if (!first) cout << ", ";
        cout << *type << " " << *name;
        first = false;
    }
    cout << "){" << endl;
    stm->body->accept(this);
    cout << "}";
}

void PrintVisitor::visit(FunDecList* stm){
    for(auto i: stm->fundecs){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(ReturnStatement* s) {
    cout << "return ";
    if (s->returnExp != NULL) s->returnExp->accept(this);
    cout << ";";
    return;
}

int PrintVisitor::visit(FCallExp* e) {
    cout << e->name << "(";
    list<Exp*>::iterator it;
    bool first = true;
    for (it = e->args.begin(); it != e->args.end(); ++it) {
        if (!first) cout << ",";
        first = false;
        (*it)->accept(this);
    }
    cout << ')';
    return 0;
}

void PrintVisitor::visit(FCallStatement* stm){
    std::cout << stm->funcCall->name  << "(";
    bool first = true;
    for (Exp* arg : stm->funcCall->args) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;
        arg->accept(this);
    }
    std::cout << ")";


}




void PrintVisitor::printIndent() {
    for (int i = 0; i < indentLevel*2; i++) {
        cout << ' ';
    }

}


