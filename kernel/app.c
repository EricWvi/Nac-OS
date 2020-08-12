
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
#define win_height 260
#define gezisize 20/2
#define dise 6
#define bai 7
int gezi_toX(int gezi){
    int a=gezi%12;
    return 20*a+9;
}
int gezi_toY(int gezi){
    int a=gezi/12;
    return 20+20*a+9;
}
void draw_digit(){
    
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
        if (j == 0x17) 
            keyflag[0] = 1;
        if (j == 0x24) 
            keyflag[2] = 1;
        if (j == 0x25)
            keyflag[1] = 1;
        if (j == 0x26)
            keyflag[3] = 1;
        if (j == 0x1C) 
            keyflag[9] = 1;
        if (j == 0x39) 
            keyflag[10] = 1;
    }
    return;
}
void draw_ge(int win,int ge,int col){
    int a=gezi_toX(ge);
    int b=gezi_toY(ge);
    api_boxfilwin(win,a-9,b-9,a+9,b+9,col);
}
void draw_she(int win,int* she,int lon,int col){
    int i=0;
    for(i=0;i<=lon;i++)
        draw_ge(win,she[i],col);
}
void main() {
    int gezi=4,turn=0,win=0,winner=0,eat=0,i, j, timer;
    char winbuf[win_width * win_height],keyflag[20];
    int she[20]={0,12,24,36};//0 is shewei 3 is sheto   
    int lon=3;
    int ranx[8]={3,6,4,7,11,4,8,9};
    int rany[8]={1,9,4,7,0,2,10,6};
    int foodg[10]={14,67,33,89,57,11,112,133,9,66};
    for(i=0;i<20;i++){
        keyflag[i] = 0;
    }
    win = api_openwin(winbuf, win_width, win_height, -1, "sss");
    timer = api_alloctimer();
    api_inittimer(timer, 128);
    api_boxfilwin(win,0,20,239,259,dise);//dise 1 1 isred useto boom
    for(i=19;i<239;i+=20){
        api_linewin(win,i,20,i,259,0);
    }
    for(j=39;j<259;j+=20){
        api_linewin(win,0,j,239,j,0);
    }
    int cnt=0;
    int fg=6;
    draw_ge(win,fg,1);
    for(;;) {
        waitting(4, timer, keyflag);
        if(eat){fg=foodg[cnt%10];cnt++;eat=0;draw_ge(win,fg,1);}
        if (keyflag[0] != 0&&she[lon]>=12) {//shetou
            if(she[lon]-12==fg){
                lon++;if(lon==7){winner=1;break;}
                she[lon]=fg;
                draw_she(win,she,lon,bai);
                eat=1;
            }else{draw_she(win,she,lon,dise);
                for(i=0;i<lon;i++)she[i]=she[i+1];
                she[lon]-=12;
                draw_she(win,she,lon,bai);
                keyflag[0] =0;
            }
        }else if (keyflag[1] != 0&&gezi<12*11) {
            if(she[lon]+12==fg){
                lon++;if(lon==7){winner=1;break;}
                she[lon]=fg;
                draw_she(win,she,lon,bai);eat=1;
            }else{draw_she(win,she,lon,dise);
                for(i=0;i<lon;i++)she[i]=she[i+1];
                she[lon]+=12;
                draw_she(win,she,lon,bai);
                keyflag[1] =0;
            }
        }else if (keyflag[2] != 0&&she[lon]%12!=0) {
            if(she[lon]-1==fg){
                lon++;if(lon==7){winner=1;break;}
                she[lon]=fg;
                draw_she(win,she,lon,bai);eat=1;
            }else{draw_she(win,she,lon,dise);
                for(i=0;i<lon;i++)she[i]=she[i+1];
                she[lon]-=1;
                draw_she(win,she,lon,bai);
                keyflag[2] =0;}
        }else if (keyflag[3] != 0&&she[lon]%12!=11) {
            if(she[lon]+1==fg){
                lon++;if(lon==7){winner=1;break;}
                she[lon]=fg;
                draw_she(win,she,lon,bai);eat=1;
            }else{
                draw_she(win,she,lon,dise);
                for(i=0;i<lon;i++)she[i]=she[i+1];
                she[lon]+=1;
                draw_she(win,she,lon,bai);
                keyflag[3] =0;}
        }
        if (keyflag[9]!=0)
            break;
        if(keyflag[10]!=0){
            break;
        }
    }
    
    if(winner==1){
        api_boxfilwin(win,10,10,90,90,7);
        char winner[12] = {'G','A','M','E','W','I','N','N','E','R','A'};
        api_putstrwin(win, 100, 100, 1,11, winner);
        waitting(100, timer, keyflag);
    }
    for(;;){
        waitting(4, timer, keyflag);
        if(keyflag[9]!=0){
            break;
        }
    }
    api_closewin(win);
    return;
}






