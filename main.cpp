#include <iostream>
#include "lexer.h"
#include "expr.h"

/**
 * <program>:= <exp>
 * <expr>  := <group> | <or_op> |<multiple> | <dot> | <counter> | <ignor> | <output>
 * <group> := [(<expr>)]
 * <or_op> := <word>+<word>
 * <word>  :=<char> [<word>]
 * <multiple>:= <word>*
 * <dot> := '.'
 * <counter> := {<expr>}
 * <ignor> := '\I'
 * <output> := '<expr>\O{<int 0->9 >}'
 */

int main() {

    std::string program = "I + loo";
    std::string input = "Waterloo W";
    lexer lexer(program.begin(),program.end());
    auto tree = match(program.begin(),program.end(),lexer);

   if(tree){
       auto match = tree->eval(input.begin(),input.end());
       if(match){
           std::cout<<"FOUND A MATCH";
       } else{
           std::cout<<"NO MATCH FOUND";
       }
   } else std::cout<<"Wrong input";



    return 0;
}
