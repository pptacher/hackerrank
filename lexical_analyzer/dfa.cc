#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stack>
#include <set>
#include <queue>
#include <unordered_map>
#include <bitset>

typedef unsigned int uint;
using std::string;

#define MSB(X) ((unsigned) (8*sizeof (unsigned long) - __builtin_clzll((X)) -1))

enum ope: uint8_t
{
    ou,
    cat,
    star,
    leaf

};

// TO DO: used derived classes.
class Node {
public:
    Node(ope o_, int pos_,char input_=' ',
        bool nullable_=false,
        Node* left=nullptr,
        Node* right=nullptr)
            : o(o_), input(input_),position(pos_),nullable(nullable_){
        operand[0] = left;
        operand[1] = right;

        if(o==leaf){
            return;
        }

        switch (o) {
            case ou:
                nullable = left->nullable || right->nullable;
                firstpos = left->firstpos;
                firstpos.insert(right->firstpos.begin(),right->firstpos.end());
                lastpos = left->lastpos;
                lastpos.insert(right->lastpos.begin(),right->lastpos.end());
                break;
            case cat:
                nullable = left->nullable && right->nullable;
                firstpos = left->firstpos;
                if(left->nullable){
                    firstpos.insert(right->firstpos.begin(),right->firstpos.end());
                }
                lastpos = right->lastpos;
                if(right->nullable){
                    lastpos.insert(left->lastpos.begin(),left->lastpos.end());
                }
                break;
            case star:
                nullable = true;
                firstpos = left->firstpos;
                lastpos = left->lastpos;
                break;
            default:
                break;

        }
    }

    ope o;
    char input;
    int position;
    bool nullable;
    std::set<uint> firstpos;
    std::set<uint> lastpos;
    Node* operand[2];

};

Node* syntax_tree(string,int&,std::vector<int>&);
void inorder(Node*);
void followpos(Node* root, std::vector<std::set<int>>& s);
std::vector<std::vector<long int>> transition_matrix(Node* root, std::vector<std::set<int>>& v, std::vector<int>& inputs,std::set<int>&);
void matrixpow(std::vector<std::vector<long int>>& matrix, uint n,std::vector<std::vector<long int>>&);


int main(){
    Node *root(nullptr);
    int pos(0);
    std::vector<int> inputs;
    //syntax_tree("((ab)|(ba)");
    //root = syntax_tree("((a|b)*)",pos);
    //root = syntax_tree("((a*)(b(a*)))",pos);
    //root = syntax_tree("(a*)b(a*)",pos);
    //root = syntax_tree("(a|b)*abb#",pos,inputs);
    root = syntax_tree("((((b*)((b(a|a))a))(((a*)|a)a))((aa)((((((b*)*)(b|((b|(aa))|b)))(a|b))|(b*))*)))#",pos,inputs);
    inorder(root);
    std::vector<std::set<int>> v(pos);

    followpos(root,v);
    std::cout << "follow: " << std::endl;
    for(auto const& u: v){
        for(int j: u){
            std::cout << j << ' ';
        }
        std::cout << std::endl;
    }
    std::set<int> finals;

    std::vector<std::vector<long int>> p = transition_matrix(root,v,inputs,finals);

    int n = p.size();
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            std::cout << p[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    uint l=937477085;
    std::vector<std::vector<long int>> pn(n,std::vector<long int>(n,0));
    for(int i=0; i<n; ++i){
        pn[i][i] = 1;
    }
    matrixpow(p,l,pn);

    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            std::cout << pn[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    long int res(0);
    const int modulo = 1000000007;

    for(int j: finals){
        std::cout << j << ' ';
        res = (res + pn[0][j])%modulo;
    }
    std::cout << res << std::endl;

    return 1;
}

void matrixmul(std::vector<std::vector<long int>>& matrix,std::vector<std::vector<long int>>& p){
    int n = matrix.size();
    const int modulo = 1000000007;
    std::vector<std::vector<long int>> res(n,std::vector<long int>(n,0));

    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            for(int k=0; k<n; ++k){
                res[i][j] = (res[i][j] + (matrix[i][k]%modulo)*(p[k][j]%modulo))%modulo;
            }
        }
    }
    for(int i=0; i<n; ++i){
        for(int j=0; j<n; ++j){
            matrix[i][j] = res[i][j];
        }
    }
}


void matrixpow(std::vector<std::vector<long int>>& matrix, uint n,std::vector<std::vector<long int>>& pn){

    while(n>0){
        if(n%2==0){
            matrixmul(matrix,matrix);
            n /= 2;
        }
        else{
            matrixmul(pn,matrix);
            --n;
        }
    }

}

