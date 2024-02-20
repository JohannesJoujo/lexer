#include <iostream>
#include "lexer.h"
#include "expr.h"

/**
 * <program>:= <exp>
 * <expr>  := <group> | <or_op> |<multiple> | <word> | <counter> | <ignor> | <output>
 * <group> := [(<expr>)]
 * <or_op> := <word>+<word>
 * <word>  :=<char> [<word>] | <any_char> [<word>]
 * <multiple>:= <word>*
 * <dot> := '.'
 * <counter> := {<expr>}
 * <ignor> := '\I'
 * <output> := '<expr>\O{<int 0->9 >}'
 */

/**
 * <program> -> <expr>
 * <expr> -> <match> [<expr>]
 * <match> ->  <or> | <group> | <word>
 * <word> -> <repeat> | <char> <word> | <char>
 * <repeat> -> <char> * | <char> <count>
 * <or> -> <word> + <word>
 * <group> -> (<match>)
 * <char> -> <any> | A-Z | a-z | 0-9
 * <any> -> .
 * <count> -> {0...9}
 */

int main() {

    std::string program = "(heter*) knowin{3}";
    std::string input = "jag heterr knowinrrrrr";
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
