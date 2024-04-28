#ifndef STACK_H
#define STACK_H
#include "headfile.h"
#define Stack_Size 2000

typedef struct stack
{
    int32 top;
    uint32 MAX;
    Point *data;
}stack;
extern void InitStack(stack *s,uint32 max,Point *data);

extern uint8 FullStack(stack * s);

extern uint8 EmptyStack(stack *s);

extern uint8 PushStack(stack * s, uint8 x, uint8 y);

extern Point PopStack(stack * s);

#endif // STACK_H
