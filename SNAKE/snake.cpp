#include<graphics.h>
#include<stdio.h>
#include<time.h>
typedef struct Snake
{
    int sx;
    int sy;
    struct Snake *next;
} Snake;
typedef struct Food
{
    int fx;
    int fy;
} Food;

int status=2;
int flag=1;
int ret=1;
int soc=0;
int head_d=2;
int HM=1;
FILE *fp;

void sign(int x,int y)
{
    setcolor(RED);
    line(x,y,x,y+30);
    line(x,y,x+120,y);
    line(x,y+30,x+120,y+30);
    line(x+120,y,x+120,y+30);
}


void welcome()
{
    setcolor(RED);
    setfontbkcolor(WHITE);
    setfont(30,0,"宋体");
    xyprintf(450,125,"普通模式");
    xyprintf(450,175,"困难模式");
    xyprintf(450,225,"炼狱模式");
    xyprintf(450,275,"查看帮助");
    line(450,155,570,155);
    line(450,205,570,205);
    line(450,255,570,255);
    line(450,305,570,305);
}

void Line()
{
    setcolor(BLUE);
    int i,j;
    for(i=0; i<=810; i+=30)
        line(i,0,i,600);
    for(j=0; j<=600; j+=30)
        line(0,j,810,j);
}

void creatfood(Food *food,Snake *snakehead)//创造食物
{
    Snake *p=(Snake *)malloc(sizeof(Snake));
    randomize();
    food->fx=random(700);
    food->fy=random(500);
    while(food->fx%30!=0)/*食物随机出现后必须让食物能够在整格内,这样才可以让蛇吃到*/
        food->fx++;
    while(food->fy%30!=0)
        food->fy++;
    for(p=snakehead; p!=NULL; p=p->next)
    {
        if(p->sx==food->fx&&p->sy==food->fy)
            creatfood(food,snakehead);
    }

}

void drawfood(int x,int y)//画食物
{
    setfillcolor(RED);
    bar(x,y,x+30,y+30);
}

Snake *creatsnake(Snake *snakehead,Snake *snakeend)//创造蛇
{
    snakehead->sx=300;
    snakehead->sy=300;
    snakeend->sx=270;
    snakeend->sy=300;
    return snakehead;
}

void drawsnake(Snake *snakehead)//画蛇
{
    setfillcolor(GREEN);
    Snake *p=(Snake *)malloc(sizeof(Snake));
    bar(snakehead->sx,snakehead->sy,snakehead->sx+29,snakehead->sy+29);
    for(p=snakehead->next; p!=NULL; p=p->next)
    {
        bar(p->sx,p->sy,p->sx+29,p->sy+29);
    }
}

void get_key()//获取键盘值
{
    char ch;
    while(kbhit())
    {
        ch=getch();
        switch(ch)
        {
        case 'a': //左
        {
            if(head_d==2) {}
            else
            {
                status = 1;
            }
            break;
        }
        case 'd':  //右
        {
            if(head_d==1) {}
            else
            {
                status = 2;
            }
            break;
        }
        case 'w':   //上
        {
            if(head_d==4) {}
            else
            {
                status = 3;
            }
            break;
        }
        case 's': //下
        {
            if(head_d==3) {}
            else
            {
                status = 4;
            }
            break;
        }
        }
    }
}
int mouse()
{
    int mx,my;
    mouse_msg msg= {0};
    msg = getmouse();
    mousepos(&mx,&my);
    if(mx>=450&&mx<=570&&my<=300&&my>=275)//帮助界面
    {
        sign(450,275);
        if (msg.is_left() && msg.is_down())
        {
            return 4;
        }
    }
    else if(mx>=450&&mx<=570&&my<=150&&my>=125)//普通
    {
        sign(450,125);
        if (msg.is_left() && msg.is_down())
        {
            return 1;
        }
    }
    else if(mx>=450&&mx<=570&&my<=200&&my>=175)//困难
    {
        sign(450,175);
        if (msg.is_left() && msg.is_down())
        {
            return 2;
        }
    }
    else if(mx>=450&&mx<=570&&my<=250&&my>=225)//炼狱
    {
        sign(450,225);
        if (msg.is_left() && msg.is_down())
        {
            return 3;
        }
    }
    else if(mx>=450&&mx<=570&&my<=300&&my>=275)//炼狱
    {
        sign(450,250);
        if (msg.is_left() && msg.is_down())
        {
            return 4;
        }
    }
    else
    {
        welcome();
        return 0;
    }
    return 0;
}

Snake *move(Snake *snakehead)
{
    Snake *pmove=(Snake *)malloc(sizeof(Snake));
    Snake *temp=(Snake *)malloc(sizeof(Snake));
    Snake *node=(Snake *)malloc(sizeof(Snake));
    pmove->next=snakehead;
    get_key();
    if(status==1)//左
    {
        pmove->sx=snakehead->sx-30;
        pmove->sy=snakehead->sy;
        head_d=1;
    }
    else if(status==2)//右
    {
        pmove->sx=snakehead->sx+30;
        pmove->sy=snakehead->sy;
        head_d=2;
    }
    else if(status==3)//上
    {
        pmove->sx=snakehead->sx;
        pmove->sy=snakehead->sy-30;
        head_d=3;
    }
    else if(status==4)//下
    {
        pmove->sx=snakehead->sx;
        pmove->sy=snakehead->sy+30;
        head_d=4;
    }
    snakehead=pmove;
    temp=snakehead;
    while(temp->next!=NULL)
    {
        node=temp;
        temp=temp->next;
    }
    free(temp);
    node->next=NULL;
    return snakehead;
}

