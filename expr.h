//
// Created by Johannes Joujo on 2024-02-04.
//

#ifndef LEXER_EXPR_H
#define LEXER_EXPR_H
#include <iostream>
#include <iomanip>
#include <vector>
#include "lexer.h"
struct op{ //klar
    virtual bool eval(it& first, it last) = 0;
    void add(op* child){
        if(child)
            children.push_back(child);
    }
    std::vector<op*> children;

    virtual std::string id()=0;
    void print(int indent=0) {
        std::cout << std::setw(indent) << " " << id() << "\n";
        for (auto child: children) {
            child->print(indent + 4);
        }
    }
};
struct word : op{ // klar
    bool eval(it& first, it last) override{
        auto result = children[0]->eval(first, last);
        if(children.size() > 1){
            return result && children[1]->eval(first, last);
        }
        return result;
    }
    std::string id() override{
        return "word";
    }

};
struct char_op:op{ //klar
    char ch;
    char_op(char c):ch(c){ }
    bool eval(it& first, it last) override{

        if(*first == ch || ch == '.'){
            std::cout<<*first;
            first++;
            return true;
        } else{
            first++;
            return false;
        }

    }
    std::string id() override{
        std::cout<<"char_op => ";
        std::cout<<ch;
        return  " ";
    }

};
struct anyChar :char_op{ //klar
    anyChar(): char_op('.'){}
    bool eval(it& first, it last) override{
        std::cout<<*first;
        return true;
    }
    std::string id() override{
        return "anyChar";
    }
};
struct multi: op{   //klar
    bool status = false;
    bool eval(it& first, it last) override{
        auto temp=first;

        while ( children[0]->eval(first,last)){
            if(first == last){ return true;}
            status = true;
        }
        if(children.size()>1) {
            while (!children[1]->eval(temp, last)) {} // works well
        }
        if(status){
            return true;
        }
        return false;
    }
    std::string id() override{
        return "multi";
    }

};
struct expr_op:op{ //   klar
    bool eval(it& first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        if(result){
            return true;
        }
        return false;
    }
    std::string id() override{
        return "expr_op";
    }
};
struct subexpr:op{ //klar

    bool eval(it& first, it last) override{
        bool result=children[0]->eval(first,last);
        if (result) return true;
        return false;
    }
    std::string id() override{
        return "subexpr";
    }

};
struct group_op:op{ // klar
    bool eval(it& first, it last) override{
        auto temp = first;
        if(first == last) {return false;}

        while(!children[0]->eval(first, last)){}
        if(children.size()>1) {
            while (!children[1]->eval(temp, last)) {}
        }
        return true;
    }
    std::string id() override{
        return "group_op";
    }
};
struct counter: op{ //inte klar
    int N = 0;
    std::string myString;
    bool result= false;
    counter(int c):N(c){}
    bool eval(it& first, it last) override {
        auto temp_last = first + N;   // update the position of the last pointer
        while (children[0]->eval(first, temp_last)){
            result= true;
            if (first == last) {break;}
        }
        return result;
    }
    std::string id() override{
        return "counter";
    }
};
struct or_op:op{
    bool eval(it& first, it last) override{
        auto temp=first;
        auto result = children[0]->eval(first, last);
        if(result){
            return true;
        }
        return children[1]->eval(temp, last);
    }
    std::string id() override{
        return "or_op";
    }
};
struct match_op:op{
    bool eval(it& first, it last) override{
        if(first == last) {
            return false;
        }
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first, last);
        }
        return true;
    }
    std::string id() override{
        return "match_op";
    }
};

multi* multiParser(it& first, it last,lexer lexer);
expr_op* parse_expr(it& first, it last,lexer lexer);
match_op* match(it first, it last, lexer lexer);
or_op* orOp(it first, it last,lexer lexer);
word* pase_word(it& first, it last,lexer lexer);
char_op* charOp(it first, it last,lexer lexer);
counter* count(it& first, it last,lexer lexer);
#endif //LEXER_EXPR_H
