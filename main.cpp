#include <windows.h>
#include <string>
#include <iostream>
#include <tlhelp32.h>
#include "timer.h"

using namespace std;

string language = "fr";

void open_page(string word)
{
	string domain = language + ".wiktionary.org";
	string url = "/wiki/" + word;
	ShellExecute(NULL, "open", ("https://" + domain + url).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

std::string get_clipboard_text()
{
	if (!OpenClipboard(nullptr))
		return "";

		HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return "";

		char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return "";

		std::string text(pszText);

	GlobalUnlock(hData);

	CloseClipboard();

	return text;
}

string check_word(string word)
{
	if (word.find(".") != -1 || word.find("/") != -1)
		return "";
	return word;
}

bool is_already_open()
{
	HANDLE hSnapShot;
	PROCESSENTRY32 pe;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	int count = 0;
	if (Process32First(hSnapShot, &pe) && count < 2)
	{
		do
		{
			if (string(pe.szExeFile).find("Wiktionary.exe") != string::npos)
			{
				count++;
				//cout << pe.th32ProcessID << endl;
			}
		}
		while (Process32Next(hSnapShot, &pe) && count < 2);
	}

	CloseHandle(hSnapShot);

	return (count > 1);
}


#ifndef DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
#else
int main()
{
#endif

	
	string word;

	Clock framerate_timer; framerate_timer.start();
	double framerate = 20.0;
	bool ls = false, s = false;
	bool lt = false, t = false;
	
	bool Keep = !is_already_open();

	while (Keep)
	{
		if (framerate_timer.duration() >= 1000000.0 / framerate)
		{
			framerate_timer.start();
			ls = s;
			lt = t;
			if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_F2) & 0x8000))
				Keep = false;

			if ((GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_F2) & 0x8000))
				t = true;
			else
				t = false;

			if (GetKeyState(VK_F2) & 0x8000 && Keep && !t)
				s = true;
			else
				s = false;

			if (s && !ls) // If pushed f2
			{
				word = check_word(get_clipboard_text());
				if (word != "")
					open_page(word);
			}
			if (t && !lt) // If pushed f2 and shift
			{
				if (language == "fr") language = "en";
				else language = "fr";
			}

		}
		else Clock::sleep(1000.0 / framerate - framerate_timer.duration() * 0.001);
	}

	return 0;
}