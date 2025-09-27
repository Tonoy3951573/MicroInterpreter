#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

#define MAXVARS 256
typedef struct {
    char var_name[MAXVARS];
    char data_type;  
    union {
       int ivar;
       float fvar;
       char *svar;   
    } value;
} vars;

vars var[MAXVARS];
int var_count = 0;

float get_var(const char name[]){
    for(int i = 0; i < var_count; i++){
        if(strcmp(var[i].var_name,name)== 0){
            if(var[i].data_type == 'i') return var[i].value.ivar;
            else if(var[i].data_type == 'f') return var[i].value.fvar;
        }
    }
    return 0.0f;
}

char* get_var_string(const char name[]){
    for(int i = 0; i < var_count; i++){
        if(strcmp(var[i].var_name,name)== 0 && var[i].data_type == 's'){
            return var[i].value.svar;
        }
    }
    return NULL;
}

float eval(char *expr) {
    //eval will be updated soon, let use it for normal calculation
    char *p = expr;
    float result = 0;
    float num = 0;
    char var_name[64], op = '+';

    while (*p) {
        while (isspace(*p)) p++;

        if (isdigit(*p)) {
            num = strtol(p, &p, 10);
        } else if (isalpha(*p)) {
            int i = 0;
            while (isalnum(*p)) var_name[i++] = *p++;
            var_name[i] = '\0';
            num = get_var(var_name);
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
    if(strstr(line,"print") != NULL){
        char arg1[256];
        sscanf(line,"print %[^\n]",arg1);

        if(arg1[0] == '\"'){
            char inside[256];
            sscanf(arg1,"\"%[^\"]\"",inside);
            printf("%s\n",inside);
        } 
        else {
            char *strval = get_var_string(arg1);
            if(strval != NULL){
                printf("%s\n",strval);
            }
            else {
                float val = eval(arg1);
                if(floor(val) == val) 
                    printf("%d\n",(int)val);
                else
                    printf("%f\n",val);
            }
        }
    }
    else if(strstr(line,"float") != NULL){
        char arg1[256],arg2[256];
        var[var_count].data_type = 'f';
        if(strstr(line,"=") != NULL){
            sscanf(line,"float %s = %[^\n]",arg1,arg2);
            strcpy(var[var_count].var_name, arg1);
            var[var_count].value.fvar = eval(arg2);
        }
        else{
            sscanf(line,"float %s ",arg1);
            strcpy(var[var_count].var_name, arg1);
            var[var_count].value.fvar = 0;
        }
        var_count++;
    }
    else if(strstr(line,"int") != NULL){
        char arg1[256],arg2[256];
        var[var_count].data_type = 'i';
        if(strstr(line,"=") != NULL){
            sscanf(line,"int %s = %[^\n]",arg1,arg2);
            strcpy(var[var_count].var_name, arg1);
            var[var_count].value.ivar = eval(arg2);
        }
        else{
            sscanf(line,"int %s ",arg1);
            strcpy(var[var_count].var_name, arg1);
            var[var_count].value.ivar = 0;
        }
        var_count++;
    }
    else if(strstr(line,"string") != NULL){
        char arg1[256],arg2[256];
        var[var_count].data_type = 's';
        if(strstr(line,"=") != NULL){
            sscanf(line,"string %s = \"%[^\"]\"",arg1,arg2);
            strcpy(var[var_count].var_name,arg1);
            var[var_count].value.svar = strdup(arg2);
        }
        else{
            sscanf(line,"string %s",arg1);
            strcpy(var[var_count].var_name,arg1);
            var[var_count].value.svar = strdup("");
        }
        var_count++;
    }
    else if(strstr(line,"input") != NULL){
        char arg1[256];
        sscanf(line, "input %[^\n]",arg1);
        int i;
        for(i = 0; i < var_count; i++){
            if(strcmp(var[i].var_name,arg1) == 0) break;
        }
        if(var[i].data_type == 'i'){
            scanf("%d",&var[i].value.ivar);
        }
        else if(var[i].data_type == 'f'){
            scanf("%f",&var[i].value.fvar);
        }
        else if(var[i].data_type == 's'){
            char buffer[256];
            scanf(" %[^\n]",buffer);
            free(var[i].value.svar);
            var[i].value.svar = strdup(buffer);
        }
    }
    else{
        printf("Syntax Error ! \n");
        exit(-1);
    }
}

int main(int argc , char **argv){
    if(argc < 2){
        printf("Uses : ./micro <source_code.mic>");
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
