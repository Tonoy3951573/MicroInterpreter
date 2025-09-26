# MicroInterpreter

**MicroInterpreter** is a tiny C-based interpreter for a custom micro-language. It supports variable declaration, arithmetic operations, input, and output. Designed as an educational tool, it helps you understand how interpreters parse and execute code line by line.

---

## Features
- Variable declaration: `var x = 5`
- Arithmetic operations: `+`, `-`, `*`, `/`
- Input from user: `input x`
- Print values or expressions: `print x + 5`
- Simple and easy-to-extend design

---

## Installation
1. Clone the repository:
```bash
git clone https://github.com/Tonoy3951573/MicroInterpreter-A-Simple-Custom-Programming-Language-Interpreter.git
```
2. Compile the interpreter:
```bash
gcc micro_interpreter.c -o micro
```
3. Run a MicroLang script
```bash
./micro example.mic
```
---
# Usage Example 
 Example `example.mic`:
 ```mic
var x = 5
var y = 10
print x
print y
var z
input z
print z + x * y
```
Output (if input is `2` for z):
```
5
10
25
```
---
# How it works
1. Reads the source file line by line
2. Parses each line:
  -Variable declaration and assignment
  - Arithmetic expression evaluation
  - Printing strings or evaluated expressions
  - Updating variable values from input
3. Evaluates expression supporting `+`,`-`,`*`,`/`.
---
# Future Improvements
  - Support floating-point numbers
  - Conditional statement(`if`,`else`)
  - Loops (`while`,`for`)
  - Functions
  - Improved error handeling and syntax checking
---
# Repository Structure
```bash
MicroInterpreter/
├── micro_interpreter.c   # Main interpreter code
├── example.mic           # Sample MicroLang script
├── README.md             # Project documentation
└── LICENSE               # Optional license file
```


