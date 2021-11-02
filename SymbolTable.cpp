#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    fstream testcase;
    testcase.open(filename, ios:: in);
    string input;
    Tree* tree = new Tree();
    int level = 0;
    while (!testcase.eof()){
        getline(testcase, input);
        int space = Space(input);
        if(space > 3)
            throw InvalidInstruction(input);
        int k = input.find(' ');
        string task = input.substr(0, k);
        if(space == 3){
            if(task == "INSERT"){
                string st = input.substr(k+1, input.length());
                int t = st.find(' ');
                string data = st.substr(0, t);
                string tp = st.substr(t+1, st.length());
                t = tp.find(' ');
                string type = tp.substr(0, t);
                string scope = tp.substr(t+1, st.length());
                bool stic;
                if(scope == "true") stic = true;
                else stic = false;
                if(type != "string"){
                    if(type != "number"){
                        if(!funct(type))
                            throw InvalidInstruction(input);
                    }
                }
                if(!identifier(data))
                    throw InvalidInstruction(input);
                if( !insert(tree, data,type,stic,level))
                    throw Redeclared(input);
                tree->insert(data,type,stic,level);
                cout << tree->root->comp << " " << tree->root->splay << endl;
            }
            else throw InvalidInstruction(input);
        }
        if(space == 2){
            if(task == "ASSIGN"){
                string st = input.substr(k+1, input.length());
                int t = st.find(' ');
                string data = st.substr(0, t);
                string value = st.substr(t+1, st.length());
                if(!identifier(data))
                    throw InvalidInstruction(input);
                if( !(Val( value) || (tree->find(value) != 0)))
                    throw InvalidInstruction(input);
                if(tree->find(value) == 0 && tree->find(data) == 0)
                    throw Undeclared(input);
                if(!assign(tree, data, value))
                    throw TypeMismatch(input);
                cout << tree->root->comp << " " << tree->root->splay << endl;
            }
            else throw InvalidInstruction(input);
        }
        if(space == 1){
            if(task == "LOOKUP"){
                string data = input.substr(k+1, input.length());
                if(!identifier(data))
                    throw InvalidInstruction(input);
                if(tree->look(data) == -1)
                    throw  Undeclared(input);
                cout << tree->look(data) << endl;
            }
            else throw InvalidInstruction(input);
        }
        if(space == 0){
            if(task == "BEGIN") level++;
            else if(task == "END"){
                if(level == 0)
                    throw  UnknownBlock();
                while (tree->find(level)){
                    tree->remove(level);
                }
                level--;
            }
            else if(task == "PRINT") tree->print(); // forget "phot lo tam vuc"
            else throw InvalidInstruction(input);
        }
    }
    testcase.close();
    if(level > 0)
        throw UnclosedBlock(level);
}