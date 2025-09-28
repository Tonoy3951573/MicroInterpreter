#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXVARS 256
#define MAXTOKENS 64

typedef struct {
    char var_name[MAXVARS];
    char data_type; 
    union {
        int ivar;
        double fvar;
        char *svar;
    } value;
} vars;
int cotationfound1=0,cotationfound2=0;

vars var[MAXVARS];
int var_count = 0;

void error(const char *msg, const char *line, int lineno) {
    fprintf(stderr, "Line %d: %s\n   --> %s\n", lineno, msg, line);
    exit(1);
}

int valid_identifier(const char *s) {
    if (!isalpha(s[0]) && s[0] != '_') return 0;
    for (int i = 1; s[i]; i++) {
        if (!isalnum(s[i]) && s[i] != '_') return 0;
    }
    return 1;
}

int tokenize(char *line, char *tokens[]) {
    int count = 0;
    char *p = line;

    while (*p) {
        while (isspace(*p)) p++; 
        if (*p == '\0') break;

        if (*p == '"') { 
            cotationfound1=1;
            p++; 
            char *start = p;
            while (*p && *p != '"') p++; 
            int len = p - start;

            tokens[count] = (char*)malloc(len + 1);
            strncpy(tokens[count], start, len);
            tokens[count][len] = '\0';
            count++;

            if (*p == '"'){p++; cotationfound2 = 1;}
        } else {
            char *start = p;
            while (*p && !isspace(*p)) p++;
            int len = p - start;

            tokens[count] = (char*)malloc(len + 1);
            strncpy(tokens[count], start, len);
            tokens[count][len] = '\0';
            count++;
        }
    }
    return count;
}

double get_var(const char name[]) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var[i].var_name, name) == 0) {
            if (var[i].data_type == 'i') return (double)var[i].value.ivar;
            else if (var[i].data_type == 'f') return var[i].value.fvar;
        }
    }
    return 0.0;
}

char* get_var_string(const char name[]) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(var[i].var_name, name) == 0 && var[i].data_type == 's') {
            return var[i].value.svar;
        }
    }
    return NULL;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

double apply_op(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) { fprintf(stderr, "Error: Division by zero\n"); exit(1); }
            return a / b;
        case '%': {
            int ia = (int)a, ib = (int)b;
            if (ib == 0) { fprintf(stderr, "Error: Modulo by zero\n"); exit(1); }
            return ia % ib;
        }
    }
    return 0;
}

double eval(const char *expr) {
    double values[1024]; int topVal = -1;
    char ops[1024]; int topOps = -1;
    const char *p = expr;

    while (*p) {
        while (isspace(*p)) p++;

        if (isdigit(*p) || (*p == '.' && isdigit(*(p+1)))) {
            double num = strtod(p, (char**)&p);
            values[++topVal] = num;
        } 
        else if (isalpha(*p) || *p == '_') {
            char varname[64]; int i = 0;
            while (isalnum(*p) || *p == '_') varname[i++] = *p++;
            varname[i] = '\0';
            values[++topVal] = get_var(varname);
        } 
        else if (*p == '(') {
            ops[++topOps] = *p++;
        } 
        else if (*p == ')') {
            while (topOps >= 0 && ops[topOps] != '(') {
                if (topVal < 1) { fprintf(stderr, "Error: Not enough operands\n"); exit(1); }
                double b = values[topVal--];
                double a = values[topVal--];
                char op = ops[topOps--];
                values[++topVal] = apply_op(a, b, op);
            }
            if (topOps < 0 || ops[topOps] != '(') { fprintf(stderr, "Error: Mismatched parentheses\n"); exit(1); }
            topOps--; 
            p++;
        } 
        else if (*p == '-') {
            if (p == expr || strchr("+-*/%(", *(p-1))) {
                p++;
                if (isdigit(*p) || (*p == '.' && isdigit(*(p+1)))) {
                    double num = strtod(p, (char**)&p);
                    values[++topVal] = -num;
                } else if (isalpha(*p) || *p == '_') {
                    char varname[64]; int i = 0;
                    while (isalnum(*p) || *p == '_') varname[i++] = *p++;
                    varname[i] = '\0';
                    values[++topVal] = -get_var(varname);
                } else if (*p == '(') {
                    ops[++topOps] = '~';
                    p++;
                } else {
                    fprintf(stderr, "Error: Invalid use of unary minus\n");
                    exit(1);
                }
            } else {
                char op = *p++;
                while (topOps >= 0 && precedence(ops[topOps]) >= precedence(op)) {
                    if (topVal < 1) { fprintf(stderr, "Error: Not enough operands\n"); exit(1); }
                    double b = values[topVal--];
                    double a = values[topVal--];
                    char op2 = ops[topOps--];
                    values[++topVal] = apply_op(a, b, op2);
                }
                ops[++topOps] = op;
            }
        }
        else if (strchr("+-*/%", *p)) {
            char op = *p++;
            while (topOps >= 0 && precedence(ops[topOps]) >= precedence(op)) {
                if (topVal < 1) { fprintf(stderr, "Error: Not enough operands\n"); exit(1); }
                double b = values[topVal--];
                double a = values[topVal--];
                char op2 = ops[topOps--];
                values[++topVal] = apply_op(a, b, op2);
            }
            ops[++topOps] = op;
        } 
        else if (*p) {
            fprintf(stderr, "Error: Unexpected character '%c'\n", *p);
            exit(1);
        }
    }

    while (topOps >= 0) {
        if (ops[topOps] == '~') {
            if (topVal < 0) { fprintf(stderr, "Error: Missing operand for unary minus\n"); exit(1); }
            values[topVal] = -values[topVal];
            topOps--;
            continue;
        }
        if (topVal < 1) { fprintf(stderr, "Error: Not enough operands\n"); exit(1); }
        double b = values[topVal--];
        double a = values[topVal--];
        char op = ops[topOps--];
        values[++topVal] = apply_op(a, b, op);
    }

    if (topVal != 0) { fprintf(stderr, "Error: Invalid expression\n"); exit(1); }
    return values[topVal];
}

