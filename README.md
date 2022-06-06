# AM22
Abstrakte Maschinen Projekt SS-2022

-------------
Language Spec
-------------
-------------

    // structs/objects?

    // Program
    Program: { Def ';' }
           ;

    // Function definition
    Def: id '(' Pars ')' Block
       ;

    // Function parameters (0-n)
    Pars: [ { id ',' } id ]
        ;

    // Statements (0-n)
    Block: '{' { Stat ';' } '}'
         ;

    Stat: return Expr
        | if Expr Block
        | while Expr Block
        | let id '=' Expr
        | Lexpr '=' Expr
        | Block
        | Term
        ;

    Lexpr: id
         | id '[' Expr ']'
         ;

    Expr: { '!' | '-' } Term
        | Term { ( '+' | '-' ) Term }
        | Term { ( '*' | '/' ) Term }
        | Term { ('&&' | '||') Term }
        | Term ( '<' | '==' | '>' ) Term
        ;
    
    Term: '(' Expr ')'
        | num
        | id
        | id '[' Expr ']' 
        | id '(' { Expr ',' } Expr ')'
        ;

-------------
