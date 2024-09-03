#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <list>
#include <map>
#include "lex.h"
#include "parse.h"
#include "pt.h"
#include "val.h"
#include <cxxabi.h>

using namespace std;

/*string demangle(const char* name) {
    int status;
    char* demangledName = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    string retVal = string(demangledName);
    free(demangledName);
    return retVal;
}

// Prints out the parse tree if debug is enabled.
void debugPrint(Pt* tree, uint level = 0) {
    if (!tree) {
        cout << "tree = null" << endl;
        return;
    }
    const string className = demangle(typeid(*tree).name());
    Pt* currTree = tree->left;
    cout << string(level, '\t') << className << '\n';
    if (currTree)
        debugPrint(currTree, level + 1);
    currTree = tree->right;
    if (currTree)
        // Fragile code, but it's for debugging anyway.
        debugPrint(currTree, level + 1);
}*/

int main(int argc, char* argv[]) {
    map<string,Value> symbols;
    int lineNumber = 0;
    istream *input = &cin;
    ifstream infile;
    infile.open(argv[argc-1]);

    for(int i = 1; i < argc; i++){
        if (i == argc-1){

            if (!infile.is_open()){
                cout << "CANNOT OPEN " << argv[i] << endl;
                return -1;
            }
            else {
                input = &infile;
                continue;
            }
        }
        else if (argc > 2) {
            cout << "ONLY ONE FILE NAME ALLOWED" << endl;
            return -1;
        }
    }
    
    Pt *prog = Prog(*input, lineNumber);
    //debugPrint(prog);
    if( prog == 0 ){
        return 0;
    }
    else {
        try{
        	prog->Eval(symbols);
    	}
        catch(std::string& e) {
            cout << "RUNTIME ERROR " << e << endl;
            
        }
        catch(char const* e){
            cout << "RUNTIME ERROR at "<< lineNumber << ": " << e << endl;
        }
    }
    
}
