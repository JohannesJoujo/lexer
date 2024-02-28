#include <iostream>
#include "lexer.h"
#include "expr.h"


/**
 *  match    := expr
    expr     := group | or_op  | word
    group    := (expr)[<expr>]
    word     := subexpr | char[word] | char
    subexpr  := char* | char<counter>
    char     := any_char | a --> z | A --> Z | 0->9
    counter  := {digit} [<expr>]
    any_char := .
    or_op    := word + word
 */

int main() {

    std::string program = "b+aa*";
    std::string input = "Haaab";
    lexer lexer(program.begin(),program.end());
    auto tree = match(program.begin(),program.end(),lexer);
    it first = input.begin();
    it last = input.end();
    if(tree){
        auto match = tree->eval(first,last);
        if(match){
            std::cout<<" :FOUND A MATCH \n\n";
        } else{
            std::cout<<" :NO MATCH FOUND \n";
        }
    } else std::cout<<"Wrong input";

    tree->print();



    return 0;
}
