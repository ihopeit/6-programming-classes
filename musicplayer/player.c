#include <stdio.h>
#include <stdlib.h> // for exit()
#include <unistd.h> // comment this line for Windows
#include <string.h> //strlen
#include "bass.h"

BOOL startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? FALSE : memcmp(pre, str, lenpre) == 0;
}

int main(int argc, char** argv){
    BASS_Init(-1, 44100, 0, 0, NULL);
    if(argc==1){
        printf("Usage: ./play xxx.mp3 \n");
        exit(-1);
    }
    HSTREAM stream = 0;
    if(startsWith("http", argv[1])){
        stream = BASS_StreamCreateURL(argv[1], 0, 0, NULL, 0);
    } else {
        stream = BASS_StreamCreateFile(FALSE, argv[1], 0, 0, 0);
    }

    if(!stream){
        printf("Usage: ./play xxx.mp3 \n");
        printf("Failed to open file, code: %d\n",BASS_ErrorGetCode());
        exit(-2);
    }

    BOOL result = BASS_ChannelPlay(stream, TRUE);
    if(!result){
        printf("Failed to play stream, code: %d\n", BASS_ErrorGetCode());
    }
	
    while (TRUE) {
        usleep(100);
        if(!BASS_ChannelIsActive(stream)){
            BASS_StreamFree(stream);
            break;
        }
    }
	
    return 0;
}
