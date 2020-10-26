#include "App.h"

BOOL PlayMidi(char* fileName){
	wchar_t buffer[256];
	swprintf_s(buffer, L"open %S type sequencer alias MUSIC", fileName);
	if(mciSendString(L"close all", NULL, 0, NULL) != 0){
		return(FALSE);
	}
	if(mciSendString(buffer, NULL, 0, NULL) != 0){
		return(FALSE);
	}
	if(mciSendString(L"play MUSIC from 0", NULL, 0, NULL) != 0){
		return(FALSE);
	}
	return TRUE;
}

BOOL WINAPI ConsoleEventHandler(DWORD event){
    LPCWSTR msg;
    switch(event){
        case CTRL_C_EVENT: msg = L"Ctrl + C"; break;
        case CTRL_BREAK_EVENT: msg = L"Ctrl + BREAK"; break;
        case CTRL_CLOSE_EVENT: msg = L"Closing prog..."; break;
        case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: msg = L"User is logging off..."; break;
        default: msg = L"???";
    }
    MessageBox(NULL, msg, L"Msg from 193541T", MB_OK);
    return TRUE;
}

int main(){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleEventHandler, TRUE)){
        printf("Failed to install console event handler!\n");
        return -1;
    }
	PlayMidi("Audio/Epic.mid");
    App::GetApp()->Run();
}