//
// Created by Johannes Joujo on 2024-02-04.
//

#ifndef LEXER_EXPR_H
#define LEXER_EXPR_H
#include <iostream>
#include <vector>
#include "lexer.h"
struct op{
    virtual bool eval(it first, it last, it& ptr) = 0;
    void add(op* child){
        if(child)
            children.push_back(child);
    }
    std::vector<op*> children;
};
struct char_op:op{
    char ch;
    char_op(char c):ch(c){ }

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

    bool eval(it first, it last,it& ptr) override{
        if(first == last){
            return false;
        }
        return true;
    }
};

struct word : op{
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

        ord+=*first;
        last = ptr +(N+1); // uppdate the last pointer
        //std::cout<<*last;
        if(n < 1 && *s=='.'){
            n++;
            while (first != last){
                ord+=*first;
                first++;
            }
            std::cout<<ord;

        }else if(n < 1){
            first=ptr;

            n++;
            while (first != last){
                if(*first==*ptr) {
                    ord+=*ptr;
                    std::cout << *ptr<<'\n';
                    ++first;
                }else{
                    return false;
                }
            }
            std::cout<<ord;

        }

        return result;
    }
};

struct group_op:op{ // wrong implementation
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

struct expr_op:op{ //klar
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
    int counter=0;
    bool eval(it first, it last,it& ptr) override{
        static int n = 0; // using static in order to preserve our current value when call back
        static int j = 0; // using static in order to preserve our current value when call back
        static int h = 0; // using static in order to preserve our current value when call back
        static int g = 0; // using static in order to preserve our current value when call back
        static std::string ord;


        if(first == last){
            return false;
        }
        ptr=ptr-1;
        if(*--ptr=='.'){
            g=1;
        }
        auto result = children[0]->eval(first, last,ptr);
        auto value=children[0]->children;

        if(!result){
            eval(++first, last,ptr);
        }

        //last = ptr +(N +1); // uppdate the last pointer
        while (first != ptr && first!=last){
            ord+=*first;
            first++;
        }
        if(j<1) {
            ++j;
            //if (value[value.size() == '.'] ) {
            if (g==1) {
                while (first != last) {
                    ord += *first;
                    first++;
                }
                h++;
                std::cout << ord << '\n';
            }
            //return true;
        }
        //first = ptr;
        if(n<1){
            n++;
            while (first != last){
                if(*first == *ptr){
                    ord+=*first;
                    first++;
                }else{
                    break;
                }
            }
            if(h<1){
                std::cout<<ord<<'\n';
                return true;
            }
            //std::cout<<ord<<'\n';
        }
        return result;
    }
};

struct or_op:op{    //klar
    bool eval(it first, it last,it& ptr) override{
        auto result = children[0]->eval(first, last,ptr);

        if(result){
            return true;
        }

        return children[1]->eval(first, last,ptr);
    }
};

struct match_op:op{
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




#endif //LEXER_EXPR_H
