#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>

using namespace std;

/*

NOTES

FLY:
ASCII characters have broken free and are in a rush.You are caught in between.Can you survive?


-  'PLAYER' VARIABLE IS USED TO CONTROL ONLY Y COORDINATE
-  X COORDINATE OF PLAYER IS FIXED
-  Y COORDINATE OF OBSTACLE IS FIXED

TO-DO

-   INTRO STUFF
-   ENDING STUFF

*/

//CLASSES

class obstacle
{
      public:
         char ch;
         short x;
         short y; 
         bool exist;  
         
         obstacle(void)
         {
            exist=false;
    	}
};

//COPIED FUNCTIONS

void gotoxy(short x, short y)
{
	COORD coord={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void setcursor(bool visible,DWORD size)
{
	HANDLE console=GetStdHandle(STD_OUTPUT_HANDLE);
	if(size==0)
	{
		size=20;
	}
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible=visible;
	lpCursor.dwSize=size;
	SetConsoleCursorInfo(console,&lpCursor);
}

//GLOBAL VARIABLES

int bg_speed=150;
unsigned short player=5, rot=1;
const short x=10;
char dot=char(21);
bool bg_init_var=true, play=true;
clock_t bg_t2;

obstacle obarray[9];
void game(void);

//MADE FUNCTIONS

void border(bool mid=true)
{
	//TO DISPLAY THE BORDER AND FILL BOARD
	
	int m;
	char sym=char(462);

	//BOTTOM
	for(m=0;m<79;++m)
	{
		gotoxy(m,24);
		cout<<sym;
	}

	//LEFT
	for(m=0;m<=23;++m)
	{
		gotoxy(0,m);
		cout<<sym;
	}

	//RIGHT
	for(m=0;m<=24;++m)
	{
		gotoxy(79,m);
		cout<<sym;
	}

	//TOP
	for(m=0;m<79;++m)
	{
		gotoxy(m,0);
		cout<<sym;
	}

	//MIDDLE
	if(mid)
	for(m=0;m<24;++m)
	{
		gotoxy(61,m);
		cout<<sym;
		gotoxy(62,m);
		cout<<sym;
	}
}

void instructions(void)
{
	//INSTRUCTIONS
	
    border(0);
	gotoxy(35,7);
	cout<<"Instructions";
	gotoxy(20,11);
	cout<<"- Use space bar to move up. Press once to hover.";
	gotoxy(20,12);
	cout<<"- Press and hold to shoot up (not stabilising).";
	gotoxy(20,13);
	cout<<"- Avoid using 'hover' and 'shoot up' in succesion.";
}

void freeze(void)
{
	//FOR PAUSE
	
    gotoxy(70,11);
 	cout<<"   ";
	gotoxy(67,12);
	cout<<"        ";

	gotoxy(68,12);
	cout<<"PAUSE";
	gotoxy(66,13);
	cout<<"EXIT(Y/N)";
	
	char inp;
	bool run=true;
	
	//TO TAKE ONLY Y/N VALUES
	while(run)
	{
		inp=getch();
			
		if(inp=='n'||inp=='N')
		{
			run=false;
            gotoxy(68,12);
			cout<<"     ";
			gotoxy(66,13);
			cout<<"         ";

			gotoxy(70,11);
 			cout<<"GO,";
			gotoxy(67,12);
			cout<<"SURVIVE!";
		}
		
		if(inp=='y'||inp=='Y')
		{
			play=false;
			run=false;
		}
	}
}

void assign_colour(void)
{
	//TO COLOURISE THE NEXT OUTPUT
	
    short num=rand()%3;

	switch(num)
	{
		case 0:
       		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			   FOREGROUND_GREEN|FOREGROUND_INTENSITY);
       		break;

		case 1:
       		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			   FOREGROUND_RED|FOREGROUND_INTENSITY);
       		break;

		case 2:
       		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			   FOREGROUND_BLUE|FOREGROUND_INTENSITY);
			break;
	}
}

