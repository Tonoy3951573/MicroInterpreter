#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAXVARS 256
typedef struct {
    char var_name[MAXVARS];
    int value;
} vars;

vars var[MAXVARS];
int var_count = 0;

int get_var(const char name[]){
    for(int i = 0; i < var_count; i++){
        if(strcmp(var[i].var_name,name)== 0){
            return var[i].value;
        }
    }
    return 0;
}

void set_var(const char name[], const int value){
    for(int i = 0; i < var_count; i++){
        if(strcmp(var[i].var_name,name)== 0){
            var[i].value = value;
        }
    }
}

int eval(char *expr) {
    char *p = expr;
    int result = 0;
    int num = 0;
    char var[64], op = '+';

    while (*p) {
        while (isspace(*p)) p++;

        if (isdigit(*p)) {
            num = strtol(p, &p, 10);
        } else if (isalpha(*p)) {
            int i = 0;
            while (isalnum(*p)) var[i++] = *p++;
            var[i] = '\0';
            num = get_var(var);
        } else {
            p++;
            continue;
        }

        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': result /= num; break;
        }

        while (isspace(*p)) p++;
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
            op = *p++;
    }
    return result;
}

void run_line(char line[]){
    
    if(strstr(line,"var") != NULL){
        char arg1[256],arg2[256];
        if(strstr(line,"=") != NULL){

            sscanf(line,"var %s = %[^\n]",arg1,arg2);
            strcpy(var[var_count].var_name, arg1);
            var[var_count].value = eval(arg2);
        }
        else{

            sscanf(line,"var %s ",arg1);
            strcpy(var[var_count].var_name, arg1);
            var[var_count].value = 0;
        }
        var_count++;
        
    }
    else if(strstr(line,"print") != NULL){
        char arg1[256];
        if(strstr(line,"\"") != NULL){
            sscanf(line,"print %[^\n]",arg1);
            printf("%s\n",arg1);
        }
        else{
            sscanf(line,"print %[^\n]",arg1);
            printf("%d\n",eval(arg1));
        }
    }
    else if(strstr(line,"input") != NULL){
        int value = 0;
        char arg1[256];
        scanf("%d",&value);
        sscanf(line, "input %[^\n]",arg1);
        set_var(arg1,value);
    }
}

int main( int argc , char **argv){
    if(argc < 2){
        printf("Uses : ./Micro <source_code.mic>");
        return 0;
    }
    else{
        FILE* fp = fopen(argv[1],"r");
        if(fp == NULL){
            perror("open");
        }
        else{
            char line[256];
            while(fgets(line,sizeof(line),fp)){
                run_line(line);
            }
        }
        fclose(fp);
    }

}