void run_line(char line[], int *lineno) {
    (*lineno)++;
    char *tokens[MAXTOKENS];
    int count = tokenize(line, tokens);
    if (count == 0) return;

    if (strcmp(tokens[0], "print") == 0) {
        if (count > 1) {
            if (get_var_string(tokens[1]) != NULL) {
                printf("%s\n", get_var_string(tokens[1]));
            }
            else if(cotationfound1==1 && cotationfound2==1){
                printf("%s\n", tokens[1]);
            }
            else if(cotationfound1==1 && cotationfound2==0){
                error("Error : Missing Ending Cotaion (\").",line,*lineno);
            }
            else if(cotationfound1==0 && cotationfound2==1){
                error("Error : Missing Starting Cotaion (\").",line,*lineno);
            }
            else {
                char *endptr;
                double num = strtod(tokens[1], &endptr);
                if (*endptr == '\0') {
                    if (floor(num) == num) printf("%d\n", (int)num);
                    else printf("%f\n", num);
                }
                else {
                    double val = eval(tokens[1]);
                    if (floor(val) == val) printf("%d\n", (int)val);
                    else printf("%f\n", val);
                }
            }
        }
    }

    else if (strcmp(tokens[0], "float") == 0) {
        var[var_count].data_type = 'f';
        if(!valid_identifier(tokens[1])){
            error("Error : Invalid Identifier.",line,*lineno);
        }
        strcpy(var[var_count].var_name, tokens[1]);
        if (count >= 4 && strcmp(tokens[2], "=") == 0) {
            var[var_count].value.fvar = eval(tokens[3]);
        } else var[var_count].value.fvar = 0.0;
        var_count++;
    }

    else if (strcmp(tokens[0], "int") == 0) {
        var[var_count].data_type = 'i';
        if(!valid_identifier(tokens[1])){
            error("Error : Invalid Identifier.",line,*lineno);
        }
        strcpy(var[var_count].var_name, tokens[1]);
        if (count >= 4 && strcmp(tokens[2], "=") == 0) {
            var[var_count].value.ivar = (int)eval(tokens[3]);
        } else var[var_count].value.ivar = 0;
        var_count++;
    }

    else if (strcmp(tokens[0], "string") == 0) {
        var[var_count].data_type = 's';
        if(!valid_identifier(tokens[1])){
            error("Error : Invalid Identifier.",line,*lineno);
        }
        strcpy(var[var_count].var_name, tokens[1]);
        if (count >= 4 && strcmp(tokens[2], "=") == 0) {
            var[var_count].value.svar = strdup(tokens[3]);
        } else {
            var[var_count].value.svar = strdup("");
        }
        var_count++;
    }

    else if (strcmp(tokens[0], "input") == 0) {
        int i;
        for (i = 0; i < var_count; i++)
            if (strcmp(var[i].var_name, tokens[1]) == 0) break;

        if (var[i].data_type == 'i') scanf("%d", &var[i].value.ivar);
        else if (var[i].data_type == 'f') scanf("%lf", &var[i].value.fvar);
        else if (var[i].data_type == 's') {
            char buffer[256];
            scanf(" %[^\n]", buffer);
            free(var[i].value.svar);
            var[i].value.svar = strdup(buffer);
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./micro <source_code.mic>\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("open");
        return 1;
    }

    char line[256];
    int line_no = 0;
    while (fgets(line, sizeof(line), fp)) {
        run_line(line,&line_no);
    }
    fclose(fp);

    for (int i = 0; i < var_count; i++)
        if (var[i].data_type == 's') free(var[i].value.svar);

    return 0;
}
