#include "gencode.hh"
/*ImpValue UnaryExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}*/

ImpValue BinaryExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue BoolExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue NumberExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue IdentifierExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


ImpValue IFExp::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


void AssignStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void PrintStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void IfStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void WhileStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void ForStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void StatementList::accept(ImpValueVisitor* v) { v->visit(this); }
void VarDec::accept(ImpValueVisitor* v) { v->visit(this); }
void VarDecList::accept(ImpValueVisitor* v) { v->visit(this); }
void Body::accept(ImpValueVisitor* v) { v->visit(this); }
void FunDec::accept(ImpValueVisitor* v) { v->visit(this); }
void FunDecList::accept(ImpValueVisitor* v) { v->visit(this); }
void ReturnStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void FCallStatement::accept(ImpValueVisitor* v) { v->visit(this); }
void Program::accept(ImpValueVisitor* v) { v->visit(this); }
void ImpCODE::interpret(Program* p) {
    etiquetas = 0;  // Inicializamos el contador de etiquetas
    env.clear();
    cout << "Run program:" << endl;
    p->accept(this);
    cout << "End of program execution" << endl;
}


void ImpCODE::visit(Program* p) {
    env.add_level();
    p->fundecs->accept(this);
}

void ImpCODE::visit(Body* b) {
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
    return;
}

void ImpCODE::visit(VarDecList* decs) {
    list<VarDec*>::iterator it;
    for (it = decs->vardecs.begin(); it != decs->vardecs.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}


void ImpCODE::visit(VarDec* vd) {
    list<string>::iterator it;
    ImpValue v;
    ImpVType tt = ImpValue::get_basic_type(vd->type);
    v.set_default_value(tt);
    for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
        env.add_var(*it, v);
    }
    return;
}



void ImpCODE::visit(StatementList* s) {
    list<Stm*>::iterator it;
    for (it = s->stms.begin(); it != s->stms.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}

void ImpCODE::visit(AssignStatement* s) {
    cout << "LDA " << s->id << endl ;
    ImpValue v = s->rhs->accept(this);
    ImpValue lhs = env.lookup(s->id);
    env.update(s->id, v);
    cout << "STOc"<< endl;
    return;
}

void ImpCODE::visit(PrintStatement* s) {
    ImpValue v = s->e->accept(this);
    if (v.type == TINT) {
        cout << "printf(\"%d\\n\", " << v.int_value << ");\n";
    } else if (v.type == TBOOL) {
        cout << "printf(\"%d\\n\", " << (v.bool_value ? 1 : 0) << ");\n";
    }
}


/*
 * var int x = 10;
 * if( 1 > x) <-
 * then print(x);
 * else print(1);
 */

void ImpCODE::visit(IfStatement* s) {
    ImpValue condition = s->condition->accept(this);

    int elseLabel = etiquetas++;
    int endLabel = etiquetas++;

    cout<< "FJP " << elseLabel << endl; //Salto en falso
    s->then->accept(this);
    cout<< "UJP " << endLabel << endl; //Sato incondicional que si o si caera

    cout<< "L " << elseLabel << endl;
    if (s->els != nullptr) {
        s->els->accept(this);
    }
    cout<< "L " << endLabel << endl;
}


void ImpCODE::visit(WhileStatement* s) {
    int loopLabel = etiquetas++;
    int endLabel = etiquetas++;

    cout << "L " << loopLabel << endl;  // Inicio del bucle
    ImpValue condition = s->condition->accept(this);
    cout << "FJP " << endLabel << endl;  // Salto si la condición es falsa
    s->b->accept(this);
    cout << "UJP " << loopLabel << endl;  // Regresamos al inicio del bucle
    cout << "L " << endLabel << endl;    // Fin del bucle
}

 
void ImpCODE::visit(ForStatement* s){
    int loopLabel = etiquetas++;
    int endLabel = etiquetas++;

    cout<< "L " << loopLabel << endl; // Iniciamos el for

    //Condiciones
    s->start->accept(this);
    s->end->accept(this);
    s->step->accept(this);

    cout << "FJP " << endLabel << endl;

    s->b->accept(this);

    cout << "UJP " << loopLabel << endl;

    cout << "L " << endLabel << endl;
}



ImpValue ImpCODE::visit(BinaryExp* e) {
    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);
    int iv, iv1, iv2;
    bool bv;
    ImpVType type = NOTYPE;
    iv1 = v1.int_value;
    iv2 = v2.int_value;
    switch (e->op) {
        case PLUS_OP:
            iv = iv1 + iv2;
            type = TINT;
            cout<< "ADI" << endl;
            break;
        case MINUS_OP:
            iv = iv1 - iv2;
            type = TINT;
            cout<< "SBI" << endl;
            break;
        case MUL_OP:
            iv = iv1 * iv2;
            type = TINT;
            cout<< "MPI" << endl;
            break;
        case DIV_OP:
            iv = iv1 / iv2;
            type = TINT;
            cout<< "DVI" << endl;
            break;
        case LT_OP:
            bv = (iv1 < iv2) ? 1 : 0;
            type = TBOOL;
            cout<< "LESc" << endl;
            break;
        case LE_OP:
            bv = (iv1 <= iv2) ? 1 : 0;
            type = TBOOL;
            cout<< "LEQc" << endl;
            break;
        case EQ_OP:
            bv = (iv1 == iv2) ? 1 : 0;
            type = TBOOL;
            cout<< "EQUc" << endl;
            break;
    }
    if (type == TINT)
        result.int_value = iv;
    else
        result.bool_value = bv;
    result.type = type;
    return result;
}

