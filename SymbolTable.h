#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class Node{
public:
    string data;
    string type;
    bool stic;
    int level;
    int comp,splay;
    Node *left = nullptr;
    Node *right = nullptr;
    Node():left(nullptr),right(nullptr){};
    Node(string data, string type, bool stic,int level): data(data), type(type),stic(stic),level(level), comp(0), splay(0), left(nullptr),right(
            nullptr) {};
    void getsplay(int count){
        this->splay = count;
    }
    void getcomp(int count){
        this->comp = count;
    }
    ~Node()= default;;
};
class Tree{
private:
    Node *rightRotate(Node *x){
        Node *y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }
    Node *leftRotate(Node *x){
        Node *y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }
    Node *splay(Node *node, const string& data,const string& type, bool stic, int level, int& count){
        if (node == nullptr || (node->level == level && node->data == data))
            return node;
        count++;
        if (node->level > level || (node->level == level && node->data.compare(data) > 0)){
            if (node->left == nullptr){
                return node;
            }
            // Zig-Zig
            if (node->left->level > level || (node->left->level == level && node->left->data.compare(data) > 0)){
                node->left->left = splay(node->left->left, data, type, stic, level,count);
                node = rightRotate(node);
            }
            else if (node->left->level <= level || (node->left->level == level && node->left->data.compare(data) <= 0)){
                // Zig-Zag
                node->left->right = splay(node->left->right, data, type,stic,level,count);
                if (node->left->right != nullptr)
                    node->left = leftRotate(node->left);
            }
            return (node->left == nullptr)? node: rightRotate(node);
        }
        else{
            if (node->right == nullptr) return node;
            // Zag-Zig (Right Left)
            if (node->right->level > level || (node->right->level == level && node->right->data.compare(data) > 0)){
                node->right->left = splay(node->right->left, data, type,stic,level,count);
                if (node->right->left != nullptr)
                    node->right = rightRotate(node->right);
            }
            else{
                // Zag-Zag (Right Right)
                node->right->right = splay(node->right->right, data, type,stic,level,count);
                node = leftRotate(node);
            }
            return (node->right == nullptr)? node: leftRotate(node);
        }
    }
    Node *splay(Node *node, int level, int count){
        if (node == nullptr || node->level == level)
            return node;
        count++;
        if (node->level > level){
            if (node->left == nullptr)
                return node;
            if (node->left->level > level)
            {
                node->left->left = splay(node->left->left, level, count);
                node = rightRotate(node);
            }
            else if (node->left->level <= level){
                // Zig-Zag
                node->left->right = splay(node->left->right, level, count);
                if (node->left->right != nullptr)
                    node->left = leftRotate(node->left);
            }
            return (node->left == nullptr)? node: rightRotate(node);
        }
        else{
            if (node->right == nullptr)
                return node;
            // Zag-Zig
            if (node->right->level > level){
                node->right->left = splay(node->right->left, level, count);
                if (node->right->left != nullptr)
                    node->right = rightRotate(node->right);
            }
            else if (node->right->level <= level){
                // Zag-Zag
                node->right->right = splay(node->right->right, level, count);
                node = leftRotate(node);
            }
            return (node->right == nullptr)? node: leftRotate(node);
        }
    }
    Node *insert(Node *& node,const string& data, const string& type, bool stic, int level, int& count){
        Node *temp = new Node( data, type, stic, level);
        if (node == nullptr){
            node = temp;
            return node;
        }
        count++;
        if (level > node->level){
            node->right = insert(node->right, data, type, stic, level, count);
        }
        else if (level == node->level && data.compare(node->data) >= 0){
            node->right = insert(node->right, data, type, stic, level, count);
        }
        else{
            node->left = insert(node->left, data, type, stic, level, count);
        }
        return node;
    }
    bool find(Node* node, string& data){
        if(node == nullptr)
            return false;
        if(node->data == data)
            return true;
        return (find(node->left, data)|| find(node->right, data));
    }
    bool find(Node* node, int& level){
        if(node == nullptr)
            return false;
        if(node->level == level)
            return true;
        return (find(node->left, level)|| find(node->right, level));
    }
    Node* Find(Node* node, string& data, int& count){
        if(node == nullptr)
            return nullptr;
        count++;
        if(node->data == data)
            return node;
        count++;
        Node * temp = Find(node->left,data, count);
        if(temp == nullptr)
            return Find(node->right, data, count);
        return temp;
    }
    int look(Node* node, const string& data){
        if(node == nullptr){
            return -1;
        }
        if(data == node->data) return node->level;
        return max(look(node->right, data), look(node->left, data));
    }
    Node *maxValueNode(Node* node){
        Node* curr = node;
        while (curr->right)
            curr = curr->right;
        return curr;
    }
    Node* remove(Node*& node){
        if (node == nullptr)
            return node;
        if (node->left == nullptr){
            Node* temp = node;
            node = node->right;
            temp->right = nullptr;
            free(temp);
        }
        else{
            Node* temp = maxValueNode(node->left);
            int count = 0;
            node->left = splay(node->left,temp->data,temp->type,temp->stic, temp->level,count);
            temp = node;
            node = node->left;
            node->right = temp->right;
            temp->left = nullptr;
            temp->right = nullptr;
            free(temp);
        }
        return node;
    }
    void PrintPre(Node* tree){
        if(tree){
            cout << tree->data << "//" << tree->level << " ";
            PrintPre(tree->left);
            PrintPre(tree->right);
        }
    }
public:
    Node* root;
    Tree():root(nullptr){};
    void splay(const string& data, const string& type, bool stic, int level){
        int count = 0;
        this->root = splay(this->root, data, type, stic, level, count);
        root->getsplay(count);
    }
    void splay(int level){
        int count = 0;
        this->root = splay(this->root, level, count);
        root->getsplay(count);
    }
    void insert(const string& data, const string& type, bool stic, int level){
        int count_n = 0;
        int count_s = 0;
        int Level = level;
        if(stic) Level = 0;
        this->root = insert(this->root, data, type, stic, Level, count_n);
        this->root = splay(this->root, data, type, stic, Level, count_s);
        root->getsplay(count_s);
        root->getcomp(count_n);
    }
    int find(string& data){
        string value;
        int size = data.length();
        int k = 0;
        for (int i = 0; i < size; ++i) {
            if(data[i] == '('){
                k = i;
                break;
            }
        }
        value = (k > 0) ? data.substr(0,k): data;
        bool kt = find(root, value);
        if(kt){
            if( k == 0) return -1;
            return 1;
        }
        return 0;
    }
    bool find(int& level){
        return find(root, level);
    }
    Node* Find(string& data){
        string value;
        int count = 0;
        int size = data.length();
        int k = 0;
        for (int i = 0; i < size; ++i) {
            if(data[i] == '('){
                k = i;
                break;
            }
        }
        value = (k > 0) ? data.substr(0,k): data;
        Node *temp = Find(root, value, count);
        temp->getcomp(count);
        return temp;
    }
    int look(const string& data){
        return look(root, data);
    }
    void remove(int level){
        splay(level);
        root = remove(root);
    }
    void print(){
        if(root){
            PrintPre(root);
            cout << endl;
        }
    }
};
int Space(string input){
    int size;
    size = input.length();
    int count = 0;
    int i = 0;
    while(i < size) {
        if(input[i] == ' ') count++;
        if(input[i] == 39){
            string st = input.substr(i+1, size - i -1);
            int k;
            k = st.find(39);
            i = i + k + 2;
        }
        else i++;
    }
    return count;
}
bool identifier(string data){
    int size;
    size = data.length();
    if(data[0] > 'z' || data[0] < 'a') return false;
    for (int i = 1; i < size; ++i) {
        if(!((data[i] <= 'z' && data[i] >= 'a') || (data[i] <= 'Z' && data[i] >= 'A') || (data[i] =='_')|| (data[i] <='9' && data[i]>='0')))
            return false;
    }
    return true;
}
bool funct(const string &type){
    int size = type.length();
    bool kt = false;
    for (int i = 0; i < size; ++i) {
        if(type[i] == '-'){
            kt = true;
            break;
        }
    }
    if(!kt) return false;
    int k = type.find('-');
    if(type[0] != '(' || type[k -1] != ')') return false;
    if(type[k+1] != '>') return false;
    string tp = type.substr(k+2,size);
    if(tp != "string"){
        if(tp != "number") return false;
    }
    string s = type.substr(1,k-2);
    string tp1;
    if(k == 2) return true;
    while (k != -1){
        k = s.find(',');
        tp = s.substr(0,k);
        tp1 = s.substr(k+1,s.size());
        if(tp != "string"){
            if(tp != "number") return false;
        }
        s = tp1;
    }
    return true;
}
bool insert(Tree*& tree, const string& data, const string& type,const bool& stic, int level){
    if(tree->root == nullptr){
        return true;
    }
    Node* pre = tree->root;
    while (pre){
        if((pre->data.compare(data) == 0) && (pre->level == level)&&(pre->stic == stic))
            return false;
        if (level > pre->level){
            pre = pre->right;
        }
        else if (level == 0 && data.compare(pre->data) > 0){
            pre = pre->right;
        }
        else
            pre = pre->left;
    }
    return true;
}
bool constNumber(string val){
    int size;
    size = val.length();
    for (int i = 0; i < size; i++) {
        if(val[i] <'0' || val[i] >'9') return false;
    }
    return true;
}
bool key(char k){
    if((k >= '0' && k <= '9') || (k >= 'a' && k <= 'z') ||(k >= 'A' && k <= 'Z') || (k == ' '))
        return true;
    return false;
}
bool constString(string val){
    int size;
    size = val.length();
    if(val[0] != 39 || val[size-1] != 39) return false;
    for (int i = 1; i < size-1 ; i++) {
        if(!key(val[i]))
            return false;
    }
    return true;
}
bool Val(const string& value){
    if(identifier(value) || constNumber(value) || constString(value)) return true;
    return false;
}
string Type(Tree*tree, const string& data){
    if(tree->find((string &) data) != 0){
        Node* temp = tree->Find((string &)data);
        return temp->type;
    }
    if(constString(data)) return "string";
    if(constNumber(data)) return "number";
    return "novalue";
}
string Typeval(const string& type, const string& val){
    int size1 = type.length();
    int k = type.find('-');
    string string_out = type.substr(k+2,size1);
    string s1 = type.substr(1,k-2);
    size1 = s1.length();
    k = val.find('(');
    int size2t = val.length();
    string s2t = val.substr(k+1, size2t);
    int size2 = s2t.length() -1;
    string s2 = s2t.substr(0, size2);
    int comma1 = 0, comma2 =0;
    for (int i = 0; i < size1; ++i) {
        if(s1[i]== ',')
            comma1++;
    }
    for (int i = 0; i < size2; ++i) {
        if(s2[i]== ',')
            comma2++;
    }
    if(comma1 != comma2) return "";
    while(comma1 >= 0){
        int k1 = s1.find(',');
        int k2 = s2.find(',');
        string s1_type = s1.substr(0, k1);
        string s2_type = s2.substr(0, k2);
        if(s1_type == "string"){
            if(!constString(s2_type))
                return "";
        }
        else if(s1_type == "number"){
            if(!constNumber(s2_type))
                return "";
        }
        s1 = s1.substr(k1+1, s1.length());
        s2 = s2.substr(k2+1, s2.length());
        comma1--;
    }
    return string_out;
}
bool assign(Tree* tree, const string& data, const string& value){
    string typedata = Type(tree, data);
    string typeval = Type(tree,value);
    Node* temp;
    if(tree->find((string &) data) != 0){
        temp = tree->Find((string &)data);
    }
    bool kt = false;
    if(typeval == typedata) kt = true;
    if(typedata == Typeval(typeval,value)) kt = true;
    if(kt){
        tree->splay(temp->data, temp->type, temp->stic, temp->level);
        return kt;
    }
    return false;
}
class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
    ~SymbolTable(){}
};
#endif