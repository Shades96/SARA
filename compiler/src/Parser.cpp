#include "Parser.h"

#include "Terminal.h"
#include "Output.h"

#include <iostream>

using std::cout;

int Parser::parse(Scanner::Token& token)
{
    NonTerminal params {
        Optional { vector<NonTerminal> {
            Repeat { vector<NonTerminal> {
                Terminal { Scanner::Token::IDENTIFIER }, // id
                Terminal { Scanner::Token::COMMA }  // ','
            }},
            Terminal { Scanner::Token::IDENTIFIER } // id
        }}
    };
    NonTerminal function{ vector<NonTerminal> {
        Terminal { Scanner::Token::IDENTIFIER },        // id
        Terminal { Scanner::Token::PARENTHESIS_OPEN },  // '('
        params,
        Terminal { Scanner::Token::PARENTHESIS_CLOSE }, // ')'
        NonTerminal { vector<NonTerminal> {} },         // Block
    }};

    Output::log() << Scanner::Token::KIND_NAMES[token.kind] << "\n";
    return EXIT_SUCCESS;
}