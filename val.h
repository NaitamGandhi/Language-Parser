#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
    ValType	T;
	int		I;
    string	S;
    
public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }
    
    
    int GetInt() const { if( IsInt() ) return I; throw "control expression not an integer"; }
    string GetStr() const { if( IsStr() ) return S; throw "RUNTIME ERROR: Value not a string"; }
       
    // add op to this
    Value operator+(const Value& op) const{
        if( IsInt() && op.IsInt() ) 
            return GetInt() + op.GetInt();
        if( IsStr() && op.IsStr() )
            return GetStr() + op.GetStr();
        throw "Type error on addition";
    }
    	
    // subtract op from this
    Value operator-(const Value& op) {
     if(IsInt() && op.IsInt() ){ 
         int j = GetInt() - op.GetInt();
        	return Value(j);
     }
        if( IsStr() && op.IsStr() ){
            string s = GetStr();
            int length = GetStr().length();
            int pos = s.find(op.GetStr());

            if(GetStr().find(op.GetStr())!=string::npos){
                if (pos == length-2){
                    s.erase(pos,pos-1);
                    s+=GetStr().at(length-1);
                    return Value(s);
                }
                s.erase(pos,pos-1);
                return Value(s);
            }
            return Value(GetStr());
        }
        throw "Type error on subtraction";
    }
    /*Value operator-(const Value& op) const{
        string temp = "";
        if( IsInt() && op.IsInt() ) 
        	int j = GetInt() - op.GetInt();
        	return Value(j);
        if( IsStr() && op.IsStr() ) {
            if(GetStr().find(op.GetStr()) != string::npos)
                temp = GetStr();
                temp.erase(temp.find(op.GetStr()), op.GetStr().length());
            return Value(temp);
        }
        throw "Type error on subtraction";
    }*/
    
    // multiply this by op
    Value operator*(const Value& op) const{
        if( IsInt() && op.IsInt() ){ // integer addition
            int j = GetInt() * op.GetInt();
            return Value(j);
        }
        else if(IsStr() && op.IsInt()){
            if(op.IsInt() > 0){
                string s = "";
                for(int i = 0; i < op.GetInt();i++){
                       s += GetStr();
                }
                return Value(s);
            }
        }
        else if(op.IsStr() && IsInt()){
            if(IsInt() > 0){
                string s = "";
                for(int i=0; i < GetInt();i++){
                       s += op.GetStr();
                }
            	return Value(s);
            }    
        }
        throw "Type error on multiplication" ; 
    }
    /*Value operator*(const Value& op) const{
        string temp = "";
        if( IsInt() && op.IsInt() ) 
        	return GetInt() * op.GetInt();
        if( (IsStr() && op.IsInt()) ){
            for(int i = 0; i < op.GetInt(); i++) {
                temp += GetStr();
            }
            return temp;
    	}
        if( (IsInt() && op.IsStr()) ){
            for(int i = 0; i < GetInt(); i++) {
                temp += op.GetStr();
            }
            return temp;
        }
        if( IsStr() && op.IsStr() ) {
        	throw "Type error on multiplication";
        }
        return 0;
    }*/
    
    // divide this by op
    Value operator/(const Value& op) const{
        if( IsInt() && op.IsInt() ) {
            int j = GetInt() / op.GetInt();
        	return Value(j);
        }
        if( IsStr() && op.IsStr() ) {
            return 0;
        }
        throw "Type error on division";
    }
    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR";
        return out;
    }
};

#endif