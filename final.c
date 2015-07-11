// David Mattia
// Main function

#include "finalfn.h"

#define WIDTH 800
#define HEIGHT WIDTH

enum Menu_Selection {
	play_game = 1,
	high_scores = 2,
	quit = 3,
	about = 4
};

int main() {
	int score;
	enum Menu_Selection selection;
	gfx_open(WIDTH,HEIGHT,"CHIELLINI");
	while((selection=(enum Menu_Selection)mainMenu()) != quit) {
		switch(selection) {
			case play_game:
				// Play pacman selected
				// Play a game and update high
				// scores if need be
				updateHighScores(playPacman());
				break;
			case high_scores:
				// Display high scores page
				highScores();
				break;
			case quit:break;
			case about: aboutSection(); break;
			default:break;
		}
	}
	// Removes sound played output
	system("clear");
}
