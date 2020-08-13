#ifndef __MEM_MANAGE_H
#define __MEM_MANAGE_H

#define  MEMMAN_FREES  4096

struct FREEINFO {
    unsigned int addr, size;
};

struct MEMMAN {
    int frees, maxfrees, lostsize, losts;
    struct FREEINFO free[MEMMAN_FREES];
};

void memman_init(struct MEMMAN *man);

unsigned int memman_total(struct MEMMAN *man);

unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);

int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);

unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);

int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

typedef struct IntStackNode{
    int value;
    struct IntStackNode* next;
}IntStackNode;

typedef struct IntStack {
    IntStackNode * first;
}IntStack;

void initInt(IntStack* s);
int isEmptyInt(IntStack * s);
int pushInt(IntStack * s, int v,struct MEMMAN * mem);
int popInt(IntStack * s,struct MEMMAN * mem);
int frontInt(IntStack * s);
void clearInt(IntStack * s , struct MEMMAN * mem);

typedef struct CharStackNode{
    char value;
    struct CharStackNode* next;
}CharStackNode;

typedef struct CharStack {
    CharStackNode * first;
}CharStack;

void initChar(CharStack * s);
int isEmptyChar(CharStack * s);
int pushChar(CharStack * s, char v ,struct MEMMAN * mem);
char popChar(CharStack * s ,struct MEMMAN * mem);
char frontChar(CharStack * s);
void clearChar(CharStack * s ,struct MEMMAN * mem);

int getData(char expression[] ,int* beg);
int calculator(char expression[],struct MEMMAN * mem);
int beforePriority(char op);
int afterPriority(char op);
int result(IntStack * numStack,char op,struct MEMMAN * mem);
int getLength(const char * str);


#endif