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
    Def: id '(' Pars ')' '{' Stats '}'
       ;

    // Function parameters (0-n)
    Pars: [ { id ',' } id ]
        ;

    // Statements (0-n)
    Stats: { Stat ';' }
         ;

    Stat: return Expr
        | if Expr '{' Stats '}'
        | while Expr '{' Stats '}'
        | let id '=' Expr
        | Lexpr '=' Expr
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
