///*
// * Stack.c
// *
// *  Created on: 2020年10月29日
// *      Author: HP
// */
#include"Stack.h"
//
//void CreateStack(Stack* s)
//{
//    s->top = -1;
//}
//
//uint8 isEmpty(Stack* s)
//{
//    return s->top == -1;
//}
//
//uint8 isFull(Stack* s)
//{
//    return s->top == MAXSIZE - 1;
//}
//
//void Push(Stack* s, uint8 x, uint8 y)
//{
//    if(isFull(s))
//        return ;
//    s->top += 1;
//    s->p[s->top].x = x;
//    s->p[s->top].y = y;
//}
//
//Point Pop(Stack* s)
//{
//    Point p;
//    p.x = 0xff;
//    p.y = 0xff;
//    if(isEmpty(s))
//        return p;
//    return s->p[(s->top)--];
//}
//
void InitStack(stack *s,uint32 max,Point *data)
{
    s->MAX = max;
    s->top = -1;
    s->data = data;
}


uint8 FullStack(stack * s)
{
    return ((s->top)+1)==(s->MAX);

}

uint8 EmptyStack(stack *s)
{
    return (s->top) == -1;
}

uint8 PushStack(stack * s, uint8 x, uint8 y)
{
    if(FullStack(s))return 0;
    Point a;
    a.x = x;
    a.y = y;
    s->data[s->top+1] = a;
    s->top = s->top+1;
    return 1;
}

Point PopStack(stack *s)
{
    if(EmptyStack(s))
    {
        Point a;
        a.x = 0xff;a.y = 0xff;
        return a;
    }
    Point ret = s->data[s->top];
    s->top = s->top-1;
    return ret;
}
