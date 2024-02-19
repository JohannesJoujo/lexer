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

    std::string program = "Io*";
    std::string input = "Waterlooooooo Ioo was defeated, you won the war Waterloo promise to"
                        " love you for ever more Waterloo couldn't escape if I wanted"
                        " to Waterloo knowing was my fate is I to be with you Waterloo finally"
                        " facing my Waterlooi ";

    lexer lexer(program.begin(),program.end());
    auto tree = match(program.begin(),program.end(),lexer);
    tree->print();
    it first = input.begin();
    it last = input.end();
    it ptr  = program.end();

    if(tree){
        auto match = tree->eval(first,last,ptr);
        if(match){
            std::cout<<" :FOUND A MATCH";
        } else{
            std::cout<<"NO MATCH FOUND";
        }
    } else std::cout<<"Wrong input";



    return 0;
}
