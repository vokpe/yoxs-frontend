# Building a Tree-Walking Interpreter

## Build our own Lexer, Parser, Tree Representation, and Evaluator

Expressed as a list of features, Monkey has the following:
- C-like syntax
- variable bindings
- integers and booleans
- arithmetic expressions
- built-in functions
- first-class and higher-order functions
- closures
- a string data structure
- an array data structure
- a hash data structure

```js
let age = 1;
let name = "Monkey";
let result = 10 * (20 / 2);

//binding an array of integers to a name
let myArray = [1, 2, 3, 4, 5];
myArray[0] // => 1

//And here is a hash, where values are associated with keys:
let thorsten = {"name": "Thorsten", "age": 28};
thorsten["name"] // => "Thorsten"

let add = fn(a, b) { return a + b; };
let add = fn(a, b) { a + b; }; //we can leave out the return if we want to

//recursive fibonacci
let fibonacci = fn(x) {
    if (x == 0) {
        0
    } else {
        if (x == 1) {
            1
        } else {
            fibonacci(x - 1) + fibonacci(x - 2);
        }
    }
};

// higher order functions (functions that take other functions as arguments)
let twice = fn(f, x) {
    return f(f(x));
};

let addTwo = fn(x) {
    return x + 2;
};

twice(addTwo, 2); // => 6
```

Our interpreter will have a few major parts
- the lexer
- the parser
- the Abstract Syntax Tree (AST)
- the internal object system
- the evaluator

## Chapter 1.1: Lexical Analysis

![alt text](fig1.png)

The first transformation from source code to tokens, is called `lexical analysis` or `lexing`. It is done by a lexer (aka tokenizer or scanner). Tokens themselves are small & easily categorizable data structures that are then fed to the parser, which does the second transformation to turn tokens into an "Abstract Syntax Tree".

For example: 

```js
"let x = 5 + 5;" 
```
when put into the lexer will generate:
```js
[
    LET,
    IDENTIFIER("x"),
    EQUAL_SIGN,
    INTEGER(5),
    PLUS_SIGN,
    INTEGER(5),
    SEMICOLON
]
```

Note: whitespace length is not converted to tokens, and are ignored. 

## 1.2 - Defining Our Tokens