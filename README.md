# AM22
Abstrakte Maschinen Projekt SS-2022

-------------
Language Spec
-------------
-------------

    // Mod?

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
        | Block
        | Lexpr '=' Expr
        | id '(' { Expr ',' } Expr ')'
        ;

    Lexpr: id
         | id '[' Expr ']'
         ;

    Expr: Term
        | ( '!' | '-' ) Term
        | Term ( '+' | '-' | '*' | '/' | '&&' | '||' | '<' | '==' | '>' ) Term
        ;
    
    Term: num
        | id
        | id '[' Expr ']' 
        | id '(' { Expr ',' } Expr ')'
        | '(' Expr ')'
        ;

---------------
Instruction set
---------------
---------------

    // Arithmetic
    Neg  <op1>
    Add  <op1> <op2>
    Sub  <op1> <op2>
    Mul  <op1> <op2>
    Div  <op1> <op2>
    Mod  <op1> <op2>

    // Comparisons
    Gt   <op1> <op2>
    Lt   <op1> <op2>
    Eq   <op1> <op2>
    Neq  <op1> <op2>
    Geq  <op1> <op2>
    Leq  <op1> <op2>

    // Boolean
    Not  <op1>
    And  <op1> <op2>
    Or   <op1> <op2>

    // Stack
    Pop  <op1>
    Push <op1>

    // Control
    Exit
    Call <op1>
    Ret  <op1>
    Jmp  <op1> <op2>

---------------
