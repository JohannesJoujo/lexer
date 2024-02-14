//
// Created by Johannes Joujo on 2024-02-04.
//
#include "expr.h"

multi* multiParser(it first, it last,lexer lexer){
    auto  word = paserWord(first,last,lexer);

    if(!word){
        return nullptr;
    }
    auto token = lexer.lex(first,last);

    if(token == lexer::MULT_OP){
        multi* value = new multi;
        value->add(word);
        return value;
    }

    return nullptr;
}
counter* count(it first, it last,lexer lexer){
    auto myWord = paserWord(first,last,lexer);
    if(!myWord){
        return nullptr;
    }

    auto value = lexer.lex(first,last);
    if(value == lexer::OPEN_BRES){
        ++first;

        value = lexer.lex(first,last);
        if(value == lexer::DIGIT){
            int x = *first - '0';
            counter* counter1 = new counter(x);

            value = lexer.lex(++first,last);
            if(value == lexer::CLOSEING_BRES){
                counter1->add(myWord);
                return counter1;
            }
        }

    }

    return nullptr;
}

or_op* orOp(it first, it last,lexer lexer){
    auto lhs = paserWord(first,last,lexer);
    if(lhs){
        auto  value = lexer.lex(first,last);

        if(value == lexer::ADD_OP){
            or_op* OR = new or_op;
            OR->add(lhs);

            first++;
            word* rhs = paserWord(first,last,lexer);

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

char_op* charOp(it first, it last,lexer lexer){
    auto  value = lexer.lex(first,last);

    if(value == lexer::END){
        return nullptr;
    }

    if(value==lexer::DOT){
        return new any_op(*first);
    }

    if(value == lexer::LETTER || value == lexer::DIGIT || value == lexer::SPACE){
        return new char_op(*first); // gets the value of the pointer *first
    }

    return nullptr;
}

word* paserWord(it& first, it last,lexer lexer){

    auto ch = charOp(first,last,lexer);
    if(ch){
        word* results = new word;
        results->add(ch);
        ++first;
        results->add(paserWord(first, last,lexer));
        return results;
    }
    return nullptr;
}
group_op* parse_group(it& first, it last,lexer lexer){
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
        auto text_node = parse_group(first, last,lexer);
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
        return group_node;
    }

    //--first;
    return nullptr;
}

expr_op* parse_expr(it& first, it last,lexer lexer){

    auto group_op = parse_group(first, last,lexer);
    if(group_op){
        auto expr_node = new expr_op;
        expr_node->add(group_op);
        expr_node->add(parse_expr(first, last,lexer));
        return expr_node;
    }

    auto or_OP = orOp(first, last, lexer);
    if(or_OP){
        auto expr_node = new expr_op;
        expr_node->add(or_OP);
        return expr_node;
    }

    auto counting = count(first,last,lexer);
    if(counting){
        auto expr_node = new expr_op;
        expr_node->add(counting);
        return expr_node;
    }

    auto multiSymbol = multiParser(first,last,lexer);
    if(multiSymbol){
        auto expr_node = new expr_op;
        expr_node->add(multiSymbol);
        return expr_node;
    }

    auto text_node = paserWord(first, last, lexer);
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