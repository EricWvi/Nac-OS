// void api_putchar(int c);

// void main() {
// //    api_putchar('C');
//     char *p = (char*)0x123;
//     p[0] = 'c';
//     p[1] = 'r';
//     p[2] = 'a';
//     p[3] = 'c';
//     p[4] = 'k';
//     p[5] = 0;
// }
void api_putchar(int c);
void api_putstr(char *s);
int  api_openwin(char *buf, int xsiz, int ysiz, int col_inv, char *title);
void api_putstrwin(int win, int x, int y, int col, int len, char *str);
void api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
void api_point(int win, int x, int y, int col);
void api_refreshwin(int win, int x0, int y0, int x1, int y1);
void api_linewin(int win, int x0, int y0, int x1, int y1, int col);
void api_closewin(int win);
int  api_getkey(int mode);
void api_showstring();
int  api_fopen(char *fname);
int  api_fread(char *buf, int maxsize, int fh);
void api_fseek(int fhandle, int offset, int mode);

int  api_alloctimer();
void api_inittimer(int timer, int data);
void api_settimer(int timer, int time);
void api_freetimer(int timer);

#define win_width 240
#define win_height 240
#define gezisize 39

int gezi_toX(int gezi){
    int x=0;
    if(gezi%3==0){
        x=40;
    }
    if(gezi%3==1){
        x=120;
    }
    if(gezi%3==2){
        x=200;
    }
    return x;
}
int gezi_toY(int gezi){
    if(gezi<3){
        return 40;
    }
    if(gezi<6){
        return 120;
    }
    return 200;

}


void waitting(int i, int timer, char* keyflag) {
    int j;
    if (i > 0) {
        api_settimer(timer, i);
        i = 128;
    } else {
        i = 0x1c;
    }
    for(;;) {
        j = api_getkey(1);
        if (i == j) {
            break;
        }

        // if (j == 0x17) 
        //     keyflag[0] = 1;
        if (j == 0x24) 
            keyflag[0] = 1;
        if (j == 0x25)
            keyflag[1] = 1;
        // if (j == 0x26)
        //     keyflag[3] = 1;
        if (j == 0x1C) 
            keyflag[2] = 1;
        if (j == 0x39) 
            keyflag[3] = 1;
    
        // j = api_getkey(1);
        // if (j == 0x10)//key q
        //     keyflag[0] = 1;
        // if (j == 0x11)//key w
        //     keyflag[1] = 1;
        // if (j == 0x12) //key e
        //     keyflag[2] = 1;
        // if(j==0x1E)//a
        //     keyflag[3]=1;
        // if(j==0x1F)//S
        //     keyflag[4]=1;
        // if(j==0x20)//D
        //     keyflag[5]=1;
        // if(j==0x2C)//Z
        //     keyflag[6]=1;
        // if(j==0x2D)//X
        //     keyflag[7]=1;
        // if(j==0x2E)//C
        //     keyflag[8]=1;
        // if(j==0x1C)//enter
        //     keyflag[9]=1;
        // if(j==0x39)//kongge
        //     keyflag[10]=1;
    }
    return;
}

