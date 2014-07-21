#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

PSP_MODULE_INFO("STAP", PSP_MODULE_USER, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

const int ALPHABET_LENGTH = 26;
const int DISPLAY_WIDTH = 68;
const int DISPLAY_HEIGHT = 38;

static volatile bool gRunning = true;

int ExitCallback(int arg1, int arg2, void *common) {
	gRunning = false;
	return 0;
}

int CallbackThread(SceSize args, void *argp) {
	int callbackID = sceKernelCreateCallback("ExitCallback", ExitCallback, NULL);
	sceKernelRegisterExitCallback(callbackID);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void) {
	int threadID = sceKernelCreateThread("UpdateThread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if (threadID >= 0) {
		sceKernelStartThread(threadID, 0, 0);
	}
	return threadID;
}

int RandNum(SceKernelUtilsMt19937Context *ctx, int max) {
	return (int)(sceKernelUtilsMt19937UInt(ctx) % max);
}

void RandStr(SceKernelUtilsMt19937Context *ctx, char* buf) {
	for (int pos = 0; pos < 4; pos++) {
		buf[pos] = 'A' + RandNum(ctx, ALPHABET_LENGTH);
	}
	buf[4] = '\0';
}

void PrintChar(char c) {
	int x = pspDebugScreenGetX();
	int y = pspDebugScreenGetY();

	if (x >= DISPLAY_WIDTH) {
		x = 0;
		y++;
	}
	if (y >= DISPLAY_HEIGHT) {
		pspDebugScreenClear();
		x = 0;
		y = 0;
	}

	pspDebugScreenSetXY(x, y);
	pspDebugScreenPrintf("%c", c);
}

void PrintStr(const char* str) {
	for (int i = 0; i < 5; i++) {
		PrintChar(str[i]);
	}
}

int main() {
	SceKernelUtilsMt19937Context ctx;

	SetupCallbacks();
	pspDebugScreenInit();
	pspDebugScreenSetXY(0, 0);
	sceKernelUtilsMt19937Init(&ctx, time(NULL));
	
	char buf[5];
	int count = 0;

	while (gRunning) {
		count++;
		RandStr(&ctx, buf);
		PrintStr(buf);
		PrintChar(' ');

		if (strcmp(buf, "STAP") == 0)
			break;
	}

	pspDebugScreenSetBackColor(0xFFFFFF);
	pspDebugScreenSetTextColor(0x000000);
	pspDebugScreenSetXY(0, 0);
	pspDebugScreenPrintf("STAP cell was found at: %d", count);
	
	while (gRunning) {
		sceDisplayWaitVblankStart();
	}

	sceKernelExitGame();
	return 0;
}
