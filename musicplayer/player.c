#include <stdio.h>
#include <unistd.h> // Windows: #include <windows.h>,  replace usleep() -> Sleep()
#include <string.h> //strlen
#include "bass.h"

BOOL startsWith(const char *pre, const char *str){
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? FALSE : memcmp(pre, str, lenpre) == 0;
}

int main(int argc, char** argv){
    BASS_Init(-1, 44100, 0, 0, NULL); 
    if(argc==1){
        printf("Usage: ./play xxx.mp3 yyy.mp3 \n");
        return -1;
    }
    HSTREAM stream = 0;

    for(int i=1; i<=argc; i++){
        printf("Start playing: %s \n", argv[i]);
        if(startsWith("http", argv[i])){
            stream = BASS_StreamCreateURL(argv[i], 0, 0, NULL, 0);
        } else {
            stream = BASS_StreamCreateFile(FALSE, argv[i], 0, 0, 0);
        }

        if(!stream){
            printf("Usage: ./play xxx.mp3 \n");
            printf("Failed to open file, %s code: %d\n", argv[1], BASS_ErrorGetCode());
            return -2;
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
    }
	
    return 0;
}
