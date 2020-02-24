#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP 100
#define NUMBER '0'

int getop (char []);
void push (double);
double pop(void);
void top(void);
void swap(void);
void clear(void);
void dup(void);
void pow_op(void);

int main(){
    int type;
    double op2;
    char s[MAXOP];
    int temp1;
    int temp2;
    double tempmod;
    double e_const = 2.71828;

    while((type = getop(s)) != EOF) {
        switch(type){
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if(op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("error: zero divisor\n");
                break;
                //4.3
            case '%':
                temp1 = pop();
                temp2 = pop();
                tempmod = temp2 % temp1;
                push(tempmod);
                break;
                //4.4
            case '?':
                top();
                break;                
            
            case '$':
                printf("Swapping the top two values...\n");
                printf("Here are your values now: ");
                swap();
                break;
            
            case '&':
                printf("Duplicating the top of your stack...\n");
                dup();
                break;
            
            case '!':
                printf("Now clearing the stack...\n");
                clear();
                printf("Your stack is now cleared.\n");
                break;
                //4.5
            case 's':
                push(sin(pop()));
                break;

            case 'p':
                pow_op();
                break;

            case 'e':
                push(pow(e_const, pop()));
                break;

            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unkown command %s\n" ,s);
                break;
            }
                
        }
        return 0;
}

#define BUFSIZE 100
char buf[BUFSIZE];
char bufp  = 0;

int getch(void){
    return (bufp > 0) ? buf[--bufp]:getchar();
}
void ungetch(int c){
    if(bufp >= BUFSIZE){
        printf("ungetch: too many characters\n");
    }else{
        buf[bufp++] = c;
    }
}

#define MAXVAL 100
int sp = 0;
double val[MAXVAL];

void push(double f){
    if (sp < MAXVAL){
        val[sp++]=f;
    }else{
        printf("error: stack full, cant push %g\n", f);
    }
}

double pop(void){
    if(sp > 0){
        return val[--sp];
    }else{
        printf("error: stack empty \n");
        return 0.0;
    }
}

int getop(char s []){
    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t');
    s[1] = '\0';
    if(!isdigit(c) && c != '.'){
        return c;
    }
    i = 0;
    if(isdigit(c)){
        while(isdigit(s[++i] = c = getch()));
    }
    if(c == '.'){
        while(isdigit(s[++i] = c = getch()));
    }
    s[i] = '\0';
    if(c != EOF){
        ungetch(c);
    }
    return NUMBER;
}

//4.4
void top(void){
    if (sp > 0){
        printf("Here is the top of your stack: %.2f\n", val[sp-1]);
    }else{
        printf("error: stack is empty.\n");
    }
    push(val[sp-1]);
}

void swap(void){
    double temp;
    if(sp > 2){
        temp = val[sp-1];
        val[sp-1] = val[sp-2];
        val[sp-2] = temp;
    }
    printf("%.2f, %.2f\n", val[sp-1], val[sp - 2]);
    push(val[sp-1]);

}

void clear(void){
    for(int i = 0; i < sp; i++){
        pop();
    }
}

void dup(void){
    if(sp > 0){
        push(val[sp-1]);
    }else{
        printf("There are no values to duplicate.\n");
    }
}

//4.5
void pow_op(void){
    double of = pop();
    double power = pop();

    if(of == 0){
        push(1);
    }else if(power == 0 && of < 0){
        printf("You cannot divide by 0.\n");
    }else{
        push(pow(power, of));
    }
}
