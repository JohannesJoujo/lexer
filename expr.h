//
// Created by Johannes Joujo on 2024-02-04.
//

#ifndef LEXER_EXPR_H
#define LEXER_EXPR_H
#include <iostream>
#include <iomanip>
#include <vector>
#include "lexer.h"
struct op{
    virtual bool eval(it first, it last, it& ptr) = 0;
    void add(op* child){
        if(child)
            children.push_back(child);
    }
    std::vector<op*> children;

    virtual std::string id()=0;
    void print(int indent=0){
        std::cout<<std::setw(indent)<<""<<id()<<'\n';
        for(auto child: children){
            child->print(indent+4);
        }
    }
};
struct char_op:op{
    char ch;
    char_op(char c):ch(c){ }

    std::string id() override{
        return "char_op";
    }
    bool eval(it first, it last,it& ptr) override{

        if(*first == ch){
            return true;

        } else{
            return false;
        }
    }
};

struct any_op : char_op{ //potentiellt klar med den

    any_op(char c) : char_op(c) {} // Constructor accepting a char argument
    std::string id() override{
        return "any_op";
    }
    bool eval(it first, it last,it& ptr) override{
        if(first == last){
            return false;
        }
        return true;
    }
};

struct word : op{
    std::string id() override{
        return "word";
    }
    bool eval(it first, it last,it& ptr) override{
        static int n = 0; // using static in order to preserve our current value when call back
        static std::string ord;

        auto result = children[0]->eval(first, last,ptr);

        if(first != last){
            ptr = first;
        }
        if(result){
            ord+=*first;
        }

        if(children.size() > 1){
            return result && children[1]->eval(++first, last,ptr);
        }

        if(n<1){
            n++;
            //std::cout<<ord<<'\n'; // Måste fixa den hära delen -------------------------------------------------
        }
        return result;
    }
};

struct counter: op{
    int N = 0;
    counter(int c):N(c){}
    std::string id() override{
        return "counter";
    }
    bool eval(it first, it last,it& ptr) override{
        static int n = 0; // using static in order to preserve our current value when call back
        static it s= ptr-4;
        static std::string ord;
        auto result = children[0]->eval(first, last,ptr);
        if(first == last){
            return false;
        }
        if(!result){
            eval(++first, last,ptr);
        }


        last = ptr +(N); // uppdate the last pointer
        //std::cout<<*last;
        if(n < 1 && *s=='.'){
            n++;
            ord+=*first;
            first=first++;
            while (first != last){
                ord+=*first;
                first++;
            }
            std::cout<<ord;

        }else if(n < 1){
            n++;
            while(first!=ptr) {
                ord += *first;
                first++;
            }
            //first=ptr;
            while (first != last){
                if(*first==*ptr) {
                    ord+=*ptr;
                    std::cout << *ptr<<'\n';
                    ++first;
                }else{
                    std::cout<<"problemo\n";
                    ord="";
                    eval(++first, last,ptr);
                    //return false;
                }
            }
            std::cout<<ord;

        }

        return result;
    }
};

struct group_op:op{ // wrong implementation
    std::string id() override{
        return "group_op";
    }
    bool eval(it first, it last,it& ptr) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last,ptr);
        return result;
    }
};

struct expr_op:op{ //klar
    std::string id() override{
        return "expr_op";
    }
    bool eval(it first, it last,it& ptr) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last,ptr);
        if(result){
            return true;
        }

        return false;
    }
};

struct multi: op{   //nästan klar
    std::string id() override{
        return "multi";
    }
    bool eval(it first, it last,it& ptr) override{
        if(first==last){
            return false;
        }
        auto result = children[0]->eval(first, last,ptr);
        if(!result){
            first++;
            children[0]->eval(first,last,ptr);
        }
        while(first!=last ){
            eval(first,last,ptr);
            std::cout<<*first;
            first++;
        }
    }
};

struct or_op:op{    //klar
    std::string id() override{
        return "or_op";
    }
    bool eval(it first, it last,it& ptr) override{
        auto result = children[0]->eval(first, last,ptr);

        if(result){
            return true;
        }

        return children[1]->eval(first, last,ptr);
    }
};

struct match_op:op{
    std::string id() override{
        return "match_op";
    }
    bool eval(it first, it last,it& ptr) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last,ptr);
        if(!result){
            return eval(first + 1, last,ptr);
        }
        return true;
    }
};


match_op* match(it first, it last, lexer lexer);
or_op* orOp(it first, it last,lexer lexer);
word* paserWord(it& first, it last,lexer lexer);
expr_op* parse_expr(it& first, it last,lexer lexer);



#endif //LEXER_EXPR_H