void collide(void)
{
	//TO CHECK COLLISION
	
	bool wall=false;
	
	if(player==23)
	{
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			   FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
			   
        gotoxy(70,11);
 		cout<<"   ";
		gotoxy(67,12);
		cout<<"        ";
		gotoxy(66,11);
		cout<<"         ";
			   
		gotoxy(65,10);
		cout<<"HIT THE WALL";
		wall=true;
	}
	
	for(int p=0;p<9;++p)
	{
		if(((obarray[p].x==x)&&(obarray[p].y==player))||(wall))
		{
			if(!(wall))
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				   FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE);
				   
                gotoxy(70,11);
 				cout<<"   ";
				gotoxy(67,12);
				cout<<"        ";
				gotoxy(66,11);
				cout<<"         ";
				   
				gotoxy(66,10);
				cout<<"HIT BY '"<<obarray[p].ch<<"'";
			}
			
			for(int m=0;m<9;++m)
			obarray[m].exist=false;
			
			play=false;
			wall=true;
			char ch=' ';
			
			gotoxy(66,13);
			cout<<"PLAY AGAIN?";
			gotoxy(68,14);
			cout<<"(Y/N)";
			
			//PREVENT ACCIDENTAL INPUT AS ' '
			while(ch==' ')
			{
				ch=getch();
			
				if(ch=='y'||ch=='Y')
				{
					play=true;
					bg_speed=150;
					
					system("cls");
					border();
					
					for(int i=0;i<9;++i)
					{
						obarray[i].exist=false;
						obarray[i].ch=' ';
						obarray[i].x=obarray[i].y=0;
					}
					game();
				}
			
				else if(!(ch==' '))
				break;
			}

			break;
		}
	}
}

void shift_array(int index)
{
	//TO SHIFT OBARRAY TO INCLUDE NEW OSTACLES
	
	int rno1, rno2;
	
    //FOR A RANDOM Y COORDINATE
    rno1=(rand()%22)+1;

	//FOR A RANDOM ASCII CHAR
     rno2=rand()%250;

    //TO AVOID BLANK SPACE ASCII VALUES
    while((rno2==0)||(rno2==7)||(rno2==8)||(rno2==9)||(rno2==10)
    ||(rno2==13)||(rno2==32)) rno2=rand()%250;
     
	for(int k=index;k<9;++k)
    {
		if(k<8)
		{
			obarray[k].ch=obarray[k+1].ch;
			obarray[k].x=obarray[k+1].x;
			obarray[k].y=obarray[k+1].y;
		}
		
		if(k==8)
		{
            obarray[k].ch=char(rno2);
			obarray[k].x=60;
			obarray[k].y=rno1;
		}
    }
}

void bg_init(void)
{
	//INITIALISE BACKGROUND
	
     int rno1, rno2;
     
     srand(time(NULL));
     
     //FOR INITIAL X COORDINATES FROM 26 WITH A SPACING OF 7
     for(int i=26,k=0;i<60;i+=7,++k)
     {
		   //FOR A RANDOM Y COORDINATE
           rno1=(rand()%22)+1;
           
           //FOR A RANDOM ASCII CHAR
           rno2=rand()%250;
           
           //TO AVOID BLANK SPACE ASCII VALUES
           while((rno2==0)||(rno2==7)||(rno2==8)||(rno2==9)||(rno2==10)
           ||(rno2==13)||(rno2==32))
           rno2=rand()%250;
           
           //STORING OBSTACLES IN ARRAY
               obarray[k].ch=char(rno2);
               obarray[k].x=i;
               obarray[k].y=rno1;
			   obarray[k].exist=true;
           
           //DISPLAY OBSTACLES
           gotoxy(i,rno1);
		   assign_colour();
           cout<<char(rno2); 
     }        
}