ImpValue ImpCODE::visit(NumberExp* e) {
    ImpValue v;
    v.set_default_value(TINT);
    v.int_value = e->value;
    cout << "LDCc " <<  e->value << endl;
    return v;
}

ImpValue ImpCODE::visit(BoolExp* e) {
    ImpValue v;
    v.set_default_value(TBOOL);
    v.int_value = e->value;
    return v;
}

ImpValue ImpCODE::visit(IdentifierExp* e) {
    if (env.check(e->name))
        cout << "LODc " << e->name <<endl;
    return env.lookup(e->name);
}

ImpValue ImpCODE::visit(IFExp* e) {
    // Evaluar la condicion
    ImpValue condition = e->cond->accept(this);

    if (condition.type != TBOOL) {
        cout << "Error de tipo: la condicion en IFExp debe ser de tipo BOOL." << endl;
        exit(1);
    }


    int elseLabel = etiquetas++;
    int endLabel = etiquetas++;
    cout << "FJP " << elseLabel << endl;


    ImpValue leftResult = e->left->accept(this);
    //cout << "STOc temp" << endl;


    cout<< "FJP " << elseLabel << endl; //Salto en falso

    cout << "L " << elseLabel << endl;
    ImpValue rightResult = e->right->accept(this);
    //cout << "STOc" << endl;


    cout << "L " << endLabel << endl;


    //cout << "LODc temp" << endl;
    ImpValue result;

    result.type = leftResult.type;

    return result;

}

// Implementación de las funciones que faltan
void ImpCODE::visit(FunDec* e) {
    // Procesamos la declaración de la función
    // Por ejemplo, podemos generar código para la cabecera de la función.
    cout << "Generando código para la función: " << e->fname << endl;
    
    // Procesamos las variables locales de la función
    for (auto& var : e->vars) {
        cout << "Variable local: " << var << endl;
    }
    
    // Procesamos el tipo de retorno de la función
    cout << "Tipo de retorno: " << e->rtype << endl;

    // Si la función tiene un cuerpo, visitamos ese cuerpo
    if (e->body) {
        e->body->accept(this);  // Visitamos el cuerpo de la función.
    }
}

void ImpCODE::visit(FunDecList* e) {
    // Recorremos la lista de funciones
    for (auto& fundec : e->fundecs) {
        fundec->accept(this);  // Visitamos cada declaración de función.
    }
}

void ImpCODE::visit(ReturnStatement* e) {
     cout << "return ";
    e->returnExp->accept(this);  // Llamar al visitante de la expresión de retorno
    cout << ";\n";  // Terminar la instrucción con un punto y coma
}

/*ImpValue ImpCODE::visit(UnaryExp *e)
{
    return ImpValue();
}
*/
void ImpCODE::visit(FCallStatement* e) {
    // Llamar al visitante de la expresión de llamada a función
    e->funcCall->accept(this);
    cout << ";\n"; // Finalizar la declaración de la llamada con un punto y coma
}
ImpValue ImpCODE::visit(FCallExp* e) {
    // Generar código para la llamada a la función
    cout << e->name << "(";
    
    for (auto it = e->args.begin(); it != e->args.end(); ++it) {
        (*it)->accept(this);  // Llamar a visit para cada argumento
        if (next(it) != e->args.end()) cout << ", ";
    }
    cout << ")";
    
    // Dependiendo de los requisitos, podría devolver un valor ImpValue aquí
    return ImpValue();  // Retornar un valor adecuado (por ejemplo, de tipo ImpValue)
}




