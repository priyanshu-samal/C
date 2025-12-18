#include <stdio.h>
 int transform(int c){
    return ~c;
 }

 int main(){
    int c;
    while((c=getchar()) != EOF){
        putchar(transform(c));
    }
 }