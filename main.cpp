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

    std::string program = ".*";
    std::string input = "Waterloooo Io was defeated, you won the war Waterloo promise to"
                        " love you for ever more Waterloo couldn't escape if I wanted"
                        " to Waterloo knowing was my fate is I to be with you Waterloo finally"
                        " facing my Waterloo ";

    lexer lexer(program.begin(),program.end());
    auto tree = match(program.begin(),program.end(),lexer);

    it first = input.begin();
    it last = input.end();
    it ptr  = first;

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