// int choose(int win,int gezi,int nagegezi,char* keyflag){
//     int a=gezi_toX(gezi);
//     int b=gezi_toY(gezi);
//     if(gezi%2==0){
//         api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,0);
//     }else{
//         api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,4);
//     }
//     gezi=nagegezi;
//     a=gezi_toX(gezi);
//     b=gezi_toY(gezi);
//     api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,5);
//     api_refreshwin(win,  0, 0, win_width-1, win_height-1);
//     keyflag[nagegezi] =0;
//     return gezi;
// }
void main() {
    int gezi=4,turn=0,win=0,winner=0;
    int i, j, timer;
    char winbuf[win_width * win_height],keyflag[20],who[9];
    for(i=0;i<9;i++){
        who[i]='#';
    }
    for(i=0;i<20;i++){
        keyflag[i] = 0;
    }
    win = api_openwin(winbuf, win_width, win_height, -1, "invader");
    timer = api_alloctimer();
    api_inittimer(timer, 128);

    api_boxfilwin(win, 0, 0, 80, 80, 0);api_boxfilwin(win, 80, 80, 160, 160, 5);
    api_boxfilwin(win, 160, 160, 239, 239, 0);api_boxfilwin(win, 0, 160, 80, 239, 0);api_boxfilwin(win, 160, 0, 239, 80, 0);
    api_boxfilwin(win, 80, 0, 160, 80, 4);api_boxfilwin(win, 0, 80, 80, 160, 4);api_boxfilwin(win, 160, 80, 239, 160, 4);
    api_boxfilwin(win, 80, 160, 160, 239, 4);

    for(;;) {
        if(who[0]=='x'&&who[1]=='x'&&who[2]=='x'){winner=1;break;}
        if(who[0]=='x'&&who[3]=='x'&&who[6]=='x'){winner=1;break;}
        if(who[2]=='x'&&who[5]=='x'&&who[8]=='x'){winner=1;break;}
        if(who[6]=='x'&&who[7]=='x'&&who[8]=='x'){winner=1;break;}
        if(who[6]=='x'&&who[4]=='x'&&who[2]=='x'){winner=1;break;}
        if(who[0]=='x'&&who[4]=='x'&&who[8]=='x'){winner=1;break;}
        if(who[0]=='o'&&who[1]=='o'&&who[2]=='o'){winner=2;break;}
        if(who[0]=='o'&&who[3]=='o'&&who[6]=='o'){winner=2;break;}
        if(who[2]=='o'&&who[5]=='o'&&who[8]=='o'){winner=2;break;}
        if(who[6]=='o'&&who[7]=='o'&&who[8]=='o'){winner=2;break;}
        if(who[6]=='o'&&who[4]=='o'&&who[2]=='o'){winner=2;break;}
        if(who[0]=='o'&&who[4]=='o'&&who[8]=='o'){winner=2;break;}

        waitting(4, timer, keyflag);
        // if(turn%2==1){//AI's turn
        //     char s[1]={'o'};
        //     if(who[6]=='#'){
        //         who[6]=='o';
        //         api_putstrwin(win,40,200,7,1,s);
        //     }else if(who[7]=='#'){
        //         who[7]=='o';
        //         api_putstrwin(win,120,200,7,1,s);
        //     }
        //     turn++;
        // }
        // if(turn%2==0){
        // for(i=0;i<9;i++){
            if (keyflag[0] != 0&&gezi>0) {
                int a=gezi_toX(gezi);
                int b=gezi_toY(gezi);
                if(gezi%2==0){
                    api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,0);
                    if(who[gezi]=='x'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);
                    }else if(who[gezi]=='o'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);
                    }
                }else{
                    api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,4);
                    if(who[gezi]=='x'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);//x
                    }else if(who[gezi]=='o'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);//o
                    }
                }
                gezi=gezi-1;
                a=gezi_toX(gezi);
                b=gezi_toY(gezi);
                api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,5);
                api_refreshwin(win,  0, 0, win_width-1, win_height-1);
                keyflag[0] =0;
            }else if(keyflag[1]!=0&&gezi<8){
                int a=gezi_toX(gezi);
                int b=gezi_toY(gezi);
                if(gezi%2==0){
                    api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,0);
                    if(who[gezi]=='x'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);
                    }else if(who[gezi]=='o'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);
                    }
                }else{
                    api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,4);
                    if(who[gezi]=='x'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);//x
                    }else if(who[gezi]=='o'){
                        api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);//o
                    }
                }
                gezi=gezi+1;
                a=gezi_toX(gezi);
                b=gezi_toY(gezi);
                api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,5);
                api_refreshwin(win,  0, 0, win_width-1, win_height-1);
                keyflag[1] =0;}
            // }else if(keyflag[2]!=0&&gezi>0){
            //     int a=gezi_toX(gezi);
            //     int b=gezi_toY(gezi);
            //     if(gezi%2==0){
            //         api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,0);
            //         if(who[gezi]=='x'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);
            //         }else if(who[gezi]=='o'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);
            //         }
            //     }else{
            //         api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,4);
            //         if(who[gezi]=='x'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);//x
            //         }else if(who[gezi]=='o'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);//o
            //         }
            //     }
            //     gezi=gezi-1;
            //     a=gezi_toX(gezi);
            //     b=gezi_toY(gezi);
            //     api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,5);
            //     api_refreshwin(win,  0, 0, win_width-1, win_height-1);
            //     keyflag[2] =0;
            // }else if(keyflag[3]!=0&&gezi<8){
            //     int a=gezi_toX(gezi);
            //     int b=gezi_toY(gezi);
            //     if(gezi%2==0){
            //         api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,0);
            //         if(who[gezi]=='x'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);
            //         }else if(who[gezi]=='o'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);
            //         }
            //     }else{
            //         api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,4);
            //         if(who[gezi]=='x'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);//x
            //         }else if(who[gezi]=='o'){
            //             api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);//o
            //         }
            //     }
            //     gezi=gezi+1;
            //     a=gezi_toX(gezi);
            //     b=gezi_toY(gezi);
            //     api_boxfilwin(win,a-gezisize,b-gezisize,a+gezisize,b+gezisize,5);
            //     api_refreshwin(win,  0, 0, win_width-1, win_height-1);
            //     keyflag[3] =0;
            // }
        // }
        if (keyflag[2]!=0)
            break;
        if(keyflag[3]!=0&&who[gezi]=='#'){
            if(turn%2==0){
                int a=gezi_toX(gezi);
                int b=gezi_toY(gezi);
                // char sa[1]={'x'};
                // api_putstrwin(win,a,b,7,1,sa);
                api_boxfilwin(win, a-10, b-10, a+10, b+10, 6);
                who[gezi]='x';
                keyflag[3]=0;
                turn++;
            }else{
                int a=gezi_toX(gezi);
                int b=gezi_toY(gezi);
                // char sa[1]={'x'};
                // api_putstrwin(win,a,b,7,1,sa);
                api_boxfilwin(win, a-10, b-10, a+10, b+10, 3);
                who[gezi]='o';
                keyflag[3]=0;
                turn++;
            } 
        }
    }
    
    if(winner==1){
        // api_boxfilwin(win,10,10,90,90,7);
        char winner[12] = {'G','A','M','E','W','I','N','N','E','R','A'};
        api_putstrwin(win, 100, 100, 1,11, winner);
        waitting(100, timer, keyflag);
    }else if(winner==2){
        // api_boxfilwin(win,10,10,90,90,7);
        char winner[12] = {'G','A','M','E','W','I','N','N','E','R','B'};
        api_putstrwin(win, 100, 100, 1,11, winner);
        waitting(100, timer, keyflag);
    }
    for(;;){
        waitting(4, timer, keyflag);
        if(keyflag[2]!=0){
            break;
        }
    }
    api_closewin(win);
    return;
}






