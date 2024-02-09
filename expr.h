//
// Created by Johannes Joujo on 2024-02-04.
//

#ifndef LEXER_EXPR_H
#define LEXER_EXPR_H
#include <iostream>
#include <vector>
#include "lexer.h"

struct op{
    virtual bool eval(it first, it last) = 0;
    void add(op* child){
        if(child)
            children.push_back(child);
    }
    std::vector<op*> children;
};

struct char_op:op{
    char ch;
    char_op(char c):ch(c){ }

    /*    std::string program = "I + war";
     *    std::string input = "war W";
     *
     * */

    bool eval(it first, it last) override{

        if(*first == ch || *first == '.'){
            return true;
        } else{
            return false;
        }

    }


};

struct any_op : char_op{
    bool eval(it first, it last) override{
        if(first == last){
            return false;
        }
        first++;
        return true;
    }

};

struct word : op{
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);
        if(children.size() > 1){
            return result && children[1]->eval(first + 1, last);
        }
        return result;
    }
};

struct group_op:op{ // wrong implementation
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        if(result){
            return true;
        }

        return false;
    }
};

struct expr_op:op{ //
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        if(result){
            return true;
        }

        return false;
    }
};

struct or_op:op{
    bool eval(it first, it last) override{
        auto result = children[0]->eval(first, last);


        if(result){
            return true;
        }

        return children[1]->eval(first, last);
    }
};

struct match_op:op{
    bool eval(it first, it last) override{
        if(first == last)
            return false;
        auto result = children[0]->eval(first, last);
        if(!result){
            return eval(first + 1, last);
        }
        return true;
    }
};


match_op* match(it first, it last, lexer lexer);
or_op* orOp(it first, it last,lexer lexer);
word* paserWord(it& first, it last,lexer lexer);




#endif //LEXER_EXPR_H
