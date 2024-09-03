#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "val.h"

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

class Pt {
public:
	int	linenum;
	Pt	*left;
	Pt	*right;

public:
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~Pt() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }

	int Traverse(bool (Pt::*func)(void) const) const {
        int cnt = 0;
		if( left ) cnt += left->Traverse(func);
		if( right ) cnt += right->Traverse(func);
        if( (this->*func)() ) {
            ++cnt;
        }
		return cnt;
	}

	int MaxDepth() const {
        int lc = 0, rc = 0;
		if( left ) lc = left->MaxDepth();
		if( right ) rc += right->MaxDepth();
		if( lc > rc ) return lc + 1;
        return rc + 1;
	}

    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }

    void CheckAssignments(map<string,bool>& var) {
		if( left ) left->CheckAssignments(var);
		if( right ) right->CheckAssignments(var);
        this->CheckAssign(var);
    }
        
	virtual void CheckAssign(map<string,bool>& var) {
		return;
	}
    
    virtual Value Eval(map<string,Value>& syms) const = 0;
};

class StmtList : public Pt {

public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    
    Value Eval(map<string,Value>& symbols) const override {
        left->Eval(symbols);
        if( right ) right->Eval(symbols);
        return Value();
    }
};

class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map <string,Value>& symbols) const override{
        cout << left->Eval(symbols);
        return Value(); //left->Eval(symbols)
    }
};

class Println : public Pt {
public:
	Println(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map <string,Value>& symbols) const override{
        cout << left->Eval(symbols) << endl;
        return Value();
    }
};

class Repeat : public Pt {
public:
	Repeat(int line, Pt *e, Pt *s) : Pt(line, e, s) {}
    
    Value Eval(map <string,Value>& symbols) const override{
        for(int i = left->Eval(symbols).GetInt(); i > 0; i--) {
        	symbols[left->getId()] = right->Eval(symbols);
        }
        return Value(symbols[left->getId()]);
        /*while(left->Eval(symbols).GetInt() != 0){
            symbols[left->getId()] = right->Eval(symbols);
        }
        //cout << symbols[left->getId()] << endl;
        return Value();*/
    }
};

class Assign : public Pt {
    string id;
public:
    //Assign(int line, string id, Pt *l) : Pt(line, l), id(id){}
	Assign(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    
    //string getId() const { return id; }
    bool isEq() const { return true; }
    
	virtual void CheckAssign(map<string,bool>& var) {
		right->CheckAssign(var);
		var[ this->id ] = true;
	}
    
    Value Eval(map<string,Value>& symbols) const override { 
        Value newVal = right->Eval(symbols);
        symbols[left->getId()] = newVal;
        //return Value(newVal);
        return Value(symbols[left->getId()]);
    }
    
};

class PlusExpr : public Pt {
public:
	PlusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    bool isPlus() const { return true; }
    
    Value Eval(map <string,Value>& symbols) const override{
        auto L = left->Eval(symbols);
        auto R = right->Eval(symbols);
        
        auto out = L + R;
        
        return out;
        
    }
};

class MinusExpr : public Pt {
public:
	MinusExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map <string,Value>& symbols) const override{
        auto L = left->Eval(symbols);
        auto R = right->Eval(symbols);
        
        auto out = L - R;
        
        return out;
        
    }
};

class TimesExpr : public Pt {
public:
	TimesExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map <string,Value>& symbols) const override{
        auto L = left->Eval(symbols);
        auto R = right->Eval(symbols);
        
        auto out = L * R;
        
        return out; 
    }
};

class DivideExpr : public Pt {
public:
	DivideExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map <string,Value>& symbols) const override{
        auto L = left->Eval(symbols);
        auto R = right->Eval(symbols);
        
        auto out = L / R;
        
        return out;
    }
};

class IConst : public Pt {
	int val;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    
    Value Eval(map<string,Value>& symbols) const override { return Value(val); }
};

class SConst : public Pt {
	string val;

public:
	SConst(Tok& t) : Pt(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    
    Value Eval(map<string,Value>& symbols) const override { return Value(val); }
};

class Ident : public Pt {
	string id;

public:
	Ident(Tok& t) : Pt(t.GetLinenum()), id(t.GetLexeme()) {}

	bool isIdent() const { return true; }
	string getId() const { return id; }
	virtual void CheckAssign(map<string,bool>& var) {
    if( var.find(getId()) == var.end() )
    	std::cout << "UNDECLARED VARIABLE " << getId() << std::endl;
    }
     
    Value Eval(map<string,Value>& symbols) const override {
        if(symbols.find(id) != symbols.end()){
            return symbols[id];
        }
        else
            throw "at " + to_string(GetLineNumber() + 1) + ": Variable used but not assigned";
        return Value();
    }
};
#endif /* PT_H_ */