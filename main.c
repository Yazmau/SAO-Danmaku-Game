#include "lib/data.h"
#include "lib/debug.h"
#include "lib/function.h"
#include "lib/imgpro.h"
#include "lib/init.h"
#include "lib/game.h"

int main() {
	allegro_init();
	game_init();
	event_loop();
	return 0;
}
