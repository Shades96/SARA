#include "Parser.h"

#include "Output.h"

#include <iostream>

using std::cout;

int Parser::parse(Terminal& token)
{
    //NonTerminal params {
    //    Optional { vector<NonTerminal> {
    //        Repeat { vector<NonTerminal> {
    //            Terminal { Terminal::IDENTIFIER }, // id
    //            Terminal { Terminal::COMMA }  // ','
    //        }},
    //        Terminal { Terminal::IDENTIFIER } // id
    //    }}
    //};
    //NonTerminal function{ vector<NonTerminal> {
    //    Terminal { Terminal::IDENTIFIER },        // id
    //    Terminal { Terminal::PARENTHESIS_OPEN },  // '('
    //    params,
    //    Terminal { Terminal::PARENTHESIS_CLOSE }, // ')'
    //    NonTerminal { vector<NonTerminal> {} },         // Block
    //}};

    return program.parse(token);
}