# AM22
Abstrakte Maschinen Projekt SS-2022

-----------
-----------
## Compiler
-----------

### Language Spec
-----------------

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
        | Term ( '+' | '-' | '*' | '/' | '%' | '&&' | '||' | '<' | '==' | '>' ) Term
        ;
    
    Term: num
        | id
        | id '[' Expr ']' 
        | id '(' { Expr ',' } Expr ')'
        | '(' Expr ')'
        ;

---------------------------
### Built-ins/Special functions
---------------------------

- print(): Prints the result of an expression to the console.
- read(): Reads input from the console and return it.
- exit(): Terminates the program.
- main(): Program entry point - must be defined.

----------
----------
## Runtime
----------

- Function parameters and locals reside on the stack
- Instruction operands index the stack

When a function is called, save a function stack frame ptr (fsfp) and save parameters on the stack.
- Within a function block, all references are relative to the fsfp.
- On return, invalidate everything past the sfp.

When a regular block is entered, save a block stack frame ptr (bsfp).
- Within a regular block, all references are STILL relative to the fsfp.
- On leaving the block, invalidate everything past the bsfp.

-------------------
### Instruction set
-------------------

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

-------------------
