#include "mem_util.h"

//内存初始化
void memman_init(struct MEMMAN *man) {
    man->frees = 0;
    man->maxfrees = 0;
    man->lostsize = 0;
    man->losts = 0;
}

//可用总内存
unsigned int memman_total(struct MEMMAN *man) {
    unsigned int i, t = 0;
    for (i = 0; i < man->frees; i++) {
        t += man->free[i].size;
    }

    return t;
}

//分配内存
//参数：内存地址，申请内存大小
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size) {
    unsigned int i, a, j;
    //首次适配算法
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].size >= size) {
            a = man->free[i].addr;
            man->free[i].addr += size;
            man->free[i].size -= size;
            if (man->free[i].size == 0) {   //清楚长度为0的空闲段
                for(j = i; j < man->frees - 1; j++){
                    man->free[j] = man->free[j+1];
                }
                man->frees--;
            }
            return a;
        }
    }

    return 0;
}

//释放内存
//参数：内存地址，释放内存所在地址，释放大小
//注意：只能用来释放用memman_alloc请求的内存
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size) {
    int i, j, k;
    //free[i]是第一个地址比所释放的地址大的空闲段，i为man->frees时free[i]不是合法的空闲段
    for (i = 0; i < man->frees; i++) {
        if (man->free[i].addr > addr) {
            break;
        }
    }

    if (i > 0) {
        if (man->free[i-1].addr + man->free[i-1].size == addr) {  //可以和上一段接起来
           man->free[i-1].size += size;
           if (i < man->frees) {
               //free[i]即下一段是合法的空闲段
               if (addr + size == man->free[i].addr) {  //可以和下一段接起来
                   man->free[i-1].size += man->free[i].size;
                   //合并下一段之后将下一段从表中删除
                   for(k = i; k < man->frees - 1; k++){
                        man->free[k] = man->free[k+1];
                    }
                   man->frees--;
               }
           }
     
           return 0;
        }
    }

    if (i < man->frees) {
        //不可以和上一段接起来，但可以和下一段接起来
        if (addr + size == man->free[i].addr) {
           man->free[i].addr = addr;
           man->free[i].size += size;
           return 0;
        }
    }

    if (man->frees < MEMMAN_FREES) {
        //不可以和上一段接起来，也不可以和下一段接起来
        //在表中插入新的空闲段位于freep[i]
        for (j = man->frees; j > i; j--) {
            man->free[j] = man->free[j-1];
        }

        man->frees++;
        if (man->maxfrees < man->frees) {
            man->maxfrees = man->frees;
        }

        man->free[i].addr = addr;
        man->free[i].size = size;
        return 0;
    }

    //空闲段达到最大数，无法回收，记录丢失内存信息
    man->losts++;
    man->lostsize += size;
    return -1;
}

//分配4K大小的页
//参数：内存地址，申请内存大小
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size) {
    unsigned int a;
    size = (size + 0xfff) & 0xfffff000; //分配满足内存请求的整数个页
    a = memman_alloc(man, size);
    return a;
}

//释放请求4K大小的页
//参数：内存地址，释放内存所在地址，释放大小
//注意：只能用来释放用memman_alloc_4k请求的内存
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size) {
    int i ;
    size = (size + 0xfff) & 0xfffff000; //释放整数个页
    i = memman_free(man, addr, size);
    return i;
}



//初始化栈
void initInt(IntStack * s){
    s->first=0;
}

//判断栈中是不是为空
int isEmptyInt(IntStack * s){
    if(s->first == 0){
        return 1;
    } else{
        return 0;
    }
}

//加入一个元素
int pushInt(IntStack * s, int v,struct MEMMAN * memman){
 //   IntStackNode * ptr = (IntStackNode*) malloc(sizeof(IntStackNode));
    IntStackNode* ptr = (IntStackNode*) memman_alloc(memman,16);
    if(ptr==0){
        return 0;
    }
    ptr->next=s->first;
    ptr->value=v;
    s->first=ptr;
    return 1;
}

//删除一个元素,并返回栈顶的值,使用之前要保证不为空
int popInt(IntStack * s ,struct MEMMAN * memman){
    if(isEmptyInt(s)){
        return 0;
    }
    int del = s->first->value;
    IntStackNode * ptr = s->first;
    s->first = ptr->next;
    //free(ptr);
    memman_free(memman,(unsigned int)ptr,16);
    return del;
}

//查看栈顶的一个值，并不弹出,同时必须保证不为空
int frontInt(IntStack * s){
    if(isEmptyInt(s)){
        return 0;
    }
    return s->first->value;
}

//清空整个stack
void clearInt(IntStack * s ,struct MEMMAN * memman){
    while (s->first != 0){
        popInt(s,memman);
    }
}

//初始化栈
void initChar(CharStack * s){
    s->first=0;
}

//判断栈中是不是为空
int isEmptyChar(CharStack * s){
    if(s->first == 0){
        return 1;
    } else{
        return 0;
    }
}

//加入一个元素
int pushChar(CharStack * s, char v, struct MEMMAN * memman){
    CharStackNode * ptr = (CharStackNode*) memman_alloc(memman,16);
    if(ptr==0){
        return 0;
    }
    ptr->next=s->first;
    ptr->value=v;
    s->first=ptr;
    return 1;
}

