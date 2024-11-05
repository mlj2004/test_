//date: 11.5. edition: 1.2
//author: Liu yc
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cmath>

#define LL long long

using namespace std;

LL read(){
	LL x=0,k=1;char ch=getchar();
	while('0'>ch || ch >'9'){if(ch=='-')k=-1;ch=getchar();}
	while('0'<=ch && ch<='9'){x=x*10+(ch^48);ch=getchar();}
	return x*k;
}
//leveldata
int InBoxLenth[10],OutBoxLenth[10];
int InBoxseq[10][100],OutBoxseq[10][100];
int AvailableSpace[10];
bool AvailableCode[10][8];

//gamedata
int CurrentLevel=1;
int CurrentInputSeq[100],InputLenth;//input start from 0
int CurrentOutputSeq[100],OutputLenth;//output start from 0

int CodeLenth;//code start from 1
int CodeType[1000];//normal type from 0 to 7, -1 for void code
int CodeData[1000];
bool GoodCode[1000];
int CurrentCodeLine;

int space[100];//space start from 0
bool SpaceUsed[200];
int RobotPos,RobotNum,RobotTyp;//0 for no number 1 for have number
void Refresh(){

}

//---------------------------------------------------------------------------------------
//0 for success -1 for fail x for Error on instruction x（实现报错）

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
	for(CurrentCodeLine = 1; CurrentCodeLine <= CodeLenth; CurrentCodeLine++){
		if(!GoodCode[CurrentCodeLine]){
			return CurrentCodeLine;
		}
		//cout << CurrentCodeLine<<":";
		if(CodeType[CurrentCodeLine] == 0){
			if(InputLenth >= InBoxLenth[CurrentLevel]){
				break;	//游戏结束
			}
			dictator = inbox();
			//inbox();
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 1){
			//cout << "outbox" << endl;
			dictator = outbox();
			//outbox();
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 2){
			dictator = add(CodeData[CurrentCodeLine]);
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 3 ){
			dictator = sub(CodeData[CurrentCodeLine]);
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 4 ){
			dictator = copyto(CodeData[CurrentCodeLine]);
			if(typ)Refresh();
		}
		else if(CodeType[CurrentCodeLine] == 5 ){
			dictator = copyfrom(CodeData[CurrentCodeLine]);
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
//---------------------------------------------------------------------------------------
void Initialize(){
	InBoxLenth[1]=2,OutBoxLenth[1]=2;
	InBoxseq[1][0]=1;
	InBoxseq[1][1]=2;
	OutBoxseq[1][0]=1;
	OutBoxseq[1][1]=2;
	AvailableSpace[1]=0;
	AvailableCode[1][0]=1;
	AvailableCode[1][1]=1;
	//--------------------------------
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
	//--------------------------------------------
	InBoxLenth[3]=8,OutBoxLenth[3]=2;
	InBoxseq[3][0]=6;
	InBoxseq[3][1]=2;
	InBoxseq[3][2]=7;
	InBoxseq[3][3]=7;
	InBoxseq[3][4]=-9;
	InBoxseq[3][5]=3;
	InBoxseq[3][6]=-3;
	InBoxseq[3][7]=-3;
	OutBoxseq[3][0]=7;
	OutBoxseq[3][1]=-3;
	AvailableSpace[3]=3;
	AvailableCode[3][0]=1;
	AvailableCode[3][1]=1;
	AvailableCode[3][2]=1;
	AvailableCode[3][3]=1;
	AvailableCode[3][4]=1;
	AvailableCode[3][5]=1;
	AvailableCode[3][6]=1;
	AvailableCode[3][7]=1;
}
void LevelInitialize(int lv){
	CurrentCodeLine=0;
	CodeLenth = 0;
	CodeType[1]=-1;
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
	InputLenth=InBoxLenth[lv];
	OutputLenth=0;
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
int main()
{
	int lv;
	scanf("%d",&lv);
	Initialize();
	LevelInitialize(lv);
	CurrentLevel=lv;
	scanf("%d",&CodeLenth);
	getchar();
	for(int i=1;i<=CodeLenth;i++){
		CurrentCodeLine=i;
		if(CheckInput()){
			GoodCode[i]=1;
		}
	}
	int ans=Solve(0);
    // for(int i=0;i<OutBoxLenth[CurrentLevel];i++){
    //     cout<<CurrentOutputSeq[i]<<endl;
    // }
	if(ans==0){
		cout<<"Success"<<endl;
	}else if(ans==-1){
		cout<<"Fail"<<endl;
	}else{
		cout<<"Error on instruction "<<ans<<endl;
	}
    return 0;
}
/*
1
2
inbox
outbox
*/

/*
1 2 5 success
7 Fail

3
10
inbox
copyto 0
inbox
copyto 1
sub 0
jumpifzero 8
jump 1
copyfrom 0
outbox
jump 1
*/
