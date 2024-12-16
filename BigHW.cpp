//date: 11.05. edition:1.3
//author: Meng lj & Liu yc 
/*
log:
11.04
更新了存档功能
更新了一键通关
11.05
debug 通过oj
12.9. 
*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <unistd.h>
#include <cmath>

#define LL long long

using namespace std;
//constant data
int MaxLevel=5;


//screendata
string Info[10];
string screen[100];
string robot[10];
//leveldata
int finish[10];
int score[10];
int InBoxLenth[10],OutBoxLenth[10];
int InBoxseq[10][100];
int OutBoxseq[10][100];
int AvailableSpace[10];
bool AvailableCode[10][8];
int StandardLenth[10][2];
//gamedata
int CurrentLevel=1;
int CurrentInputSeq[100],InputLenth;//input strat from 0
int CurrentOutputSeq[100],OutputLenth;//output start from 0

int CodeLenth;//code start from 1
int CodeType[1000];//normal type from 0 to 7, -1 for void code
int CodeData[1000];
int CurrentCodeLine;
bool GoodCode[1000];
int space[100];//space start from 0
bool SpaceUsed[100];
int RobotPos,RobotNum,RobotTyp;//0 for no number 1 for have number
bool superkey;
bool InGame;
string UserName;
int StepCounter;

LL read(){
	LL x=0,k=1;char ch=getchar();
	while('0'>ch || ch >'9'){if(ch=='-')k=-1;ch=getchar();}
	while('0'<=ch && ch<='9'){x=x*10+(ch^48);ch=getchar();}
	return x*k;
}

string NumberShifter(int a){
	if(a>=100 || a<=-100)return "Inf";
	string ans="";
	if(a<0)ans+="-",a=-a;
	else ans+=" ";
	if(a<10){
		ans+=char(a+'0');
		ans+=" ";
	}else{
		ans+=char(a/10+'0');
		ans+=char(a%10+'0');
	}
	return ans;
}

char waituntil(char c1,char c2){
	char ch=getch();
	while(ch!=c1 && ch!=c2){
		ch=getch();
	}
	return ch;
}

void ReadAccount(){
    ifstream fin;
	fin.open("./account/"+UserName+".ac");
	finish[0]=1;
	if(fin && !fin.eof()){
		fin.read(reinterpret_cast <char*>(finish),sizeof(finish));
		fin.read(reinterpret_cast <char *>(score),sizeof(score));
	}
	fin.close();
}

void WriteAccount(){
	ofstream fout;
	fout.open("./account/"+UserName+".ac");
	fout.write(reinterpret_cast <char *>(finish),sizeof(finish));
	fout.write(reinterpret_cast <char *>(score),sizeof(score));
	fout.close();
}

bool CheckInput(){
	string ss;
	getline(cin,ss);
	int l=ss.length(),typ=-1,dat=0,k=1;
	string s="";
	bool flag=0;
	int i;
	for(i=0;i<l;i++){
		if(ss[i]==' ')break;
		s+=ss[i];
	}
	while(ss[i]==' '){
		i++;
	}
	if(ss[i]=='-'){
		k=-1;i++;
	}
	for(;i<l;i++){
		if('0' <= ss[i] && ss[i]<='9'){
			flag=1;
			dat=dat*10+ss[i]-'0';
		}else break;
	}
	while(ss[i]==' '){
		i++;
	}
	if(i!=l) return 0;
	if(k==-1)return 0;
	dat=dat*k;
	if(s=="inbox"){
		typ=0;
	}else if(s=="outbox"){
		typ=1;
	}else if(s=="add"){
		typ=2;
	}else if(s=="sub"){
		typ=3;
	}else if(s=="copyto"){
		typ=4;
	}else if(s=="copyfrom"){
		typ=5;
	}else if(s=="jump"){
		typ=6;
	}else if(s=="jumpifzero"){
		typ=7;
	}else return 0;
	if(!AvailableCode[CurrentLevel][typ])return 0;
	if(typ>=2 && !flag) return 0;
	if(typ<=1 && flag)return 0;
	GoodCode[CurrentCodeLine]=1;
	CodeType[CurrentCodeLine]=typ;
	CodeData[CurrentCodeLine]=dat;
	return 1;
}

void Opening(){
	printf("press any button to start !\n");
	char ch=getch();
    printf("Please input your username !\n:");
    cin>>UserName;
	getchar();
    ReadAccount();
}

int Index(){
	char ch=0;
	system("cls");
	string s[100];
	s[0]="please select level! \n";
	s[1]="finish previous levels to unlock new level\n";
	s[2]="(press left and right arrows to switch)\n";
	s[3]="(press \"Enter\" to start)\n";
	s[4]="(press \"Esc\" to quit)\n";
	s[5]="\n";
	s[6]="\n";
	s[7]= "                                               \n";
	s[8] ="   level    level    level    level    level   \n";
	s[9] ="     1        2        3        4        5     \n";
	s[10]="    new    locked.  locked.  locked.  locked.  \n";
	s[11]="                                               \n";
	s[12]="                                               \n";
	for(int i=1;i<=MaxLevel;i++){
		string f="  new  ";
		char c[10];
		sprintf(c,"-=%03d=-",score[i]);
		if(finish[i])f="finish!";
		if(!finish[i-1])f="locked.";
		int x=10,y=i*9-7;
		for(int j=0;j<7;j++){
			s[x][y+j]=f[j];
			s[x+1][y+j]=c[j];
		}
	}
	for(int i=0;i<7;i++){
		s[7][CurrentLevel*9-7+i]='-';
		s[12][CurrentLevel*9-7+i]='-';
	}
	for(int i=0;i<4;i++){
		s[8+i][CurrentLevel*9-8]='|';
		s[8+i][CurrentLevel*9]='|';
	}
	s[7][CurrentLevel*9-8]=s[12][CurrentLevel*9-8]=s[7][CurrentLevel*9]=s[12][CurrentLevel*9]='*';
	for(int i=0;i<13;i++){
		cout<<s[i];
	}
	while(ch!=13){
		ch=getch();
		if(ch==27)return 1;
		if(ch==-32){
			ch=getch();
			if(ch=='M' && CurrentLevel<MaxLevel && finish[CurrentLevel]){
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[12][CurrentLevel*9-7+i]=' ';
				for(int i=0;i<6;i++)
					s[7+i][CurrentLevel*9-8]=s[7+i][CurrentLevel*9]=' ';
				CurrentLevel++;
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[12][CurrentLevel*9-7+i]='-';
				for(int i=0;i<4;i++)
					s[8+i][CurrentLevel*9-8]=s[8+i][CurrentLevel*9]='|';
				s[7][CurrentLevel*9-8]=s[12][CurrentLevel*9-8]=s[7][CurrentLevel*9]=s[12][CurrentLevel*9]='*';
			}
			if(ch=='K' && CurrentLevel>1){
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[12][CurrentLevel*9-7+i]=' ';
				for(int i=0;i<6;i++)
					s[7+i][CurrentLevel*9-8]=s[7+i][CurrentLevel*9]=' ';
				CurrentLevel--;
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[12][CurrentLevel*9-7+i]='-';
				for(int i=0;i<4;i++)
					s[8+i][CurrentLevel*9-8]=s[8+i][CurrentLevel*9]='|';
				s[7][CurrentLevel*9-8]=s[12][CurrentLevel*9-8]=s[7][CurrentLevel*9]=s[12][CurrentLevel*9]='*';
			}
			system("cls");
			for(int i=0;i<13;i++){
				cout << s[i];
			}
		}
	}
	return 0;
	//system("pause");
}

void HelpPage(){
	system("cls");
	printf("                                       __  __   ______   __        ______   __                                          \n");
	printf("                                      / / / /  / ____/  / /       / __  /  / /                                          \n");
	printf("                                     / /_/ /  / /___   / /       / /_/ /  / /                                           \n");
	printf("                                    / __  /  / ____/  / /       / ____/  /_/                                            \n");
	printf("                                   / / / /  / /___   / /____   / /      __                                              \n");
	printf("                                  /_/ /_/  /_____/  /______/  /_/      /_/     (press 'H' to return )                   \n");
	printf(" *---------------------------------------------------------------------------------------------------------------------*\n");
	printf(" | Welcome! In this game you are going control a robot in Xiaoming's room to fulfill several tasks. there are two belts|\n");
	printf(" | and some empty spaces in Xiaoming's room. One is Input belt, where you can get sequenced boxes with number on it.   |\n");
	printf(" | The robot can do several operations on these boxes and put it to the Output belt. Xiaoming will check the boxes on  |\n");
	printf(" | the Output belt to judge your correctness.operations you can do are as follow.                                      |\n");
	printf(" |---------------------------------------------------------------------------------------------------------------------|\n");
	printf(" |  name  |parameter|                                           explanation                                            |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" | inbox  |    No   |The robot picks up the first box from the input belt, which becomes the current box (discard      |\n");
	printf(" |        |         |previous box.The game ends when there are no more boxes on the input belt.                        |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" | outbox |    No   |The robot places the current box on the output belt. (no longer has the current box.)             |\n");
	printf(" |        |         |(FAILS when there is no current box.)                                                             |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" |  add   |    X    |Add the number at position X to the number on the current box. The box at position X remains      |\n");
	printf(" |        |         |unchanged (FAILS when There is no current box or There is no block at position X.)                |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" |  sub   |    X    |Subtract the number at position X from the number on current box. The box at position X remains   |\n");
	printf(" |        |         |unchanged (FAILS when There is no current box or There is no block at position X.)                |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" | copyto |    X    |Copy the current box to position X. If there is a box at position X, the original box is discarded|\n");
	printf(" |        |         |(FAILS when There is no current block or Position X does not exist.)                              |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" |copyfrom|    X    |Copy the box from position X to the current box, i.e. replace the current box with the box at     |\n");
	printf(" |        |         |position X and discard original current block. (FAILS when There is no block at that position X   |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" |  jump  |    X    |Change the robot's program sequence to start executing from the Xth instruction.                  |\n");
	printf(" |        |         |(FAILS when The Xth instruction does not exist.)                                                  |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	printf(" |        |         |If the current block is 0, change the robot's program sequence to start executing from the Xth    |\n");
	printf(" | jumpif-|    X    |instruction. If the current block is not 0, do nothing.(FAILS when The Xth instruction does not   |\n");
	printf(" |  zero  |         |exist or There is no current block)                                                               |\n");
	printf(" |--------*------------------------------------------------------------------------------------------------------------|\n");
	waituntil('h',27);
	return ;
}

void Refresh(){
	system("cls");
	printf("%s",Info[CurrentLevel].c_str());
	//changescreen
	for(int i=0;i<8;i++){
		string s="   ";
		if(i<(InBoxLenth[CurrentLevel]-InputLenth)){
			s=NumberShifter(CurrentInputSeq[i+InputLenth]);
		}
		int x=3*i+1,y=4;
		for(int j=0;j<3;j++){
			screen[x][y+j]=s[j];
		}
	}
	for(int i=0;i<8;i++){
		string s="   ";
		if(i<OutputLenth){
			s=NumberShifter(CurrentOutputSeq[OutputLenth-i-1]);
		}
		int x=3*i+1,y=55;
		for(int j=0;j<3;j++){
			screen[x][y+j]=s[j];
		}
	}
	for(int i=0;i<16;i++){
		string s=" X ";
		if(i<AvailableSpace[CurrentLevel]){
			if(SpaceUsed[i]){
				s=NumberShifter(space[i]);
			}else{
				s="   ";
			}
		}
		int x=7+12*(i/8),y=12+5*(i%8);
		for(int j=0;j<3;j++){
			screen[x][y+j]=s[j];
		}
	}
	int x=12*(RobotPos/8),y=10+5*(RobotPos%8);
	for(int i=0;i<6;i++){
		for(int j=0;j<7;j++){
			screen[x+i][y+j]=robot[i][j];
		}
	}
	string s="   ";
	if(RobotTyp) s = NumberShifter(RobotNum);
	for(int j=0;j<3;j++){
		screen[x+1][y+2+j]=s[j];
	}
	//print
	int l=max(CodeLenth,23);
	for(int i=0;i<=l;i++){
		printf("%s|\t",screen[min(i,24)].c_str());
		if(i==0)printf("## ---- CODE: ---- ##\n");
		else if(i<=CodeLenth){
			if(CurrentCodeLine==i)printf(">");
			else printf(" ");
			printf("%2d:",i);
			if(CodeType[i]==0){
				printf("inbox\n");
			}
			if(CodeType[i]==1){
				printf("outbox\n");
			}
			if(CodeType[i]==2){
				printf("add %d\n",CodeData[i]);
			}
			if(CodeType[i]==3){
				printf("sub %d\n",CodeData[i]);
			}
			if(CodeType[i]==4){
				printf("copyto %d\n",CodeData[i]);
			}
			if(CodeType[i]==5){
				printf("copyfrom %d\n",CodeData[i]);
			}
			if(CodeType[i]==6){
				printf("jump %d\n",CodeData[i]);
			}
			if(CodeType[i]==7){
				printf("jumpifzero %d\n",CodeData[i]);
			}
			if(CodeType[i]==-1){
				printf("\n");
			}
		}else{
			printf("\n");
		}
	}
	for(int i=0;i<6;i++){
		for(int j=0;j<7;j++){
			screen[x+i][y+j]=' ';
		}
	}
	if(InGame){
		sleep(1);
	}
}

int CodeInput(bool typ){
	if(typ){
		char ch=0;
		while(ch!='s'){
			ch=getch();
			if(ch==27)return 1;
			if(ch=='c' && CurrentCodeLine>=1){
				int tmp = CodeType[CurrentCodeLine];
				CodeType[CurrentCodeLine]=-1;
				Refresh();
				if(!CheckInput()){
					CodeType[CurrentCodeLine]=tmp;
					printf("input invalid ! (press 'Y' to continue)");
					waituntil('y','y');
					Refresh();
				}else{
					Refresh();
				}
			}
			if(ch=='a'){
				CodeLenth++;
				for(int i=CodeLenth-1;i>=CurrentCodeLine;i--){
					CodeData[i+1]=CodeData[i];
					CodeType[i+1]=CodeType[i];
				}
				CurrentCodeLine++;
				CodeType[CurrentCodeLine]=-1;
				Refresh(); 
				if(!CheckInput()){
					printf("input invalid ! (press 'Y' to continue)");
					waituntil('y','y');
					for(int i=CurrentCodeLine+1;i<=CodeLenth;i++){
					CodeData[i-1]=CodeData[i];
					CodeType[i-1]=CodeType[i];
					}
					CodeLenth--;
					CurrentCodeLine--;
					Refresh();
				}else{
					Refresh();
				}
			}
			if(ch=='d' && CodeLenth>1){
				for(int i=CurrentCodeLine+1;i<=CodeLenth;i++){
					CodeData[i-1]=CodeData[i];
					CodeType[i-1]=CodeType[i];
				}
				CodeLenth--;
				CurrentCodeLine--;
				Refresh();
			}
			if(ch=='h'){
				HelpPage();
				Refresh();
			}
			//-----------------------
			if(UserName == "JIE_GE"){
				if(ch=='r'){
					RobotPos++;
					Refresh();
				}
				if(ch=='e'){
					RobotPos--;
					Refresh();
				}
				if(ch=='k'){
					superkey = 1;
					return 0;
				}
			}
			//-----------------------
			if(ch==-32){
				ch=getch();
				if(ch=='P' && CurrentCodeLine<CodeLenth){
					CurrentCodeLine++;
					Refresh();
				}
				if(ch=='H' && CurrentCodeLine>1){
					CurrentCodeLine--;
					Refresh();
				}
			}
		}
	}
	return 0;
}
//-----------------------------------------------------------------
//0 for success -1 for fail x for Error on instruction x
int inbox(){
	RobotNum = CurrentInputSeq[InputLenth];
	InputLenth += 1;
	RobotTyp = 1;	
	//cout << RobotTyp << endl;
	return 0;
}

int outbox(){
	//cout<<RobotTyp;
	if(RobotTyp == 0){
		return CurrentCodeLine;
	}
	else{
		CurrentOutputSeq[OutputLenth] = RobotNum;
		//cout << CurrentOutputSeq[OutputLenth];
		OutputLenth += 1;
		RobotTyp = 0;
	}
	return 0;
}

int add(int X){
	if(RobotTyp == 0 || SpaceUsed[X] == 0 || !AvailableCode[CurrentLevel][2]){
		return CurrentCodeLine;
	}
	else{
		RobotNum += space[X];
	}
	return 0;
}

int sub(int X){
	if(RobotTyp == 0 || SpaceUsed[X] == 0 || !AvailableCode[CurrentLevel][3]){
		return CurrentCodeLine;
	}
	else{
		RobotNum -= space[X];
	}
	return 0;
}

int copyto(int X){
	if(RobotTyp == 0 || X > AvailableSpace[CurrentLevel] - 1 || X < 0 || !AvailableCode[CurrentLevel][4]){
		return CurrentCodeLine;
	}
	else{
		space[X] = RobotNum;
		SpaceUsed[X] = 1;
		//cout << space[X] << ' ' << SpaceUsed[X] << endl;
	}
	return 0;
}

int copyfrom(int X){
	if(X > AvailableSpace[CurrentLevel]-1 || X < 0 || SpaceUsed[X] == 0 || !AvailableCode[CurrentLevel][5]){//
		return CurrentCodeLine;
	}
	else{
		RobotNum = space[X];
		RobotTyp = 1;
	}
	return 0;
}

int jump(int X){
	if(X > CodeLenth || X <= 0 || !AvailableCode[CurrentLevel][6]){
		return CurrentCodeLine;
	}
	//判断是否违法（num在程序之外）
	CurrentCodeLine = X-1;//codedata[x] jump to anywhere
	return 0;
}

int jumpifzero(int X){
	if(X > CodeLenth || RobotTyp == 0 || X <= 0 || !AvailableCode[CurrentLevel][7]){
		return CurrentCodeLine;
	}
	//判断是否违法（num在程序之外）
	if(RobotNum == 0){
		CurrentCodeLine = X-1;//codedata[x] jump to anywhere
	}
	return 0;
}

int Solve(bool typ){
	int dictator = 0;
	//cout << AvailableSpace[CurrentLevel];//0?
	for(int i = 0; i < InBoxLenth[CurrentLevel]; i++){
		CurrentInputSeq[i] = InBoxseq[CurrentLevel][i];
	}
	InputLenth = 0;
	OutputLenth = 0;
	int tr = 1;
	StepCounter = 0;
	for(CurrentCodeLine = 1; CurrentCodeLine <= CodeLenth; CurrentCodeLine++){
		if(!GoodCode[CurrentCodeLine]){
			return CurrentCodeLine;
		}
		StepCounter ++ ;
		//cout << CurrentCodeLine<<":";
		if(CodeType[CurrentCodeLine] == 0){
			if(InputLenth >= InBoxLenth[CurrentLevel]){
				break;	//游戏结束
			}
			dictator = inbox();
			//inbox();
			RobotPos = 0;
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 1){
			//cout << "outbox" << endl;
			dictator = outbox();
			//outbox();
			RobotPos = 7;
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 2){
			dictator = add(CodeData[CurrentCodeLine]);
			RobotPos = CodeData[CurrentCodeLine];
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 3 ){
			dictator = sub(CodeData[CurrentCodeLine]);
			RobotPos = CodeData[CurrentCodeLine];
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 4 ){
			dictator = copyto(CodeData[CurrentCodeLine]);
			RobotPos = CodeData[CurrentCodeLine];
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 5 ){
			dictator = copyfrom(CodeData[CurrentCodeLine]);
			RobotPos = CodeData[CurrentCodeLine];
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 6 ){
			dictator = jump(CodeData[CurrentCodeLine]);
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 7 ){
			dictator = jumpifzero(CodeData[CurrentCodeLine]);
			if(typ)Refresh();
		}
		if(dictator != 0){
			return dictator;//
		}
        //cout<<CurrentCodeLine<<" "<<RobotNum<<endl;
	//检查outbox是否完全一致
	}
	if(OutputLenth == OutBoxLenth[CurrentLevel]){
		for(int i = 0; i < OutBoxLenth[CurrentLevel]; i++){
			tr *= (OutBoxseq[CurrentLevel][i] == CurrentOutputSeq[i]);
		}
		if (tr == 1){
			return 0;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------

double scounter(double x,double y){
	double k=y/x-1;
	return pow(0.5,k);
}

int scorecounter(){
	system("cls");
	ifstream fin;
	fin.open("success.txt");
	string s;
	while(!fin.eof()){
		getline(fin,s);
		cout<<s<<endl;
	}
	int s1=40*scounter(StandardLenth[CurrentLevel][0],CodeLenth);
	int s2=60*scounter(StandardLenth[CurrentLevel][1],StepCounter);
	int ans=s1+s2;
	printf("               your ans | standard | score\n");
	printf("CODE LENGTH :    %03d        %03d       %03d\n",CodeLenth,StandardLenth[CurrentLevel][0],s1);
	printf("RUNNING STEP:    %03d        %03d       %03d\n",StepCounter,StandardLenth[CurrentLevel][1],s2);
	printf("-------------------------------------------------\n");
	printf("TOTALL SCORE:    %03d \n",ans);
	return ans;
}

void Initialize(){
	ifstream fin;
	fin.open("leveldata.txt",ios::in);
	int t=1;
	while(!fin.eof()){
	string str;
		for(int i=0;i<5;i++){
			getline(fin,str);
			str+='\n';
			Info[t]+=str;
		}
		fin>>InBoxLenth[t];
		for(int i=0;i<InBoxLenth[t];i++)fin>>InBoxseq[t][i];
		fin>>OutBoxLenth[t];
		for(int i=0;i<OutBoxLenth[t];i++)fin>>OutBoxseq[t][i];
		fin>>AvailableSpace[t];
		for(int i=0;i<8;i++)fin>>AvailableCode[t][i];
		fin>>StandardLenth[t][0]>>StandardLenth[t][1];
		fin.get();
		t++;	
	}
	fin.close();
	screen[ 0] =	"   *---*                                              *---*   ";
	screen[ 1] =	"   |   |                                              |   |   ";
	screen[ 2] =	" I *---*                                              *---* O ";
	screen[ 3] =	" N *---*                                              *---* U ";
	screen[ 4] =	" P |   |                                              |   | T ";
	screen[ 5] =	" U *---*                                              *---* p ";
	screen[ 6] =	" T *---*   *---**---**---**---**---**---**---**---*   *---* U ";
	screen[ 7] =	"   |   |   |   ||   ||   ||   ||   ||   ||   ||   |   |   | T ";
	screen[ 8] =	"   *---*   *---**---**---**---**---**---**---**---*   *---*   ";
	screen[ 9] =	"/|\\*---*    #0   #1   #2   #3   #4   #5   #6   #7     *---* | ";
	screen[10] =	" | |   |                                              |   |\\|/";
	screen[11] =	"   *---*                                              *---*   ";
	screen[12] =	"   *---*                                              *---*   ";
	screen[13] =	"   |   |                                              |   |   ";
	screen[14] =	"   *---*                                              *---*   ";
	screen[15] =	"   *---*                                              *---*   ";
	screen[16] =	"   |   |                                              |   |   ";
	screen[17] =	"   *---*                                              *---*   ";
	screen[18] =	"   *---*   *---**---**---**---**---**---**---**---*   *---*   ";
	screen[19] =	"   |   |   |   ||   ||   ||   ||   ||   ||   ||   |   |   |   ";
	screen[20] =	"   *---*   *---**---**---**---**---**---**---**---*   *---*   ";
	screen[21] =	"   *---*    #8   #9   #10  #11  #12  #13  #14  #15    *---*   ";
	screen[22] =	"   |   |                                              |   |   ";
	screen[23] =	"   *---*                                              *---*   ";
	screen[24] =	"                                                              ";
	robot[0]=" *---* ";
	robot[1]=" |   | ";
	robot[2]="[*---*]";
	robot[3]="\\[0_0]/";
	robot[4]="  [_]  ";
	robot[5]="  d b  ";
}

void LevelInitialize(int lv,bool typ){
	CurrentCodeLine=0;
	if(typ){
        CodeLenth = 0;
	    CodeType[1]=-1;
    }
    RobotNum = 0;
	RobotTyp = 0;
	RobotPos = 0;
	for(int i=0;i<AvailableSpace[lv];i++){
		space[i]=0;
		SpaceUsed[i]=0;
	}
	for(int i=0;i<=InBoxLenth[lv];i++){
		CurrentInputSeq[i]=InBoxseq[lv][i];
	}
	InputLenth=0;
	OutputLenth=0;
}

void MainGame(){
    LevelInitialize(CurrentLevel,1);
	int s;//modified by mlj /10_28
	while(1){
		superkey=0;
		LevelInitialize(CurrentLevel,0);
		Refresh();
		if(CodeInput(1))break;
		InGame=1;
		if(superkey==0)
			s=Solve(1);
		else
			s=0;
		InGame=0;
		if(s==0){
			//printf("Success ! (press 'q' return to index)\n");
			score[CurrentLevel]=max(score[CurrentLevel],scorecounter());
			finish[CurrentLevel]=1;
			char ch=waituntil('q',27);
			break;
		}else{
			if(s==-1){
				printf("Fail (press 'q' return to index, 'r' to retry)\n");
			}else{
				printf("Error on instruction %d(press 'q' return to index, 'r' to retry)\n",s);
			}
			char ch=waituntil('q','r');
            if(ch=='q')break;
		}
	}
}

int main()
{
	Initialize();
    Opening();
	while(1){
		WriteAccount();
		if(Index())break;
		MainGame();
	}
    return 0;
}

/*
 *---*
 |123|
[*---*]
\[0_0]/
  [_]
  d b

*/

 