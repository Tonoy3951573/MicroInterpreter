# MicroInterpreter

**MicroInterpreter** is a tiny C-based interpreter for a custom micro-language. It supports variable declaration, arithmetic operations, input, and output. Designed as an educational tool, it helps you understand how interpreters parse and execute code line by line.

---

## Features
- Support floating-point numbers
- Variable declaration: `int x = 5` , `float y = 5.5`, and `string str = "hello"`
- Arithmetic operations: `+`, `-`, `*`, `/`,`%`
- Input from user: `input x`
- Print values , expressions or string: `print x + 5`,`print "hello world"`
- Simple and easy-to-extend design

---

## Installation
1. Clone the repository:
```bash
git clone https://github.com/Tonoy3951573/MicroInterpreter.git
cd MicroInterpreter
```
2. Compile the interpreter:
```bash
gcc -lm micro_interpreter.c -o micro
```
3. Run a MicroLang script
```bash
./micro example.mic
```
---
# Usage Example 
 Example `example.mic`:
 ```mic
int x = 5
int y = 10
print x
print y
float z
input z
print z + x * y
```
Output (if input is `2.00` for z):
```
5
10
25.00
```
---
# How it works
1. Reads the source file line by line
2. Parses each line:
  - Variable declaration and assignment
  - Arithmetic expression evaluation
  - Printing strings or evaluated expressions
  - Updating variable values from input
3. Evaluates expression supporting `+`,`-`,`*`,`/`,`%`.
---
# Future Improvements
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


