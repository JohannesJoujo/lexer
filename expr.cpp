//
// Created by Johannes Joujo on 2024-02-04.
//
#include "expr.h"
multi* multiParser(it &first, it last,lexer lexer){ //klar
    auto restore=first;
    auto  word = charOp(first,last,lexer);
    if(!word){
        return nullptr;
    }
    first++;
    auto token = lexer.lex(first,last);
    if(token == lexer::MULT_OP){
        multi* value = new multi;
        value->add(word);
        value->add(parse_expr(++first,last,lexer));
        return value;
    }
    first=restore;
    return nullptr;
}
anyChar* any_char(it first, it last,lexer lexer){ // klar
    auto value = lexer.lex(first,last);
    if(value == lexer::DOT){
        return new anyChar;
    }
    return nullptr;

}
or_op* orOp(it first, it last,lexer lexer){ //klar
    auto lhs = pase_word(first,last,lexer);
    if(lhs){
        auto  value = lexer.lex(first,last);

        if(value == lexer::ADD_OP){
            or_op* OR = new or_op;
            OR->add(lhs);

            first++;
            word* rhs = pase_word(first,last,lexer);

            if(rhs){
                OR->add(rhs);
            } else{
                return nullptr;
            }
            return OR;
        }
    }

    return nullptr;
}
char_op* charOp(it first, it last,lexer lexer){ //klar
    auto  value = lexer.lex(first,last);
    auto result = any_char(first,last,lexer);
    if(result){
        anyChar* ch = new anyChar;
        ch->add(result);
        return ch;
    }

    if(value == lexer::LETTER || value == lexer::DIGIT || value == lexer::SPACE){
        return new char_op(*first); // gets the value of the pointer *first
    }

    return nullptr;
}
subexpr* parserSubexpr(it& first, it last,lexer lexer){ // klar

    auto multSymbol = multiParser(first,last,lexer);
    if(multSymbol){
        auto* result = new subexpr;
        result->add(multSymbol);
        return result;
    }

    auto countSymbol = count(first,last,lexer);
    if(countSymbol){
        subexpr* result = new subexpr;
        result->add(countSymbol);
        return result;
    }

    return nullptr;
}
word* pase_word(it& first, it last,lexer lexer){ //klar
    auto extraSymbol = parserSubexpr(first,last,lexer);
    if(extraSymbol){
        word* results = new word;
        results->add(extraSymbol);
        return results;
    }

    auto ch = charOp(first,last,lexer);
    if(ch){
        word* results = new word;
        results->add(ch);
        first++;
        results->add(pase_word(first, last,lexer));
        return results;

    }

    return nullptr;
}
group_op* parse_group(it& first, it last,lexer lexer){
    auto restor = first;
    auto value = lexer.lex(first, last);
    bool space = false;
    while (value == lexer::SPACE){
        ++first;
        value = lexer.lex(first, last);
        space = true;
    }
    if(space)
        value = lexer.lex(++first, last);

    if(value == lexer::LEFT_PAREN){
        ++first;
        auto text_node = parse_expr(first, last,lexer);
        if(!text_node)
            return nullptr;

        while (*first != ')'){

            if(first == last) break;

            first++;
        }
        value = lexer.lex(first, last);
        if(value != lexer::RIGHT_PAREN) {
            return nullptr;
        }

        auto group_node = new group_op;
        group_node->add(text_node);
        group_node->add(parse_expr(++first, last,lexer));
        return group_node;
    }
    first = restor;
    return nullptr;
}

counter* count(it& first, it last,lexer lexer){
    auto restore = first;
    auto  word = charOp(first,last,lexer);
    if(!word){
        return nullptr;
    }
    first++;
    auto value = lexer.lex(first,last);
    if(value == lexer::OPEN_BRES){
        ++first;

        value = lexer.lex(first,last);
        if(value == lexer::DIGIT){
            char_op* myChar = new char_op(*restore);
            int x = *first - '0';
            counter* counter1 = new counter(x);
            counter1->add(myChar);
            value = lexer.lex(++first,last);
            if(value == lexer::CLOSEING_BRES){
                counter1->add(parse_expr(++first,last,lexer));
                return counter1;
            }
        }

    }

    first = restore;
    return nullptr;
}

expr_op* parse_expr(it& first, it last,lexer lexer){

    auto group_op = parse_group(first, last,lexer);
    if(group_op){
        auto expr_node = new expr_op;
        expr_node->add(group_op);
        return expr_node;
    }

    auto or_OP = orOp(first, last, lexer);
    if(or_OP){
        auto expr_node = new expr_op;
        expr_node->add(or_OP);
        return expr_node;

    }


    auto text_node = pase_word(first, last, lexer);
    if(text_node){
        auto expr_node = new expr_op;
        expr_node->add(text_node);
        expr_node->add(parse_expr(first, last,lexer));
        return expr_node;
    }

    return nullptr;
}

match_op* match(it first, it last, lexer lexer){

    auto  expr_node = parse_expr(first,last,lexer);
    if(expr_node){
        auto result = new match_op;
        result->add(expr_node);
        return result;


    }

    return nullptr;

}
