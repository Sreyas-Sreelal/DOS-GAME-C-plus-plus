#include<fstream.h> //for file handling (saving score name and password)
#include<conio.h>//for getch,kbhit textcolor and text background
#include<stdlib.h>//for rand() functions
#include<process.h>//for exit() function
#include<stdio.h>//for gets and other functions
#include<string.h>//for string functions strlen,strcmp etc
#include<dos.h>//for delay functions

char name[24],password[12];//vars for saving temp inputted name and password
int logined=0,//for checking whether player logined
score=0,//temp var to hold score in current section
data_size,//var to get number of objects saved in file
pindex;//temp bvar holding index of player's data in object of the class


char dir = '0'; //the variable holding the direction of in game object

class game_data//class containing game datas
{
 int x,y,cx,cy,width,height;//position variables that will hold position of player and game area

 public:

 int gameover;//var to check the game is over or not

 void controller();//function for getting player inputs to move the game object
 void update();//function that will update game screen on each events in the game
 void load_collectibles();//functions to load collectibles or pickups
 void move();//the function that moves game object according to controller()

 game_data()//constructor
 {
 /*Resetting vars to default*/
 width    = 50;
 height   = 20;
 x        = width/2;
 y        = height/2;

 /*x and y will be half of width and height so that to center the object*/
 cx       = (random(width-1)+1);
 cy       = (random(height-1)+1);

 /*random coordinates for pickups*/

 gameover = 0;//game @ the begging will not be over

 }
};

class player_data//class holding player details and saving functions
{
 char name[24],password[12];
 int pscore;//variable to hold player score
 public:

 player_data()//constructor
 {
 pscore = 0;
 }

 void input()//input function to get player details
 {
 cout<<"Input your name ";
 gets(name);
 cout<<"Input password ";
 gets(password);
 }

 void display()//display function to display details
 {
   cout<<"\nName: "<<name;
   cout<<"\nScore: "<<pscore;
 }

 void Chart()//another display function that will help to print top 10 in chart
 {
   cout<<"\t\t"<<name
       <<"\t"<<pscore<<endl;
 }

 char *getname()//accesor to get name of the player
 {
  return name;
 }

 int checkpass(char pass[])//function to check whether the password matches
 {
  return strcmp(pass,password);
 }

 char *getpass()//accessor function to get password (used only in temp objects)
 {
 return password;
 }

 int getscore()//accessor function to get score
 {
  return pscore;
 }

 void  load_score()//function loads the score from the data base and intialise to global var
 {
  score = pscore;
 }

 void save_score()//global var that holds updated score is assigned back to player score variable
 {
 pscore = score;
 }
}
s[25]/*global object contains all details of registerd players*/,
temp/*object holds temporary player data*/;


void game_data::controller()
{
 if(kbhit())//gets key input
 {
  switch(getch())//using getch to indentify inputted key
  {
   /*Setting direction according to input*/

   case 'w':
   dir = 'U';//Upward
   break;

   case 's':
   dir = 'D';//Downward
   break;

   case 'a':
   dir = 'L';//Left
   break;

   case 'd':
   dir = 'R';//Right
   break;
   /*Key to close the game*/
   case 'x':
   gameover = 1;//this will break the main loop
   break;

  }
 }

 if(x==width||y==height||x==0||y==0)//if the game object hits the wall
   gameover = 1;

 else
 if(x==cx&&y==cy)//if the game object collects pickups
 {
 score+=10;
 load_collectibles();//load new pickup with random coords
 }
}

void game_data::move()
{
   switch(dir)//processnig movements
   {
   case 'U':
   y--;
   update();
   break;

   case 'D':
   y++;
   update();
   break;

   case 'L':
   x--;
   update();
   break;

   case 'R':
   x++;
   update();
   break;
  }
 }

void game_data::update()
{
 clrscr();
 int i,j;
 cout<<"\t\t\tScore : "<<score<<endl;//prints score top left
 /*looping through the coordinates of game area*/
 for(i=0;i<=height;i++)
 {
  for(j=0;j<=width;j++)
  {

   if(i==y&&j==x)//if the coords of player object reached
   cout<<"";//player object

   else
   if(i==0||i==height||j==0||j==width)//sets walls on borders
   cout<<"#";//wall object
   else
   if(cy==i&&cx==j)//id reached pickups coordinates
   cout<<"*";//pickup object
   else
   cout<<" ";//space on inside of the game area
  }
  cout<<endl;
 }

}

void game_data::load_collectibles()
{
 /*Sets the random coordinates to pickups (min 0 max will be height-1 and width-1)*/
 cx = random(width-1)+1;
 cy = random(height-1)+1;
}
/*Global Functions*/

void Login();//to login the player
void Register();//to register new player
void save();//save the details to database
void connect();//connect to database and get all main objects of player class intialised with registerd player's details
void Search_Name();//function to search by player name
void Search_Score();//function to search by player score
void menu();//function handling menu
void Top10();//function to sort the objects acording to their scoer and to display top 10 of them

void save()
{
 ofstream file("data.dat",ios::out);
 for(int i=0;i<data_size;i++)
 {
  file.write((char*)&s[i],sizeof(s[i]));//write the details of updated player class in all
 }
 file.close();
}

