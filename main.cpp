#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <time.h>

#include "resource.h"

DWORD WINAPI random_letter(void *lpParameter)
{
    INPUT key_strock;
    {
        key_strock.type = INPUT_KEYBOARD;
        key_strock.ki.time = 0;
        key_strock.ki.dwExtraInfo = 0;
        key_strock.ki.wScan = 0;
    }

    while(true)
    {
        key_strock.ki.dwFlags = 0;
        key_strock.ki.wVk = 0x41 + rand() % (0x5A + 1 - 0x41);
        SendInput(1, &key_strock, sizeof(INPUT));

        SleepEx(500, FALSE); // time is in miliseconds (60000 is 1 minute)
        key_strock.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &key_strock, sizeof(INPUT));

        SleepEx(60000 + rand() % ((60000 * 5) + 1 - 60000), FALSE); // time is in miliseconds (60000 is 1 minute)
    }
    return 0;
}

DWORD WINAPI random_capslosk(void *lpParameter)
{
    INPUT key_strock;
    {
        key_strock.type = INPUT_KEYBOARD;
        key_strock.ki.time = 0;
        key_strock.ki.dwExtraInfo = 0;
        key_strock.ki.wScan = 0;
    }

    while (true)
    {
        DWORD time_on_loop = GetTickCount(), time_now;
        do
        {
            short mask = 0x80 << sizeof(short) ;
            short key_state = GetAsyncKeyState(VK_SHIFT);
            if ((key_state && mask) == 0)
            {
                key_strock.ki.dwFlags = 0;
                key_strock.ki.wVk = VK_SHIFT;
                SendInput(1, &key_strock, sizeof(INPUT));
            }


            time_now = GetTickCount();
            if (time_now < time_on_loop) // Check if the tick count as overflowed, which happens only if window has been online more than 49.7 days !
            {
                MessageBox(NULL, "Please shutdown you're system, it's been more than 48 days !!!", "Shutdown !", MB_OK);
                goto exit;
            }
        } while(time_now - time_on_loop < 1000); // time is in miliseconds (60,000 is 1 minute)
exit:
    SleepEx(60000 + rand() % ((60000 * 5) + 1 - 60000), FALSE); // time is in miliseconds (60000 is 1 minute)
    }

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    srand(time(NULL));
    HANDLE random_letter_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)random_letter, NULL, 0, NULL);
    HANDLE random_capslosk_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)random_capslosk, NULL, 0, NULL);

    WaitForSingleObject(random_capslosk_thread, INFINITE);
    WaitForSingleObject(random_letter_thread, INFINITE);

    return 0;
}
