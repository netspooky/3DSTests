#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <3ds/allocator/mappable.h>

#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

int telnet(char mybuf[])
{
    int socket_desc;
    struct sockaddr_in server;

    //Create socket
    printf("[ ^ __ ^ ] Creating socket, please wait . . . \n\n");
    socket_desc = socket (AF_INET , SOCK_STREAM , IPPROTO_IP);
    if (socket_desc < 0)
    {
        printf("[ - __ - ] Could not create socket . . .\n\n");
    }

    server.sin_addr.s_addr = inet_addr(mybuf);
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("[ - __ - ] Network error, please check your Wifi connection!\n\n");
        puts("Press Start to return to home menu . . .\n");
        return 1;
    }

    puts("[ ^ __ ^] Connected!\n");
    return 0;
}

int main(int argc, char **argv)
{
	gfxInitDefault();               //Initialize Graphic Output
	consoleInit(GFX_TOP, NULL);     //Initialize Console on Top screen
	printf("==============\n");
	printf("| 3DS Telnet |\n");
	printf("==============\n\n");

	while (aptMainLoop())
	{
		hidScanInput();                         //Scanning input from user
		u32 kDown = hidKeysDown();              //Define keypress

		if (kDown & KEY_START)
			break;

		static SwkbdState swkbd;                //State of Software Keyboard
		static char mybuf[60];                  //Buffer of 60 characters
		static SwkbdStatusData swkbdStatus;     //Status of Software Keyboard
		SwkbdButton button = SWKBD_BUTTON_NONE; //?
		bool didit = false;                     //Flag for keypress


		if (kDown & KEY_A)
		{
			didit = true;
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 3, -1);
			swkbdSetInitialText(&swkbd, mybuf);
			swkbdSetHintText(&swkbd, "Enter Hostname");
			swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Nahh", false);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_MIDDLE, "eh?", true);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "HACK", true);
		    printf("Enter Hostname: ");
			//swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);
			static bool reload = false;
			swkbdSetStatusData(&swkbd, &swkbdStatus, reload, true);
			reload = true;
			button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
		}

		if (didit)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
			    printf("%s\n", mybuf);
                telnet(mybuf);

			} else
				printf("swkbd event: %d\n", swkbdGetResult(&swkbd));
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}
	gfxExit();
	return 0;
}