void connect()
{

 fstream file("data.dat",ios::in|ios::out|ios::app);
 file.seekg(0);
 int i = 0;//to get no of registered player

 while(!file.eof())
 {
 file.read((char*)&s[i],sizeof(s[i]));//initialising player class objects with registered players
 if(file.eof())break;
 i++;
 }
  data_size = i;//setting the data size with no of player registered
  file.close();
}
void Register()
{
 int i;
 temp.input();
 int found = 0;//to check whether the name exists in the database
 for(i=0;i<data_size;i++)
 {

  if(strcmp(s[i].getname(),temp.getname())==0)//if the name exists
   {
    found = 1;
    break;
   }
  }
 if(found)
 {
 cout<<"\nThis name exits you will be  now redirected to login section\n ";
 delay(2000);
 Login();//calling function login so as to redirect to login area
 }
 else//new account
 {
 s[data_size] = temp;
 pindex = data_size;
 data_size++;//incrementing no of registered players
 cout<<"\nYour Account registered successfully !!!\n";
 logined=1;//player logged in
 }
 }


void Login()
{

 int found=0/*same use as in register function*/,i=0;
 temp.input();
 for(i=0;i<data_size;i++)
 {
 if(strcmp(temp.getname(),s[i].getname())==0)//inputted name exists
 {
 found =1;
 if(s[i].checkpass(temp.getpass())==0)//checking password
 {
 logined = 1;
 temp = s[i];
 cout<<"\nYou logined succesfully!!\n";
 pindex = i;
 break;
 }

 else//if wrong password entered
 {
 cout<<"\nYou entered wrong password Enter it again \n";
 break;
 }
 }
 }

 if(found == 0)//if not found and needed to be registered
 {
 cout<<"\nThis account doesnt exists Please register \n";
 delay(100);
 Register();//calling Register function to redirect to register page
 }
 else
 {
 if(!logined)//player not logined (incorrect password)
 {
 delay(100);
 Login();//calling login screen again
 }
 }
 }

 void menu()
 {
 clrscr();

 int res/*to get response in the main menu*/,res2/*response in search menu*/;
 cout<<"\t\t\tASCII MAN  - A C++ GAME \n\t\t\t\t......\n\t\t\t\t......\n\t\t\t\t......\n\n\n";
 cout<<"\t\t|||||Main Menu||||||\n\n";
 cout<<"\t\tInput your option \n\
 \t\t1.Login\n\
 \t\t2.Register\n\
 \t\t3.Search for other players\n\
 \t\t4.Highscores (Top 10)\n\
 \t\t5.Help\n\
 \t\t6.Exit\n\t\t";
 cin>>res;
 switch(res)
 {
 case 1:
 Login();
 break;
 case 2:
 Register();
 break;
 case 3:
 clrscr();
 cout<<"\t\t|||||Search Menu||||||\n\n";
 cout<<"\t\tSeach for Player\n\nInput your option\n\
	1.Search by name\n\
	2.Search by score\n\
	3.Go back to Main menu\n";
 cin>>res2;
 switch(res2)
 {
 case 1:
 Search_Name();
 break;
 case 2:
 Search_Score();
 break;
 case 3:
 menu();
 break;
 }
 break;

 case 4:
 Top10();
 break;
 case 5:
 clrscr();
 cout<<"Use W - to move up \nS - to go down \nA - to go left \nD - to go right\n\
 Dont hit the walls if you do so the game will get lose\n\
 10 score for collecting each pickup and your score will be saved always in your account ";
 getch();
 menu();
 break;
 case 6:
 exit(0);
 break;
 }
}

void Search_Score()
{
 clrscr();
 char chart;
 int tmp;
 cout<<"Input the score to be searched\n";
 cin>>tmp;
 for(int i=0;i<data_size;i++)
 {
 if(s[i].getscore() == tmp)
 s[i].display();
 }
 cout<<"\n\nPress y to search for more people or any other key to go back  \n ";
 cin>>chart;
 switch(chart)
 {
  case 'y'://to search again
  Search_Score();
  break;
  default://any other letter
  menu();
  break;
 }
}

void Search_Name()
{
 clrscr();

 char tmp[24],chart;
 cout<<"Input the name of player to be searched\n";
 gets(tmp);
 for(int i=0;i<data_size;i++)
 {
 if(strcmp(s[i].getname(),tmp)==0)
 s[i].display();
 }
 cout<<"\n\nPress y to search for more people or any other key to go back  \n ";
 cin>>chart;
 switch(chart)
 {
  case 'y':
  Search_Name();
  break;
  default:
  menu();
  break;
 }
}

void Top10()
{
 clrscr();
 int i,j;
 player_data tmp;
 /*temp object for swapping*/

 /*Linear sort*/
 for(i=0;i<data_size;i++)
 {
  for(j=0;j<data_size;j++)
  {
   if(s[i].getscore()>s[j].getscore())
   {
    tmp = s[i];
    s[i]= s[j];
    s[j]= tmp;
   }
  }
 }
 cout<<"\n\t\t\t   TOP 10 Players";
 cout<<"\n\t\t\t....................\n\n\n";
 cout<<"\t\tName\tScore\n\n";

 for(i=0;i<10;i++)
  {
  if(s[i].getscore()>0)//excluding zero scores
  s[i].Chart();//calling chart function
  }
 cout<<"\n\n\Press any key to go back ";
 getch();
 menu();//going back to menu
}



void main()
{

 clrscr();

 randomize();

 /*Setting color to background and objects*/

 textbackground(WHITE);
 textcolor(BLACK);

 game_data game;//object holding game data from game_data class
 connect();//connect to database
 menu();//display menu function

 if(logined)//if the player logged in
 {

 s[pindex].load_score();//loading player current score and stats

 game.update();//setting up game backgrounds

 /*The functions controller and  move will be called continuosly till the game get overs
 as gameplay depends on these functions*/

 while(!game.gameover)
 {
  game.controller();
  game.move();
  delay(150);//the looping speed is decreased using this so as to provide an excellent gameplay mechanism
 }
 }
 clrscr();
 cout<<"\t\t!!!! GAME OVER !!!!!\n";


 s[pindex].save_score();//saves the score and stats of player
 save();//saves details to database

 getch();

}
