//
//  main.c
//  rpn
//
//  Created by William McCarthy on 192//20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//  Alfredo Rodriguez - Garcia 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MATH '1'
#define VARIABLE '2'
#define STACK '3'
#define MAXVAL  100

double variables[26];

size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}


double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';
  
  if(isalpha(c)){
        i = 0;
        s[i++] = c;
        while(isalpha(s[i++] = c = getch_()))
        s[i] = '\0';
        return MATH;
    }

    if(c == '=' || c == '?'){
        i=0;
        s[i++] = c;
        while(isalpha(s[i++] = c = getch_()));
        return VARIABLE;
    }

    if(c == '@'){
        i=0;
        s[i++] = c;
        while(isalpha(s[i++] = c = getch_()));
        return STACK;
    }

  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return

  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s

}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
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

void Math(char * s){
    double op1, op2, result = 0;
    if(strcmp(s, "sin\n") == 0){
        result = sin(pop());
    }else if(strcmp(s, "cos\n") == 0){
        result = cos(pop());
    }else if(strcmp(s, "exp\n") == 0){
        result = exp(pop());
    }else if(strcmp(s, "pow\n") == 0){
        op1 = pop();
        op2 = pop();
        result = pow(op2, op1);
    }
    push(result);
    printf("\t%.2f\n", result);
}

void Var(char * s){
    if(strspn(s,"=")){
        double temp = variables[s[2]-'A'] = pop();
        push(temp);
        
    }else if(strspn(s,"?")){
        push(variables[s[2]-'A']);
        printf("\t%.2f\n",variables[s[2]-'A']);
    }
}

void Stack(char*s){
    if(strcmp(s, "@Top\n") == 0){
        if (sp > 0){
            printf("Here is the top of your stack: %.2f\n", val[sp-1]);
        }else{
            printf("error: stack is empty.\n");
        }
        push(val[sp-1]);
    }else if(strcmp(s, "@Swap\n") == 0){
        double temp;
        if(sp > 2){
            printf("Now swapping your values...\n");
            temp = val[sp-1];
            val[sp-1] = val[sp-2];
            val[sp-2] = temp;
        }
        printf("%.2f, %.2f\n", val[sp-1], val[sp - 2]);
        push(val[sp-1]);
    }else if(strcmp(s, "@Dup\n") == 0){
        if(sp > 0){
            push(val[sp-1]);
            printf("Your value has been duplicated.\n");
        }else{
            printf("There are no values to duplicate.\n");
        }
    }else if(strcmp(s, "@Clear\n") == 0){
        for(int i = 0; i < sp; i++){
        pop();
        }
        printf("Your stack is cleared...\n");

    }
}

void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':        printf("\t%.8g\n", pop());  break;
      case NUMBER:      push(atof(s));              break;
      case MATH:        Math(s);                    break;
      case VARIABLE:    Var(s);                     break;
      case STACK:       Stack(s);
      case '+':         push(pop() + pop());        break;
      case '*':         push(pop() * pop());        break;
      case '-':         push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
      default:      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}
