#include "mem_manage.h"

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