//删除一个元素,并返回栈顶的值,使用之前要保证不为空
char popChar(CharStack * s , struct MEMMAN * memman){
    if(isEmptyChar(s)){
        return 0;
    }
    char del = s->first->value;
    CharStackNode * ptr = s->first;
    s->first = ptr->next;
    memman_free(memman,(unsigned int) ptr,16);
    return del;
}

//查看栈顶的一个值，并不弹出,同时必须保证不为空
char frontChar(CharStack * s){
    if(isEmptyChar(s)){
        return 0;
    }
    return s->first->value;
}

//清空整个stack
void clearChar(CharStack * s , struct MEMMAN * memman){
    while (s->first != 0){
        popChar(s,memman);
    }
}


int beforePriority(char op){
    int priority=0;
    if(op=='+' || op=='-'){
        priority=2;
    }else if (op=='*'||op=='/')
    {
        priority=4;
    }else if (op=='(')
    {
        priority=8;
    }else if (op==')')
    {
        priority=1;
    }else if (op == '^')
    {
        priority = 7;
    }else
    {
        priority = -1;
    }
    return priority;
}


int afterPriority(char op){
    if(op == '+' || op == '-'){
        return 3;
    }else if (op == '*' || op == '/')
    {
        return 5;
    }else if (op=='(')
    {
        return 1;
    }else if (op == ')')
    {
        return 9;
    }else if (op == '^')
    {
        return 6;
    }else
    {
        return -1;
    }
}


int getLength(const char * str){
    int x = 0;
    while (1){
        if (str[x] != '\0'){
            x++;
        }else{
            break;
        }
    }
    return x;
}
int getData(char expression[] , int * beg){
    int i =*beg;
    int length = getLength(expression);
    int num = 0;
    int posOrNag=1;

    //第一个数字特殊处理
    if(expression[i] >= 48 && expression[i] <= 57){
        num = expression[i]-'0';
    }else if(expression[i] == '+'){
        posOrNag = 1;
    }else if(expression[i] == '-'){
        posOrNag = 0;
    }else{
        return 0;
    }
    int j;
    for (j = i+1; j < length; ++j) {
        if(expression[j] >= 48 && expression[j] <= 57)
        {
            num *= 10;
            num += (expression[j]-'0');
        }else{
            
            break;
        }
    }
    *beg = j-1;
    if(posOrNag == 0){
        num = 0 - num;
    }

    return num;
}
int result(IntStack * numStack,char op,struct MEMMAN * memman){
    int first = popInt(numStack,memman);
    int second = popInt(numStack,memman);
    int ans=0;
    if(op == '-'){
        ans = second - first;
    }else if (op == '+')
    {
        ans = first + second;
    }else if(op == '*'){
         ans = first * second;
    }else if (op == '/')
    {
        ans = second/first;
    }else if (op == '^')
    {
        ans = 1;
        for (int i = 0; i < first; ++i) {
            ans *= second;
        }
    }else
    {
        return 0;
    }
    
    return pushInt(numStack,ans,memman);
}
int calculator(char expression[], struct MEMMAN * memman){

    //查询字符数组的长度
    int length = getLength(expression);

    //创建数字栈
    IntStack numStack;
    initInt(&numStack);

    //创建符号栈
    CharStack  opStack;
    initChar(&opStack);

    int number = 0;
    for (int i = 0; i < length; ++i) {
        //处理单目运算符
        if(i==0){
            if(expression[0] == '-' || expression[0] == '+'){
                number = getData(expression,&i);
                pushInt(&numStack,number,memman);
                continue;
            }
        }

        if(expression[i]>=48 && expression[i]<=57){
            number = getData(expression,&i); 
            pushInt(&numStack,number,memman);
        }else if(expression[i] == '+' || expression[i]=='-' || expression[i]=='*'
            || expression[i]=='/' ||expression[i]=='^'){
            if(isEmptyChar(&opStack)){
                    pushChar(&opStack,expression[i],memman);
                }else{
                    int out = beforePriority(expression[i]);
                    int in = afterPriority(frontChar(&opStack));
                    if(out > in){
                        pushChar(&opStack,expression[i],memman);
                    }else{
                        char op = popChar(&opStack,memman);
                        int a = result(&numStack,op,memman);
                        if(!a){
//                            printf("C1 err\n");
//                            exit(1);
                        }
                        pushChar(&opStack,expression[i],memman);
                    }
                }
        }else if(expression[i]=='('){
            pushChar(&opStack,expression[i],memman);
                //
                if(expression[i+1]=='+' || expression[i+1]=='-'){
                    //同上可优化
                    ++i;
                    number = getData(expression,&i);
                    pushInt(&numStack,number,memman);
                }
        }else if(expression[i]==')'){
            while (frontChar(&opStack) != '('){
                    char x = popChar(&opStack,memman);
                    int a =result(&numStack,x,memman);
                    if(!a){
//                        printf("C2 err\n");
//                        exit(2);
                    }
                }
                popChar(&opStack,memman);
        }

    }

    while (!isEmptyChar(&opStack)){
        char op = popChar(&opStack,memman);
        if(op!='+' && op!='-' && op!='*' && op!='/' && op!='^'){
//            printf("C3 err\n");
//            exit(3);
        }
        int a = result(&numStack,op,memman);
        if(!a){
//            printf("C4 err\n");
//            exit(4);
        }
    }

    int cal = popInt(&numStack,memman);
    if(!isEmptyInt(&numStack)){
//        printf("C5 err\n");
//        exit(5);
    }

    clearChar(&opStack,memman);
    clearInt(&numStack,memman);

    return cal;

}