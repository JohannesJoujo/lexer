//
// Created by Johannes Joujo on 2024-02-04.
//

#include "lexer.h"

char lexer::get_char(it& first) {
    return *++first;
}

lexer::lexer(it first, it last){
    while(first != last){
        input += *first;
        ++first;
    }
}

token lexer::lex(it& first, it& last) {

    if(first == last){
        return END;
    }

    switch (*first) {
        case '+':
            current_lexeme = *first;
            return ADD_OP;
        case '*':
            current_lexeme = *first;
            return MULT_OP;
        case '(':
            current_lexeme = *first;
            return LEFT_PAREN;
        case ')':
            current_lexeme = *first;
            return RIGHT_PAREN;
        case '{':
            current_lexeme = *first;
            return OPEN_BRES;
        case '}':
            current_lexeme = *first;
            return CLOSEING_BRES;
        case '.':
            current_lexeme = *first;
            return DOT;
        case ' ':
            current_lexeme = *first;
            return SPACE;
        case '\\':
            current_lexeme = *first;
            return BACK_SLASH;

        default:
            if(isalpha(*first)){
                return LETTER;
            }
            if(isdigit(*first)){
                return DIGIT;
            }
    }

    return UNKOWN;
}

char lexer::lexeme() {
    return current_lexeme;
}

void lexer::setCurrentLexeme(char currentLexeme) {
    current_lexeme = currentLexeme;
}