void bg_update(void)
{
	//TO UPDATE BACKGROUND
	
	int rno1, rno2;

    if((bg_init_var)||((clock()-bg_t2)>=bg_speed))
    {
        for(int i=0;i<9;++i)
        {
            if(obarray[i].exist)
            {
				//ERASING
				gotoxy(obarray[i].x,obarray[i].y);
				cout<<" ";

				if((obarray[i].x>0)&&(obarray[i].x<61))
				{
					//UPDATING
					obarray[i].x--;
					gotoxy(obarray[i].x,obarray[i].y);
					assign_colour();
					cout<<obarray[i].ch;
				}
				
				if(obarray[i].x==1)
				{
                    gotoxy(obarray[i].x,obarray[i].y);
					cout<<" ";
					shift_array(i);
				}
            }

			else if(!(obarray[i].exist))
			{
				if(obarray[i-1].exist)
				if(obarray[i-1].x<54)
				{
                    //FOR A RANDOM Y COORDINATE
           			rno1=(rand()%22)+1;
           			
					//FOR A RANDOM ASCII CHAR
           			rno2=rand()%250;

           			//TO AVOID BLANK SPACE ASCII VALUES
          			while((rno2==0)||(rno2==7)||(rno2==8)||(rno2==9)||(rno2==10)
          			||(rno2==13)||(rno2==32)) rno2=rand()%250;
          			
          			obarray[i].exist=true;
					obarray[i].ch=char(rno2);
					obarray[i].x=61;
					obarray[i].y=rno1;
					
					obarray[i].x--;
					gotoxy(obarray[i].x,obarray[i].y);
					assign_colour();
					cout<<obarray[i].ch;
				}
			}
        }

        bg_t2=clock();
                     
        if(bg_init_var)
        bg_init_var=false;
    }

	collide();
}

void disp_player(void)
{
	//TO MAKE PLAYER CHAR ROTATE
	
	switch(rot)
	{
		case 1:
			dot=char(30);
			++rot;
			break;
			
		case 2:
            dot=char(16);
            ++rot;
			break;
			
		case 3:
            dot=char(31);
            ++rot;
			break;
			
		case 4:
            dot=char(17);
            rot=1;
			break;
	}
	
	gotoxy(x,player);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
	        FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);
	cout<<dot;
	bg_update();
}

void game(void)
{
	//THE GAME'S MAIN()
	char inp;
	bool init=true, timecheck=true;
	int fall_speed=80;
	clock_t t2=0, t3=0, start=0, pause=0, resume=0, totalpause=0;
	
	start=clock();
	player=5;
	
    //FIRST TIME DISPLAY
    setcursor(0,0);
    gotoxy(67,3);
	cout<<"FLY V1.0";
	gotoxy(70,11);
 	cout<<"GO,";
	gotoxy(67,12);
	cout<<"SURVIVE!";
	gotoxy(68,19);
    cout<<"TIME";
	bg_init();

	//GAME LOOP
	while(play)
	{
        bg_update();
        
        gotoxy(67,20);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			    FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);
		cout<<float((clock()-start-totalpause)/1000.0);
		
		int time=(float(clock()-start-totalpause))/1000;
		
		//FOR MAD RUSH TIME
		if(time>40)
		{
            gotoxy(70,11);
 			cout<<"   ";
			gotoxy(67,12);
			cout<<"        ";
			
			gotoxy(66,11);
			cout<<"MAD RUSH!";
			
			bg_speed=10;
		}

		//INCREMENT SPEED
		if((time!=0)&&(time<40)&&(time%2==0)&&(timecheck))
		{
			bg_speed-=7;
			timecheck=false;
		}
		
		//TO INCREMENT ONLY ONCE A SECOND
		if(!(time%2==0))
		timecheck=true;
        
		if(!kbhit())
		{
			if(player<23)
			if((init)||((clock()-t2)>=fall_speed))
			{
                gotoxy(x,player);
				cout<<" ";
				player+=1;
				disp_player();

                t2=clock();

				if(init)
				init=false;
			}
		}//END OF IF-!KBHIT()

		//FIRST KBHIT()
		if(kbhit())
		{
			t3=clock();
			
			inp=getch();
			
				if(inp=='p'||inp=='P')
				{
					pause=clock();
					freeze();
					resume=clock();
					totalpause+=(resume-pause);
				}
			
			//LOOP TO WAIT 0.4 SECONDS FOR NEXT INPUT(STABILIZING)
			while((clock()-t3)<444)
			{
				bg_update();

				//SECOND KBHIT()
                if(kbhit())
                {
					inp=getch();

					if(inp==' ')
					if(player>2)
					{
            		    gotoxy(x,player);
						cout<<" ";
						player-=2;
						dot=char(30);
						gotoxy(x,player);
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							   FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED);
						cout<<dot;
					}
                }//END OF SECOND IF-KBHIT()
			}//END OF WAITING WHILE LOOP
		}//END OF FIRST IF-KBHIT()
	}//END OF GAME LOOP
}//END OF GAME()

int main(void)
{
	border();
	game();

	return 0;
}