Snake *judge_food(Food *food,Snake *snakehead)
{
    if(food->fx==snakehead->sx&&food->fy==snakehead->sy)
    {
        soc++;
        creatfood(food,snakehead); //初始化食物坐标
        drawfood(food->fx,food->fy);//画食物
        Snake *node=(Snake *)malloc(sizeof(Snake));
        if(status==4)
        {
            node->sx=snakehead->sx;
            node->sy=snakehead->sy+30;
        }
        else if(status==3)
        {
            node->sx=snakehead->sx;
            node->sy=snakehead->sy-30;
        }
        else if(status==2)
        {
            node->sx=snakehead->sx+30;
            node->sy=snakehead->sy;
        }
        else if(status==1)
        {
            node->sx=snakehead->sx-30;
            node->sy=snakehead->sy;
        }

        /*ode->next=snakehead->next;
        snakehead->next=node;*/
        node->next=snakehead;
        snakehead=node;
    }
    return snakehead;
}

void judge_bround(Snake *snakehead)
{
    Snake *p=(Snake *)malloc(sizeof(Snake));
    for(p=snakehead->next; p!=NULL; p=p->next)
    {
        if(snakehead->sx==p->sx&&snakehead->sy==p->sy)
            ret=0;
        else
            continue;
    }
    if(snakehead->sx<=800&&snakehead->sx>=0&&snakehead->sy>=0&&snakehead->sy<600)
        flag=1;
    else
        flag=0;
}

void desoc(int n)
{
    setfont(20,0,"宋体");
    setcolor(WHITE);
    setfillcolor(RED);
    xyprintf(0,0,"得分：%d",n);
}

void bkimage()
{
    PIMAGE pimg=newimage();
    getimage(pimg,"pimg.jpg");
    putimage(0,0,pimg);
    delimage(pimg);
}

void help_mouse()
{
    int mx,my;
    mouse_msg msg= {0};
    msg = getmouse();
    mousepos(&mx,&my);
    if(mx>=680&&mx<=760&&my<=340&&my>=300)//帮助界面
    {
        if (msg.is_left() && msg.is_down())
        {
            HM=0;
        }
    }
    else
        HM=1;
}


void help()
{
    setfont(40,0,"宋体");
    setfontbkcolor(WHITE);
    setcolor(RED);
    xyprintf(60,100,"玩家可以控制的小蛇去吃食物,每吃一个");
    xyprintf(60,150,"就能增加蛇身的长度,但是小蛇不能碰到");
    xyprintf(60,200,"墙壁和自己本身,另外小蛇也不能往回走");
    xyprintf(60,250,"玩家按下 W A S D 可以控制小蛇的移动");
}

void game1(int x)
{
    fp=fopen("soc.txt","a+");
    int k=0;
    setbkcolor(EGERGB(0,0,0));
    int nowsoc=0;
    Snake *snakehead=(Snake*)malloc(sizeof(Snake));
    Snake *snakeend=(Snake*)malloc(sizeof(Snake));
    Food *food=(Food*)malloc(sizeof(Food));
    snakehead->next=snakeend;
    snakeend->next=NULL;
    creatsnake(snakehead,snakeend);//初始化蛇坐标
    creatfood(food,snakehead);//初始化食物坐标
    while(flag==ret)
    {
        cleardevice();
        Line();
        desoc(soc);
      //  get_key();//获取键盘值
        drawsnake(snakehead);//画蛇
        drawfood(food->fx,food->fy);
        snakehead=move(snakehead);//上下左右移动30
        snakehead=judge_food(food,snakehead);//判断是否碰到边界和吃到食物
        judge_bround(snakehead);
        delay_fps(x);
    }
    cleardevice();
    fscanf(fp,"%d",&nowsoc);
    setfont(40,0,"宋体");
    setfontbkcolor(WHITE);
    setcolor(RED);
    if(nowsoc>soc)
    {
        bkimage();
        xyprintf(280,200,"历史最高分：%d",nowsoc);
        xyprintf(280,270,"当前总得分：%2d",soc);
        xyprintf(420,340,"esc退出");
    }
    else
    {
        bkimage();
        fclose(fp);
        remove("soc.txt");
        fp=fopen("soc.txt","w+");
        fprintf(fp,"%d",soc);
        xyprintf(280,200,"恭喜你打破历史：%d",soc);
        xyprintf(420,350,"esc退出");
    }
    fclose(fp);
    soc=0;
    flag=1;
    ret=1;
    while(1)
    {
        k =getch();
        if(k==27)
            break;
        else
            continue;
    }

}

int main()
{
    initgraph(810,600,0);
    setcaption("贪吃蛇");
    PIMAGE pimg=newimage();
    getimage(pimg,"pimg.jpg");
    int m;
    while(1)
    {
        cleardevice();
        putimage(0,0,pimg);
        welcome();//界面
        while(1)//获取鼠标
        {
            m=mouse();
            if(m==1||m==2||m==3||m==4)
                break;
            else
                continue;
        }
        switch(m)
        {
        case 1://普通模式
            game1(6);
            status=2;
            break;
        case 2://困难模式
            game1(9);
            status=2;
            break;
        case 3://炼狱模式
            game1(12);
            status=2;
            break;
        case 4://查看帮助
            cleardevice();
            putimage(0,0,pimg);
            setfont(40,0,"宋体");
            setfontbkcolor(YELLOW);
            xyprintf(680,300,"返回");
            while(HM)
            {
                help_mouse();
                help();
            }
            HM=1;
            break;
        }
    }
    delimage(pimg);
    getch();
    closegraph();
    return 0;
}
