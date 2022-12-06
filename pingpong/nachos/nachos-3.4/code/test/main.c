#include "syscall.h"

int main() {

	int pingId, pongId;
	PrintString("Ping-Pong: \n");

	pingId = Exec("./test/ping");
	if (pingId == -1)
	{
		return 1;
	}

	pongId = Exec("./test/pong");
	if (pongId == -1)
	{
		return 1;
	}
	Join(pingId);
	Join(pongId);

	PrintString("Finish test\n");
}


