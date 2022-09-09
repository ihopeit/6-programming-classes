# download bass lib, and copy the dynamic libray to system direcotry such as /usr/local/lib
# http://www.un4seen.com/

default: player.out

player.out: player.c bass.h
	gcc -o player.out player.c -lbass

clean:
	rm *.out