std::vector<std::vector<long int>> transition_matrix(Node* root, std::vector<std::set<int>>& v, std::vector<int>& inputs, std::set<int>& finals){
    std::unordered_map<uint64_t,uint> hm;
    std::vector<std::vector<int>> adjacency(1);
    uint64_t ss(0);
    for(int j: root->firstpos){
        ss |= (1<<j);
    }
    int count(0);
    hm.insert({ss,count++});

    std::queue<uint64_t> q;
    q.push(ss);
    while(!q.empty()){
        ss = q.front();
        auto it1 = hm.find(ss);
        std::bitset<8> bs(ss);
        std::cout << bs << std::endl;
        q.pop();
        uint64_t next[2] = {0,0};
        while(ss>0){
            char msb(MSB(ss));
            ss ^= (1<<msb);
            uint x;
            if(inputs[msb]=='#'){
                finals.insert(it1->second);
                x = 2;
                continue;
            }
            else{
                x = inputs[msb]-'a';
            }
            for(int j: v[msb]){
                next[x] |= (1<<j);
            }
        }
        for(int i=0; i<2; ++i){
            if(next[i]==0){
                continue;
            }
            auto it = hm.find(next[i]);
            if(it==hm.end()){
                hm.insert({next[i],count});
                adjacency.push_back({});
                q.push(next[i]);
                adjacency[it1->second].push_back(count++);
            }
            else{
                adjacency[it1->second].push_back(it->second);
            }
        }
    }

    std::vector<std::vector<long int>> trans(adjacency.size(),std::vector<long int>(adjacency.size(),0));

    for(uint i=0; i<adjacency.size(); ++i){
        for(int j: adjacency[i]){
            trans[i][j]++;
        }
    }

    return trans;

}

//TO DO: reduce backtracking.
Node* syntax_tree(string s,int& pos,std::vector<int>& inputs){
    int n = s.length();
    std::cout << s << std::endl;
    Node *p, *p1, *left;
    int i(0);
    ope nop(leaf);

    if(s[i]=='('){
        ++i;
    }

    std::queue<Node*> q;
    parse:
    while(i<n && s[i]!=')'){
            std::cout << s[i] << ' '<<i << std::endl;
        if(std::isalpha(s[i]) || s[i]=='#'){
            p1 = new Node(leaf,pos,s[i]);
            p1->firstpos.insert(pos);
            p1->lastpos.insert(pos++);
            inputs.push_back(s[i]);
            if(i+1<n && s[i+1]=='*'){
                Node* p2 = new Node(star,-1,' ',true,p1,nullptr);
                q.push(p2);
                ++i;
            }
            else{
                q.push(p1);
            }
        }
        else if (s[i]=='('){
            int st(1);
            size_t l = i;
            while(st>0){
                l = s.find_first_of("()",l+1);
                if(s[l]=='('){
                    ++st;
                }
                else{
                    --st;
                }
            }
            p1 = syntax_tree(s.substr(i,l-i+1),pos,inputs);
            i = l;

            if(i+1<n && s[i+1]=='*'){
                Node* p2 = new Node(star,-1,' ',true,p1,nullptr);
                q.push(p2);
                ++i;
            }
            else{
                q.push(p1);
            }
        }
        else if(s[i]=='|'){
            nop = ou;
            p1 = q.front();
            q.pop();
            while(!q.empty()){
                Node* p2 = new Node(cat,-1,' ',true,p1,q.front());
                q.pop();
                p1 = p2;
            }
            left = p1;
        }
        ++i;
    }

    p1 = q.front();
    q.pop();
    while(!q.empty()){
        Node* p2 = new Node(cat,-1,' ',true,p1,q.front());
        q.pop();
        p1 = p2;
    }
    if(nop==ou){
        p = new Node(ou,-1,' ',true,left,p1);
    }
    else{
        p = p1;
    }

    if(i+1<n && s[i+1]=='*'){
        Node* p2 = new Node(star,-1,' ',true,p,nullptr);
        ++i;
        p = p2;
    }
    if(i+++1<n){
        nop = leaf;
        q.push(p);
        goto parse;
    }

    return p;

}

void followpos(Node* root, std::vector<std::set<int>>& s){
    if(root==nullptr){
        return;
    }

    switch (root->o) {
        case cat:
            for(int j: root->operand[0]->lastpos){
                s[j].insert(root->operand[1]->firstpos.begin(),root->operand[1]->firstpos.end());
            }
            break;
        case star:
            for(int j: root->operand[0]->lastpos){
                s[j].insert(root->operand[0]->firstpos.begin(),root->operand[0]->firstpos.end());
            }
            break;
        default :
            break;
    }

    followpos(root->operand[0],s);
    followpos(root->operand[1],s);

}

void inorder(Node* root) {
        if (root ==nullptr){
            return;
        }

        Node* p = root;
        std::stack<Node*> parent;

        while(true){

            while(p->operand[0]){ //descend left, stacking nodes on the way down.
                parent.push(p);
                p=p->operand[0];
            }

            std::cout << p->o << ' ' <<p->input << ' ' << p->position << std::endl;
            for(uint j: p->firstpos){
                std::cout << j << ' ';
            }
            std::cout << std::endl;
            for(uint j: p->lastpos){
                std::cout << j << ' ';
            }
            std::cout << std::endl;

            while(!p->operand[1]){

                if(!parent.empty()){
                    p = parent.top();
                    parent.pop();
                    std::cout << p->o <<' ' << ' ' << p->position << std::endl;
                    for(uint j: p->firstpos){
                        std::cout << j << ' ';
                    }
                    std::cout << std::endl;
                    for(uint j: p->lastpos){
                        std::cout << j << ' ';
                    }
                    std::cout << std::endl;
                }
                else
                    return;
            }

            p=p->operand[1];

        }//while(true)

}
