// David Mattia
// Main function

#include "finalfn.h"

#define WIDTH 800
#define HEIGHT WIDTH

int main() {
	int i, score;
	gfx_open(WIDTH,HEIGHT,"CHIELLINI");
	while((i=mainMenu()) != 3) {
		switch(i) {
			case 1:
				// Play pacman selected
				// Play a game and update high
				// scores if need be
				updateHighScores(playPacman());
				break;
			case 2:
				// Display high scores page
				highScores();
				break;
			case 3:break;
			case 4: aboutSection(); break;
			default:break;
		}
	}
	// Removes sound played output
	system("clear");
}
