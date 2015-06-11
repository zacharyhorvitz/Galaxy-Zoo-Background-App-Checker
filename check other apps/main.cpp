#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <tchar.h>
#include <psapi.h>
#include <ctime>
//#include <pthread.h> need to get working
using namespace std;

//ideas:coding sites?
//check for content screen opencv?....hmmmmmyes
//get it to open in fullscreentab 
//other applications, youtube, netflix
LPCSTR url;
LPCSTR currentname;

//take from notepad
HWND windowgamein;//window game in
string makelog()
{
	time_t time;
	std::time(&time);
	stringstream filename;
	filename<<"THISISIT"<<time<<"file.txt";
	string returnvalue = filename.str();
	return returnvalue;
}
long gettime()
{
	time_t time;
	std::time(&time);
	long timenow = time;
	return timenow;
}
POINT checkpositionchange()
{
	POINT newp;
	if(GetCursorPos(&newp))
	{
		return newp; 
	}
	
}
bool keyboardused()
{
	for(int i = 0; i < 120; i++)
	{
	if(GetAsyncKeyState(i) != 0)
	{
		return true;
	}
	}
	return false;
}
bool checkprocesses(HWND attentionwindow)
{
	ifstream file("interruptprocesses.txt");
	
	if(file.is_open())
	{
	string page;
	int number = 0;
	while(getline(file,page))
	{
	LPCSTR pName = page.c_str();
	cout<<page<<endl;
	windowgamein = FindWindow(NULL, pName); 
	//cout<<GetExitCodeProcess(////)<<endl;
	if(windowgamein !=0 && windowgamein == attentionwindow)
	{
		cout<<"HI";
		return true;
	}
	}
	}
	return false;

}
bool isitrunning(LPCSTR name)
{
	
	HWND checkwindow;
	checkwindow = FindWindow(NULL, name);
	//cout<<GetExitCodeProcess(////)<<endl;
	if(checkwindow == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
LPCSTR pullupsite()////need to finish, need to figure out way to figure out webpage name from url
{
	ifstream file("mysite.txt");//make sure this exists.
	bool found = false;
	string mysite;
	int line = 1;
	LPCSTR firstline; //used if it doesn't catch earlier
	
	while(getline(file,mysite))
	{
	if(line % 2 == 0)
	{
		firstline = mysite.c_str();
		cout<<firstline<<endl;
	//	Sleep(10000000);
		return firstline;
	}
	//if(line % 2 != 0)
	//{
	//if(line == 1)
	//{
	//	firstline = mysite.c_str();
	//}
	//int chooser = rand() % 4; //the higher the number, the more likely it will get picked
	//if(chooser == 0)
	//{
	//LPCSTR pName = mysite.c_str();
	//found = true;//not used
	//return pName;

	
	
	line++;
	}

	return "Error";
	
}




int main()
{
	HWND old = 0;
	long starttime = gettime();
	ofstream savefile(makelog());
	int ticker = 0;
	bool wastetime = false;
	bool engaged = false;
	POINT previouspoint = {0,0};
	//url = pullupsite(); //not working
	url = "http://www.galaxyzoo.org/#/classify";
	while(true)
	{
	HWND attentionwindow = GetForegroundWindow();
	if(attentionwindow != old)//time spent in window without change, add mouse/keyboard input component
	{
	stringstream NAMEOFATTENTIONWINDOW;
	NAMEOFATTENTIONWINDOW<<old;
	savefile<<NAMEOFATTENTIONWINDOW.str()<<endl;//need to convert HWND to string
	savefile<<gettime()-starttime<<endl;
	starttime = gettime();
	}
	old = attentionwindow;
	
	/*if(checkprocesses("Galaxy Zoo - Google Chrome"))
	{
		cout<<"hi";
	}*/
	while(checkprocesses(attentionwindow))
	{
	
	cout<<"HI"<<endl;
	POINT tempoint = checkpositionchange();
	//keyboard too?
	if(ticker == 1000)
	{
	ticker = 0;
	
	if(!isitrunning("Galaxy Zoo - Mozilla Firefox"))
	{
	if(engaged == true)
	{
		cout<<"FAILURE TO LOAD"<<endl;
		Sleep(1000);
		engaged = false;
		break;
	}
	}

	if(abs(tempoint.x - previouspoint.x) <= 1 && abs(tempoint.y-previouspoint.y)<=1 && !keyboardused())
		{
			wastetime = true;
			cout<<"!!!"<<endl;
		}
	previouspoint = tempoint;
	}
	if(wastetime && !engaged)
	{
	//add time check, so that in case that the tab is missing, it knows it launched site so it would wait
	ShellExecute(windowgamein, 0, url, 0, 0 , 5);
	cout<<"HI"<<endl;
	wastetime = false;
	engaged = true;
	Sleep(5000);
	}
	
	ticker++;
	
	}
	}

return 0;
}