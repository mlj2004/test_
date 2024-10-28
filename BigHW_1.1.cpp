//date: 10.28. edition:1.1
//author: Meng lj & Liu yc 
#include <iostream>
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
int InBoxLenth[10],OutBoxLenth[10];
int InBoxseq[10][100];
int OutBoxseq[10][100];
int AvailableSpace[10];
bool AvailableCode[10][8];
//gamedata
int CurrentLevel=1;
int CurrentInputSeq[100],InputLenth;//input strat from 0
int CurrentOutputSeq[100],OutputLenth;//output start from 0

int CodeLenth;//code start from 1
int CodeType[1000];//normal type from 0 to 7, -1 for void code
int CodeData[1000];
int CurrentCodeLine;

int space[100];//space start from 0
bool SpaceUsed[100];
int RobotPos,RobotNum,RobotTyp;//0 for no number 1 for have number

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
	CodeType[CurrentCodeLine]=typ;
	CodeData[CurrentCodeLine]=dat;
	return 1;
}
void Opening(){
	printf("press any bottom to start !\n");
	char ch=getch();
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
	for(int i=1;i<=MaxLevel;i++){
		string f="  new  ";
		if(finish[i])f="finish!";
		if(!finish[i-1])f="locked.";
		int x=10,y=i*9-7;
		for(int j=0;j<7;j++){
			s[x][y+j]=f[j];
		}
	}
	for(int i=0;i<7;i++){
		s[7][CurrentLevel*9-7+i]='-';
		s[11][CurrentLevel*9-7+i]='-';
	}
	for(int i=0;i<3;i++){
		s[8+i][CurrentLevel*9-8]='|';
		s[8+i][CurrentLevel*9]='|';
	}
	s[7][CurrentLevel*9-8]=s[11][CurrentLevel*9-8]=s[7][CurrentLevel*9]=s[11][CurrentLevel*9]='*';
	for(int i=0;i<12;i++){
		cout<<s[i];
	}
	while(ch!=13){
		ch=getch();
		if(ch==27)return 1;
		if(ch==-32){
			ch=getch();
			if(ch=='M' && CurrentLevel<MaxLevel && finish[CurrentLevel]){
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[11][CurrentLevel*9-7+i]=' ';
				for(int i=0;i<5;i++)
					s[7+i][CurrentLevel*9-8]=s[7+i][CurrentLevel*9]=' ';
				CurrentLevel++;
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[11][CurrentLevel*9-7+i]='-';
				for(int i=0;i<3;i++)
					s[8+i][CurrentLevel*9-8]=s[8+i][CurrentLevel*9]='|';
				s[7][CurrentLevel*9-8]=s[11][CurrentLevel*9-8]=s[7][CurrentLevel*9]=s[11][CurrentLevel*9]='*';
			}
			if(ch=='K' && CurrentLevel>1){
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[11][CurrentLevel*9-7+i]=' ';
				for(int i=0;i<5;i++)
					s[7+i][CurrentLevel*9-8]=s[7+i][CurrentLevel*9]=' ';
				CurrentLevel--;
				for(int i=0;i<7;i++)
					s[7][CurrentLevel*9-7+i]=s[11][CurrentLevel*9-7+i]='-';
				for(int i=0;i<3;i++)
					s[8+i][CurrentLevel*9-8]=s[8+i][CurrentLevel*9]='|';
				s[7][CurrentLevel*9-8]=s[11][CurrentLevel*9-8]=s[7][CurrentLevel*9]=s[11][CurrentLevel*9]='*';
			}
			system("cls");
			for(int i=0;i<12;i++){
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
			if(ch=='r'){
				RobotPos++;
				Refresh();
			}
			if(ch=='e'){
				RobotPos--;
				Refresh();
			}
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
	if(X > AvailableSpace[CurrentLevel] || X < 0 || SpaceUsed[X] == 0 || !AvailableCode[CurrentLevel][5]){
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
	CurrentCodeLine = CodeData[X];
	return 0;
}

int jumpifzero(int X){
	if(X > CodeLenth || RobotTyp == 0 || X <= 0 || !AvailableCode[CurrentLevel][7]){
		return CurrentCodeLine;
	}
	//判断是否违法（num在程序之外）
	if(RobotNum == 0){
		CurrentCodeLine = CodeData[X];
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
	for(CurrentCodeLine = 1; CurrentCodeLine <= CodeLenth; CurrentCodeLine++){
		//cout << CodeType[CurrentCodeLine];
		if(CodeType[CurrentCodeLine] == 0){
			if(InputLenth >= InBoxLenth[CurrentLevel]){
				break;	//游戏结束
			}
			dictator = inbox();
			//inbox();
			if(typ){
				RobotPos=0;
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 1){
			//cout << "outbox" << endl;
			dictator = outbox();
			//outbox();
			if(typ){
				RobotPos=7;
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 2){
			dictator = add(CodeData[CurrentCodeLine]);
			if(typ){
				RobotPos=CodeData[CurrentCodeLine];
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 3 ){
			dictator = sub(CodeData[CurrentCodeLine]);
			if(typ){
				RobotPos=CodeData[CurrentCodeLine];
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 4 ){
			dictator = copyto(CodeData[CurrentCodeLine]);
			if(typ){
				RobotPos=CodeData[CurrentCodeLine];
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 5 ){
			dictator = copyfrom(CodeData[CurrentCodeLine]);
			if(typ){
				RobotPos=CodeData[CurrentCodeLine];
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 6 ){
			dictator = jump(CodeData[CurrentCodeLine]);
			if(typ){
				Refresh();
				sleep(1);
			}
		}
		else if(CodeType[CurrentCodeLine] == 7 ){
			dictator = jumpifzero(CodeData[CurrentCodeLine]);
			if(typ){
				Refresh();
				sleep(1);
			}
		}
		if(dictator != 0){
            
			return CurrentCodeLine;//modified by mlj /10_28
		}
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
//-----------------------------------------------------------------
void Initialize(){
	Info[1]=		"this is level one! Let the robot take out each box from the   |Coding instructions: Use up and down arrows to select a|\n"
					"input sequence and put it into the output sequence            |line. Press 'C' to change the current code,'A' to add a|\n"
					"                                                              |new line, 'D' to delet the code, \"Enter\" to confirm    |\n"
					"                                                              |your new code. Press 'S' to start the robot!           |\n"
					"(press 'H' for help,press Esc to quit)                        |                                                       |\n";
	InBoxLenth[1]=2,OutBoxLenth[1]=2;
	InBoxseq[1][0]=1;
	InBoxseq[1][1]=2;
	OutBoxseq[1][0]=1;
	OutBoxseq[1][1]=2;
	AvailableSpace[1]=0;
	AvailableCode[1][0]=1;
	AvailableCode[1][1]=1;
	Info[2]=		"this is level two! For every two boxes in the input sequence, |Coding instructions: Use up and down arrows to select a|\n"
					"subtract the second from the first and place the result in the|line. Press 'C' to change the current code,'A' to add a|\n"
					"output sequence. Then subtract the first from the second and  |new line, 'D' to delet the code, \"Enter\" to confirm    |\n"
					"place the result in the output sequence. Repeat this process. |your new code. Press 'S' to start the robot!           |\n"
					"(press 'H' for help,press Esc to quit)                        |                                                       |\n";
	InBoxLenth[2]=8,OutBoxLenth[2]=8;
	InBoxseq[2][0]=3;
	InBoxseq[2][1]=9;
	InBoxseq[2][2]=5;
	InBoxseq[2][3]=1;
	InBoxseq[2][4]=-2;
	InBoxseq[2][5]=-2;
	InBoxseq[2][6]=9;
	InBoxseq[2][7]=-9;
	OutBoxseq[2][0]=-6;
	OutBoxseq[2][1]=6;
	OutBoxseq[2][2]=4;
	OutBoxseq[2][3]=-4;
	OutBoxseq[2][4]=0;
	OutBoxseq[2][5]=0;
	OutBoxseq[2][6]=18;
	OutBoxseq[2][7]=-18;
	AvailableSpace[2]=3;
	AvailableCode[2][0]=1;
	AvailableCode[2][1]=1;
	AvailableCode[2][2]=1;
	AvailableCode[2][3]=1;
	AvailableCode[2][4]=1;
	AvailableCode[2][5]=1;
	AvailableCode[2][6]=1;
	AvailableCode[2][7]=1;
	Info[3]=		"this is level three! Take two numbers from the input belt in  |Coding instructions: Use up and down arrows to select a|\n"
					"sequence. If they are equal, output one of them; otherwise,   |line. Press 'C' to change the current code,'A' to add a|\n"
					"discard it. Repeat this process until the input belt is empty.|new line, 'D' to delet the code, \"Enter\" to confirm    |\n"
					"                                                              |your new code. Press 'S' to start the robot!           |\n"
					"(press 'H' for help,press Esc to quit)                        |                                                       |\n";
	InBoxLenth[3]=8,OutBoxLenth[3]=2;
	InBoxseq[3][0]=6;
	InBoxseq[3][1]=2;
	InBoxseq[3][2]=7;
	InBoxseq[3][3]=7;
	InBoxseq[3][4]=-9;
	InBoxseq[3][5]=3;
	InBoxseq[3][6]=-3;
	InBoxseq[3][7]=-3;
	OutBoxseq[2][0]=-6;
	OutBoxseq[2][1]=6;
	OutBoxseq[2][2]=4;
	OutBoxseq[2][3]=-4;
	OutBoxseq[2][4]=0;
	OutBoxseq[2][5]=0;
	OutBoxseq[2][6]=18;
	OutBoxseq[2][7]=-18;
	AvailableSpace[3]=3;
	AvailableCode[3][0]=7;
	AvailableCode[3][1]=-3;
	Info[4]=		"this is level one! (press Esc to quit)                        |Coding instructions: Use up and down arrows to select a|\n"
					"                                                              |line. Press 'C' to change the current code,'A' to add a|\n"
					"                                                              |new line, 'D' to delet the code, \"Enter\" to confirm    |\n"
					"                                                              |your new code. Press 'S' to start the robot!           |\n";
	InBoxLenth[4]=5,OutBoxLenth[4]=5;
	Info[5]=		"this is level one! (press Esc to quit)                        |Coding instructions: Use up and down arrows to select a|\n"
					"                                                              |line. Press 'C' to change the current code,'A' to add a|\n"
					"                                                              |new line, 'D' to delet the code, \"Enter\" to confirm    |\n"
					"                                                              |your new code. Press 'S' to start the robot!           |\n";
	InBoxLenth[5]=5,OutBoxLenth[5]=5;
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
	finish[0]=1;
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
		LevelInitialize(CurrentLevel,0);
		Refresh();
		if(CodeInput(1))break;
		s=Solve(1);
		if(s==0){
			printf("Success ! (press 'q' return to index)\n");
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
