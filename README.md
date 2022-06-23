# **SARA** (**S**tack **A**utomata are **R**eally **A**wesome)

> A compiler for a toy programming language, that compiles to bytecode, together with a stack-based VM that runs it - all written in modern C++17.

This project was created for the course *Abstract Machines* at TU Vienna.

-----------
-----------
## Compiler
-----------

### Language Spec (EBNF)
-----------------

    Program: { Def ';' }
           ;

    Def: id '(' Pars ')' Block
       ;

    Pars: [ { id ',' } id ]
        ;

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
         ;

    Expr: Term
        | ( '!' | '-' ) Term
        | Term ( '+' | '-' | '*' | '/' | '%' | '&&' | '||' | '<' | '==' | '>' ) Term
        ;
    
    Term: num
        | id
        | id '(' { Expr ',' } Expr ')'
        | '(' Expr ')'
        ;

---------------------------
### Built-ins/Special functions
---------------------------

- print(): Prints the result of an expression to the console.
- read(): Reads input from the console and returns it.
- kill(): Terminates the program.
- main(): Program entry point - must be defined.

----------
----------
## Runtime
----------

- Function parameters and locals reside on the stack
- Instruction operands are popped from the stack in specified order

When a function is called, save a function stack frame ptr (fsfp) and the return address (ra), and save parameters on the stack.
- Within a function block, all references are relative to the fsfp.
- On return, invalidate everything past the sfp and jump back to the ra.

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
    Pop  <src> <dest>
    Push <val>
    Load <src>

    // Control
    Kill
    Ret
    Call
    Jmp  <dest>
    Jmpc <cond> <dest>

    // Builtin functions
    Prnt <op1>
    Read

-------------------
