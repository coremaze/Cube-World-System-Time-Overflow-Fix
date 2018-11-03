#include <windows.h>

unsigned int base;

unsigned int current_time = 0;
unsigned int previous_timeGetTime;

unsigned int GetTime(){

    /*
    This function replaces Cube World's timeGetTime.
    It uses a time delta which works across timeGetTime overflows.
    Cube World will treat the result of this as the current system time,
    instead of the actual system time. This means that you may run the
    game for 24 days before encountering the bug, as opposed to
    only being able to run your PC for 24 days before encountering the bug.
    */

    unsigned int this_time = timeGetTime();
    unsigned int delta_time = this_time - previous_timeGetTime;
    current_time += delta_time;
    previous_timeGetTime = this_time;

    return current_time;

}

void WriteFunc(BYTE* location, BYTE* newFunc){
    DWORD dwOldProtection;
	VirtualProtect(location, 4, PAGE_EXECUTE_READWRITE, &dwOldProtection);
    *(DWORD*)location = (DWORD)newFunc;
	VirtualProtect(location, 4, dwOldProtection, &dwOldProtection);
}

extern "C" __declspec(dllexport) bool APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            base = (unsigned int)GetModuleHandle(NULL);

            //Initialize previous time
            previous_timeGetTime = timeGetTime();

            //Overwrite the reference to Cube World's timeGetTime with our function
            WriteFunc((BYTE*)(base + 0x2FC5C8), (BYTE*)&GetTime);
            break;
    }
    return true;
}
