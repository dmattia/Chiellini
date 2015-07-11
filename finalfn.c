// David Mattia
// Functions for final.c

#include <stdio.h>
#include "gfx3.h"
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "finalfn.h"

#define WIDTH 800
#define HEIGHT WIDTH
#define SECTIONS 20
#define SECTION WIDTH / SECTIONS
#define RADIUS SECTION / 2

#define WHITE gfx_color(255,255,255)
#define GREEN gfx_color(0,146,70)
#define YELLOW gfx_color(255,255,0)
#define RED gfx_color(206,43,55)
#define BLUE gfx_color(0,0,255)
#define PINK gfx_color(250,10,240)
#define CYAN gfx_color(10,240,230)
#define ORANGE gfx_color(255,140,0)
#define LIGHT_BACKGROUND gfx_color(238,232,170)
#define BEVEL gfx_color(220,216,157);
#define BLACK gfx_color(0,0,0);

enum directions {noDirection, Right, Up, Left, Down};

// Creates the how to play display
void aboutSection() {
	Box backBox;
	char c, line[80]; //
	int x,y,lineNumber=1;
	FILE *fin;

	// Draw background and boxes
	soccerBackground();
	drawBox(WIDTH/2,HEIGHT*4/10,WIDTH*3/4,HEIGHT*3/4);
	backBox = drawBox(WIDTH/2,HEIGHT*9/10,WIDTH*3/4,HEIGHT/8);

	// Draw text in boxes	
	RED;
	centeredText(WIDTH/2,HEIGHT*9/10,"Back");

	// Read from file
	fin = fopen("howtoplay.txt","r");	
	while(fgets(line,sizeof(line),fin)) {
		line[strlen(line)-1] = '\0';
		// +2 is added to lineNumber for display purposes
		centeredText(WIDTH/2,HEIGHT*(lineNumber+2)/20,line);
		lineNumber++;
	}
	
	// Respond to user events
	while(1) {
		// Wait for user input
		c = gfx_wait();
		x = gfx_xpos();
		y = gfx_ypos();
	
		if(c==1 && isInBox(backBox,x,y)) {
			return;	
		} // else do nothing
	}

	fclose(fin);
}

// Displays options to play game, open high scores, or exit
enum Menu_Selection mainMenu() {
	char c = 0;
	int x=0,y=0,i;	
	Box titleBox, playNowBox, highScoresBox, quitBox, aboutBox;

	soccerBackground();

	// Draw boxes
	LIGHT_BACKGROUND;

	titleBox = drawBox(WIDTH/2,HEIGHT/4,WIDTH*3/4,HEIGHT/4);
	playNowBox = drawBox(WIDTH/2,HEIGHT/2,WIDTH*3/4,HEIGHT/8);
	highScoresBox = drawBox(WIDTH*19.5/64,HEIGHT*13/20,WIDTH*23/64,HEIGHT/8);
	aboutBox = drawBox(WIDTH*44.5/64,HEIGHT*13/20,WIDTH*23/64,HEIGHT/8);
	quitBox = drawBox(WIDTH/2,HEIGHT*4/5,WIDTH*3/4,HEIGHT/8);

	// Draw text in boxes
	RED;
	centeredText(WIDTH/2,HEIGHT/4,"Chiellini");
	centeredText(WIDTH/2,HEIGHT/2,"Play Now!");
	centeredText(WIDTH*19.5/64,HEIGHT*13/20,"High Scores");
	centeredText(WIDTH*44.5/64,HEIGHT*13/20,"How To Play");
	centeredText(WIDTH/2,HEIGHT*4/5,"Quit");

	drawPacMan(5,5,1);
	drawPacMan(15,5,1);

	// Respond to click events
	while(1) {
		c=gfx_wait();
		x=gfx_xpos();
		y=gfx_ypos();

		if(c==1 && (isInBox(playNowBox,x,y) || isInBox(titleBox,x,y))) {
			// Play a game of pacman
			return play_game; 
		} else if(c==1 && isInBox(highScoresBox,x,y)) {
			// Show high scores
			return high_scores;
		} else if(c==1 && isInBox(aboutBox,x,y)) {
			// Show how to play section
			return about;
		} else if(c==1 && isInBox(quitBox,x,y)) {
			// Quit
			return quit;
		}
	} 
}

// Draws a basic soccer field as the background
void soccerBackground() {
	int thickness;

	WHITE;
	gfx_clear_color(0,146,70); // green
	gfx_clear();
	gfx_clear_color(0,0,0); // black

	// loop makes the lines thicker
	for(thickness=-2;thickness<=2;thickness++) {
		// center line
		gfx_line(0,HEIGHT/2+thickness,WIDTH,HEIGHT/2);
		gfx_circle(WIDTH/2,HEIGHT/2,WIDTH/5+thickness);	
	
		// bottom goal box
		gfx_line(WIDTH/4+thickness,HEIGHT,WIDTH/4+thickness,HEIGHT*4/5);
		gfx_line(WIDTH*3/4+thickness,HEIGHT,WIDTH*3/4+thickness,HEIGHT*4/5);
		gfx_line(WIDTH/4,HEIGHT*4/5+thickness,WIDTH*3/4,HEIGHT*4/5+thickness);
		
		// top goal box
		gfx_line(WIDTH/4+thickness,0,WIDTH/4+thickness,HEIGHT/5);
		gfx_line(WIDTH*3/4+thickness,0,WIDTH*3/4+thickness,HEIGHT/5);
		gfx_line(WIDTH/4,HEIGHT/5+thickness,WIDTH*3/4,HEIGHT/5+thickness);
	
		// corners
		gfx_circle(0,HEIGHT,WIDTH/100+thickness);
		gfx_circle(0,0,WIDTH/100+thickness);
		gfx_circle(WIDTH,0,WIDTH/100+thickness);
		gfx_circle(WIDTH,HEIGHT,WIDTH/100+thickness);
	}
}

// Draws the score at the top of the screen
void drawScore(int score) {
	WHITE;
	char s[10];
	// covert int score to a char array
	sprintf(s,"%i",score);
	centeredText(WIDTH/2,15,s);
}

// Centers text at a given location.  Simplifies the process of displaying text
// regardless of length
void centeredText(int xCenter, int yCenter, char str[]) {
	int length = strlen(str),x,y;
	x = xCenter - length * 3;
	y = yCenter;
	gfx_text(x,y,str);
}

// Displays the high scores
void highScores() {
	char line[80],c;
	int lineNumber = 1,x,y;
	Box backBox;
	FILE *highScores;
	highScores = fopen("highscores.txt","r");

	soccerBackground();
	
	// get high scores from file
	while(fgets(line,sizeof(line),highScores)) {
		line[strlen(line)-1] = '\0';
		LIGHT_BACKGROUND;
		drawBox(WIDTH/2,HEIGHT*lineNumber/10,WIDTH*3/4,HEIGHT/15);
		RED;
		centeredText(WIDTH/2,HEIGHT*lineNumber/10,line);
		drawPacMan(5,lineNumber*2,1);
		lineNumber++;
	}
	fclose(highScores);

	// create back button
	LIGHT_BACKGROUND;
	backBox = drawBox(WIDTH/2,HEIGHT*9/10,WIDTH*3/4,HEIGHT/10);
	BLACK;
	centeredText(WIDTH/2,HEIGHT*9/10,"Back");

	gfx_flush();
	
	// respond to click events
	while(1) {
		c = gfx_wait();
		x = gfx_xpos();
		y = gfx_ypos();

		if(c==1 && isInBox(backBox,x,y))
			return;
	}	
}

// Changes high scores if new score should be a high score
void updateHighScores(int newScore) {
	char line[80], newName[40], numbers[8][5], names[8][40],scoreStr[8][10];
	int i=0,j,scores[8];
	FILE *highScores;

	// Getting current scores
	highScores = fopen("highscores.txt","r");
	while(fgets(line,sizeof(line),highScores)) {
		sscanf(line, "%s %s %s", numbers[i],names[i],scoreStr[i]);
		scores[i] = atoi(scoreStr[i]);
		i++;
	}
	fclose(highScores);

	// Update array if needed
	if(newScore > scores[7]) {
		for(i=0; newScore <= scores[i]; i++) {}
		for(j=8; j>=i; j--) {
			scores[j] = scores[j-1];
			strcpy(names[j],names[j-1]);
		}
		scores[i] = newScore;
	
		newHighScore(newName);	
		strcpy(names[i],newName);
		strcat(names[i],":");
	}
	highScores = fopen("highscores.txt","w");
	for(j=0;j<8;j++) {
		sprintf(numbers[j],"%i",j+1);
		strcat(numbers[j],".");
		sprintf(scoreStr[j],"%i",scores[j]);
		fprintf(highScores,"%s %s %s\n",numbers[j],names[j],scoreStr[j]);
	}
	fclose(highScores);
}

// Takes in a word.  Returns that word ending at the character before
// the first non-alpha character
void trim(char word[]) {
	int i;
	for(i=0;i<strlen(word);i++) {
		if(!isalpha(word[i])) {
			word[i] = '\0';
			return;
		}
	}
}

// asks user to enter their name when they acheive a high score
void newHighScore(char newName[]) {
	int i=0;
	Box textBox, submitBox;	

	newName[0] = '\0';

	soccerBackground();

	LIGHT_BACKGROUND;
	drawBox(WIDTH/2,HEIGHT/3,WIDTH*3/4,HEIGHT/4);
	textBox = drawBox(WIDTH/2,HEIGHT*3/5,WIDTH*3/4,HEIGHT/8);
	submitBox = drawBox(WIDTH/2,HEIGHT*29/40,WIDTH*3/4,HEIGHT/16);
	RED;
	centeredText(WIDTH/2,HEIGHT/3,"New High Score! Enter your name:");
	centeredText(WIDTH/2,HEIGHT*29/40,"Press enter to submit");	

	while((newName[i++] = gfx_wait()) != 13 || newName[0]==13) {
		newName[i] = '\0';
	
		// Disallow non-printables and capitalize first letter
		if(newName[i-1]==8) {
			// was a backspace
			newName[--i] = '\0';
			i = (i<1) ? 1 : i;
			newName[--i] = '\0';
		} else if(i>10) {
			// name is too long
			newName[--i]='\0';
		} else if(newName[i-1]=='S' || newName[i-1]=='R' || newName[i-1]=='Q' || newName[i-1]=='T') {
			// was an arrow key
			newName[--i] = '\0';
		} else if(!isalpha(newName[i-1])) {
			// was not printable
			newName[--i] = '\0';
		} else if(i==1) {
			// was first key
			if(newName[0]==32 || newName[0]==13) {
				// first key was a space or enter
				newName[--i] = '\0';
			} else {
				// was a valid first key
				newName[i-1] = toupper(newName[i-1]);
			}
		}
	
		LIGHT_BACKGROUND;
		drawBox(WIDTH/2,HEIGHT/3,WIDTH*3/4,HEIGHT/4);
		textBox = drawBox(WIDTH/2,HEIGHT*3/5,WIDTH*3/4,HEIGHT/8);
		submitBox = drawBox(WIDTH/2,HEIGHT*29/40,WIDTH*3/4,HEIGHT/16);
		RED;
		centeredText(WIDTH/2,HEIGHT/3,"New High Score! Enter your name:");
		centeredText(WIDTH/2,HEIGHT*29/40,"Press enter to submit");		
		centeredText(WIDTH/2,HEIGHT*3/5,newName);
	}
	
	trim(newName);
}

// Draws a box at a given center, height, and width. Returns the upper left corner
// location and bottom right corner locations in a Box.
Box drawBox(int xCenter, int yCenter, int width, int height) {
	int i, verticalMargin=height/15, horizMargin=width/15;
	Box box;

	// Make the margins equal to the smaller of the two
	horizMargin = (horizMargin > verticalMargin) ? verticalMargin : horizMargin;
	verticalMargin = (verticalMargin > horizMargin) ? horizMargin : verticalMargin;
	
	for(i=0;i<height;i++) {
		LIGHT_BACKGROUND;
		gfx_line(xCenter-width/2,yCenter-height/2+i,xCenter+width/2,yCenter-height/2+i);
		BEVEL;
		if(i<verticalMargin || i>height-verticalMargin) {
			gfx_line(xCenter-width/2,yCenter-height/2+i,xCenter+width/2,yCenter-height/2+i);
		} else {
			gfx_line(xCenter-width/2,yCenter-height/2+i,xCenter-width/2+horizMargin,yCenter-height/2+i);	
			gfx_line(xCenter+width/2-horizMargin,yCenter-height/2+i,xCenter+width/2,yCenter-height/2+i);	
		}
	}
	box.upperleftX = xCenter - width/2;
	box.upperleftY = yCenter - height/2;
	box.bottomrightX = xCenter + width/2;
	box.bottomrightY = yCenter + height/2;
	
	// Draw the 3d border
	BLACK;
	gfx_line(box.upperleftX+horizMargin,box.upperleftY+verticalMargin,
		box.bottomrightX-horizMargin,box.upperleftY+verticalMargin);
	gfx_line(box.bottomrightX-horizMargin,box.upperleftY+verticalMargin,
		box.bottomrightX-horizMargin,box.bottomrightY-verticalMargin);
	gfx_line(box.bottomrightX-horizMargin,box.bottomrightY-verticalMargin,
		box.upperleftX+horizMargin,box.bottomrightY-verticalMargin);
	gfx_line(box.upperleftX+horizMargin,box.bottomrightY-verticalMargin,
		box.upperleftX+horizMargin,box.upperleftY+verticalMargin);

	// Draw lines from corners to border
	gfx_line(box.upperleftX,box.upperleftY,box.upperleftX+horizMargin,box.upperleftY+verticalMargin);
	gfx_line(box.upperleftX,box.bottomrightY,box.upperleftX+horizMargin,box.bottomrightY-verticalMargin);
	gfx_line(box.bottomrightX,box.upperleftY,box.bottomrightX-horizMargin,box.upperleftY+verticalMargin);
	gfx_line(box.bottomrightX,box.bottomrightY,box.bottomrightX-horizMargin,box.bottomrightY-verticalMargin);

	return box;
}

// returns if a location is in a box
int isInBox(Box box, int x, int y) {
	return x > box.upperleftX && x < box.bottomrightX && y > box.upperleftY && y < box.bottomrightY;
}

// Starts a new game of pacman
int playPacman() {
	int i,canDie=1,elapsed, previousElapsed, poweredUpElapsed=6, lives = 3, score=0,
		count=0, x,y,state=0,dir=0,poweredUp=0,nextDir=0;
	char direction = 1;
	static int ghostsEaten=0;
	int board[SECTIONS][SECTIONS] = {0};
	ghost Suarez,Randy,Luis,Digger, Ghosts[4] = {Suarez, Randy, Luis, Digger};

	#ifdef __APPLE__
		system("afplay sounds/pacman_beginning.wav &");
	#elif
		system("aplay sounds/pacman_beginning.wav &");
	#endif

	gfx_clear();
	setupBoard(board);	
	reset(Ghosts,&x,&y,&direction,lives,board);

	// Creates timer for game
	time_t start_t, end_t, power_t;
	time(&start_t);

	while(direction != 'q' && lives > 0) {
		switch(direction) {
			case 'S': nextDir = 1; break; // right arrow
			case 'R': nextDir = 2; break; // up arrow
			case 'Q': nextDir = 3; break; // left arrow
			case 'T': nextDir = 4; break; // down arrow
			case 'p': direction = gfx_wait();break; // pause
			default: break;
		}

		// Get current time since start of game
		time(&end_t);
		elapsed = (int)(difftime(end_t,start_t));		
		if(poweredUp) {
			poweredUpElapsed = (int)(difftime(end_t,power_t));
		}

		// Plays chomp.wav every second
		if(elapsed>previousElapsed) {
			#ifdef __APPLE__
				system("afplay sounds/pacman_chomp.wav &");
			#elif
				system("afplay sounds/pacman_chomp.wav &");
			#endif
		}		

		// Prepare board
		gfx_clear();	
		drawLives(lives);
		drawBoard(board);
		drawScore(score);
		state = count++ % 2;

		// Update pacman's posiiton
		updatePosition(&x,&y,&dir,nextDir,board);
		if(canDie && pacmanDied(Ghosts,x,y)) {
			if(poweredUpElapsed>=5 && board[x][y]!=3) {
				// if hit ghost without powerup
				lives--;
				#ifdef __APPLE__
					system("afplay sounds/pacman_death.wav &");
				#elif
					system("aplay sounds/pacman_death.wav &");
				#endif
				reset(Ghosts,&x,&y,&direction,lives,board);
				time(&start_t);
			} else {
				// if it ghost with powerup
				// places all eaten ghosts to (9,9)
				for(i=0;i<4;i++) {
					if(Ghosts[i].x==x && Ghosts[i].y==y) {
						Ghosts[i].x = 9;
						Ghosts[i].y = 9;
						#ifdef __APPLE__
							system("afplay sounds/pacman_eatghost.wav &");
						#elif
							system("aplay sounds/pacman_eatghost.wav &");
						#endif
						score+=100*(pow(2,ghostsEaten++));
					}	
				}
			}
		}
		
		// Move all ghosts
		moveGhosts(Ghosts,elapsed,poweredUpElapsed,x,y,board);

		// Draw sprites
		drawGhosts(Ghosts,x,y,poweredUpElapsed);
		drawPacMan(x,y,state*dir);

		// Check if pacman ate anything
		if(board[x][y] == 3) {
			score+=50;
			ghostsEaten=0;
			poweredUp=1;
			time(&power_t);
		} else if(board[x][y]==0) {
			score+=10;
		}

		// Flush all drawings and sleep for a bit. zzz.
		gfx_flush();
		usleep(200000);

		// Check status for winning
		if (isWon(board)) {
			setupBoard(board);
			#ifdef __APPLE__
				system("afplay sounds/pacman_intermission.wav &");
			#elif
				system("aplay sounds/pacman_intermission.wav &");
			#endif
			reset(Ghosts,&x,&y,&direction,lives,board);
		}

		if(canDie && pacmanDied(Ghosts,x,y)) {
			if(poweredUpElapsed>=5 && board[x][y]!=3) {
				// Met a ghost without powerup
				lives--;
				#ifdef __APPLE__
					system("afplay sounds/pacman_death.wav &");
				#elif
					system("aplay sounds/pacman_death.wav &");
				#endif
				reset(Ghosts,&x,&y,&direction,lives,board);
				time(&start_t);
			} else {
				// Met a ghost with powerup. Eat 'em.
				for(i=0;i<4;i++) {
					if(Ghosts[i].x==x && Ghosts[i].y==y) {
						// sends all eaten ghosts to (9,9)
						Ghosts[i].x = 9;
						Ghosts[i].y = 9;
						#ifdef __APPLE__
							system("afplay sounds/pacman_eatghost.wav &");
						#elif
							system("aplay sounds/pacman_eatghost.wav &");
						#endif
						score+=100*(pow(2,ghostsEaten++));
					}
				}
			}
		}

		board[x][y] = 2;
	
		// This is set here to see if a second has elapsed by the next iteration
		// If it has, a sound will be played
		previousElapsed = elapsed;

		// check for a new click
		if(gfx_event_waiting())
			direction = gfx_wait();
	}

	// Chiellini/Pacman has used all of his lives
	return score;
}

// Sets chiellini/pacman in his starting positions as well as all ghosts.
void reset(ghost Ghosts[4],int *x, int *y, char *direction, int lives, int board[][SECTIONS]) {
	int i;
	*x = 9;
	*y = 14;
	*direction = 'h';
	for(i=0;i<4;i++) {
		Ghosts[i].x = i+7;
		Ghosts[i].y = 9;
		Ghosts[i].dir = 0;
	}
	Ghosts[0].x+=2;
	Ghosts[0].y--;
	gfx_clear();
	drawBoard(board);
	drawLives(lives);
	drawGhosts(Ghosts,*x,*y,6);
	drawPacMan(*x,*y,0);

	if (lives > 0) {
		sleep(3);
		while(*direction != 'Q' && *direction != 'S' && *direction != 'q') {
			*direction = gfx_wait();
		}
	}
}

// Draws how many lives remain in the bottom left hand corner.
// Lives lost go to the coroner.
void drawLives(int lives) {
	int i;
	for(i=0;i<lives;i++) {
		drawPacMan(i+2,19,1);
	}
}

// Moves all ghosts
void moveGhosts(ghost Ghosts[4],int time, int poweredUpElapsed, int pacmanX, int pacmanY, int board[][SECTIONS]) {
	int totalElapsed=time,i;
	static int slow=0; // Scared ghosts can only move if this is even
	time%=14;
	
	// Move Suarez / Ghost1
	if(totalElapsed > 0) {
		if(poweredUpElapsed < 5 && !(Ghosts[0].x==9 && Ghosts[0].y==9) && slow%2==0) {
			updateGhost(20-pacmanX,20-pacmanY,&Ghosts[0].x,&Ghosts[0].y,&Ghosts[0].dir,board);
		} else if(poweredUpElapsed < 5) {
			// Sit Still
		} else if(time < 7) {
			updateGhost(pacmanX,pacmanY,&Ghosts[0].x,&Ghosts[0].y,&Ghosts[0].dir,board);
		} else {
			updateGhost(1,1,&Ghosts[0].x,&Ghosts[0].y,&Ghosts[0].dir,board);
		}
	} else {
		Ghosts[0].x = 8;
		Ghosts[0].y = 8;
	}

	// Move Randy / Ghost2
	if(totalElapsed > 1) {
		if(poweredUpElapsed < 5 && !(Ghosts[1].x==9 && Ghosts[1].y==9) && slow%2==0) {
			updateGhost(20-pacmanX,20-pacmanY,&Ghosts[1].x,&Ghosts[1].y,&Ghosts[1].dir,board);
		} else if(poweredUpElapsed < 5) {
			// Sit Still
		} else if(time < 10) {
			updateRandomGhost(&Ghosts[1].x,&Ghosts[1].y,&Ghosts[1].dir,board);
		} else {
			int corner = rand() % 4;
			updateGhost(corner%2*16+1,(corner<2) ? 1 : 18,&Ghosts[1].x,&Ghosts[1].y,&Ghosts[1].dir,board);
		}
	} else {
		Ghosts[1].x = 8;
		Ghosts[1].y = 9;
	}

	// Move Luis / Ghost3
	if(totalElapsed > 3) {
		if(poweredUpElapsed < 5 && !(Ghosts[2].x==9 && Ghosts[2].y==9) && slow%2==0) {
			updateGhost(20-pacmanX,20-pacmanY,&Ghosts[2].x,&Ghosts[2].y,&Ghosts[2].dir,board);
		} else if(poweredUpElapsed < 5) {
			// Sit Still
		} else if(time < 5 || (time > 7 && time < 12)) {
			updateGhost(pacmanX,pacmanY,&Ghosts[2].x,&Ghosts[2].y,&Ghosts[2].dir,board);
		} else {
			updateRandomGhost(&Ghosts[2].x,&Ghosts[2].y,&Ghosts[2].dir,board);
		}
	} else {
		Ghosts[2].x = 9;
		Ghosts[2].y = 9;
	}

	// Move Digger / Ghost4
	if(totalElapsed > 5) {
		if(poweredUpElapsed < 5 && !(Ghosts[3].x==9 && Ghosts[3].y==9) && slow%2==0) {
			updateGhost(20-pacmanX,20-pacmanY,&Ghosts[3].x,&Ghosts[3].y,&Ghosts[3].dir,board);
		} else if(poweredUpElapsed < 5) {
			// Sit Still
		} else if(time < 4) {
			updateGhost(1,9,&Ghosts[3].x,&Ghosts[3].y,&Ghosts[3].dir,board);
		} else if(time < 11) {
			updateGhost(pacmanX,pacmanY,&Ghosts[3].x,&Ghosts[3].y,&Ghosts[3].dir,board);
		} else {
			updateGhost(17,18,&Ghosts[3].x,&Ghosts[3].y,&Ghosts[3].dir,board);
		}
	} else {
		Ghosts[3].x=10;
		Ghosts[3].y=9;
	}

	slow++;
}

// draws each ghost sprite
void drawGhosts(ghost Ghosts[4],int pacmanX, int pacmanY, int elapsed) {
	int i;
	for(i=0;i<4;i++)
		drawGhost(Ghosts[i].x,Ghosts[i].y,(elapsed>=5)*(i+1),pacmanX,pacmanY);
}

// checks if there are any uneaten pebbles. Returns true if none remain, false otherwise
int isWon(int board[][SECTIONS]) {
	int i,j,completed=1;
	for(i=0;i<SECTIONS;++i)
		for(j=0;j<SECTIONS;++j)
			if(board[i][j]==0)
				completed=0;
	return completed;
}

// Checks if pacman's posiiton equals any of the ghosts positions.
// Note: Pacman does not always die, as he can have his powerup activated.
int pacmanDied(ghost Ghosts[4], int x, int y) {
	int i;
	for(i=0;i<4;i++)
		if(Ghosts[i].x==x && Ghosts[i].y==y)
			return 1;
	return 0;
}

// Pauses the game. Prominent for early testing, but no longer needed
// after creation of menu.
void gameOver(int board[][SECTIONS]) {
	usleep(300000);
}


// sets up the board/field pacman moves on based on a file
void setupBoard(int board[][SECTIONS]) {
	FILE *boardInit;
	int i,j;
	char c;
	boardInit = fopen("board.txt","r");
	if(!boardInit) {
		printf("Could not find board.txt");
	}
	char line[256];
	for(i=0;i<20;i++) {
		for(j=0;j<20;j++) {
			fscanf(boardInit, "%d ", &board[j][i]);
			board[j][i] = board[j][i];
		}
	}
	board[0][0]=1;
}

// Adds a barrier in a given location.  Currently is never called. 
// Mantains value for testing purposes.
void addBarrier(int x,int y,int board[][SECTIONS]) {
	board[x][y] = 1;
}

// Draws the board based on the board array.
void drawBoard(int board[][SECTIONS]) {
	int i,j,k;
	for(i=0; i<SECTIONS; i++) {
		for(j=0; j<SECTIONS; j++) {
			if(board[i][j]==0) {
				WHITE;
				gfx_circle(i*SECTION,j*SECTION,2);
			} else
			if(board[i][j]==1) {
				BLUE;
				if(j-1>=0 && (board[i][j-1]!=1))
					gfx_line(i*SECTION-RADIUS,j*SECTION-RADIUS,i*SECTION+RADIUS,j*SECTION-RADIUS);
				if(i+1<=39 && (board[i+1][j]!=1))
					gfx_line(i*SECTION+RADIUS,j*SECTION-RADIUS,i*SECTION+RADIUS,j*SECTION+RADIUS);
				if(j+1<=39 && (board[i][j+1]!=1))
					gfx_line(i*SECTION+RADIUS,j*SECTION+RADIUS,i*SECTION-RADIUS,j*SECTION+RADIUS);
				if(i-1>=0 && (board[i-1][j]!=1))
					gfx_line(i*SECTION-RADIUS,j*SECTION+RADIUS,i*SECTION-RADIUS,j*SECTION-RADIUS);
			} else
			if(board[i][j]==3) {
				RED;
				for(k=0;k<30;k++) {
					gfx_line((i*SECTION-RADIUS)+RADIUS/3,(j*SECTION-RADIUS)+k,(i*SECTION+RADIUS)-RADIUS/3,(j*SECTION-RADIUS)+k);
				}
			}
		}
	}
}

// Updates position and direction for pacman or ghosts
void updatePosition(int *x, int *y, int *dir, int nextDir, int board[][SECTIONS]) {
	//Changing direction if doesn't run into a wall
	switch(nextDir) {
		case noDirection:break;
		case Right: if(board[*x+1][*y] != 1) *dir = nextDir;break;
		case Up: if(board[*x][*y-1] != 1) *dir = nextDir;break;
		case Left: if(board[*x-1][*y] != 1) *dir = nextDir;break;
		case Down: if(board[*x][*y+1] != 1) *dir = nextDir;break;
		default: break;
	}
	//Updates pacmans position
	switch(*dir) {
		case noDirection:break;
		case Right: if(board[*x+1][*y] != 1) *x = *x+1;break;
		case Up: if(board[*x][*y-1] != 1) *y = *y-1;break;
		case Left: if(board[*x-1][*y] != 1) *x = *x-1;break;
		case Down: if(board[*x][*y+1] != 1) *y = *y+1;break;
		default: break;
	}
	//Sends from left/right of screen to right/left
	if(*x == 20 && *dir == Right) {
		*x=0;
	} else if (*x == -1 && *dir == Left) {
		*x=19;
	}
}

// Updates a ghost's position based on a target at (xTarget,yTarget).
void updateGhost(int xTarget, int yTarget, int *xCurrent, int *yCurrent, int *dir, int board[][SECTIONS]) {
	int x = *xCurrent, y = *yCurrent, xDif, absXDif, yDif, absYDif, topChoice, nextChoice, lastChoice,
		possibleDirs[4] = {0}, i;

	// Check if at a branch point with more than two possible moves
	if(board[*xCurrent+1][*yCurrent]!=1) {
		possibleDirs[0]++;
	}
	if(board[*xCurrent-1][*yCurrent]!=1) {
		possibleDirs[2]++;
	}
	if(board[*xCurrent][*yCurrent+1]!=1) {
		possibleDirs[3]++;
	}
	if(board[*xCurrent][*yCurrent-1]!=1) {
		possibleDirs[1]++;
	}

	// Find best and next best options to move to
	xDif = xTarget - *xCurrent;
	yDif = yTarget - *yCurrent;
	absXDif = (xDif>0) ? xDif : -xDif;
	absYDif = (yDif>0) ? yDif : -yDif;
	if (absXDif > absYDif) {
		topChoice = (xDif>0) ? 1 : 3;
		nextChoice = (yDif>0) ? 4 : 2;
	} else {
		topChoice = (yDif>0) ? 4 : 2;
		nextChoice = (xDif>0) ? 1 : 3;
	}

	// Remove possibility of moving backwards
	switch(*dir) {
		case Right: possibleDirs[2]--;break;
		case Up:possibleDirs[3]--;break;
		case Left: possibleDirs[0]--;break;
		case Down: possibleDirs[1]--;break;
		default:break;
	}

	// Move in best possible direction
	if(possibleDirs[topChoice-1] == 1) {
		updatePosition(xCurrent, yCurrent, &topChoice, 0, board);
		*dir = topChoice;
	} else if (possibleDirs[nextChoice-1] == 1) {
		updatePosition(xCurrent, yCurrent, &nextChoice, 0, board);
		*dir = nextChoice;
	} else {
		possibleDirs[topChoice-1]--;
		possibleDirs[nextChoice-1]--;
		for(i=1;i<5;++i) {
			if(possibleDirs[i-1]==1) {
				*dir = i;
				updatePosition(xCurrent,yCurrent,&i,0,board);
				break;
			}
		}
	}
}

// Ghost picks a random location to move to at any node, but never moves backwards
void updateRandomGhost(int *x, int *y, int *dir, int board[][SECTIONS]) {
	int i, hasMoved = 0;
	while(!hasMoved) {
		i = rand() % 4 + 1;//Picks a random direction
		switch(i) {
			case Right: 
				if(board[*x+1][*y] != 1 && *dir!=3) {
					*x = *x+1;
					*dir = 1;
					hasMoved++;
				}
				break;
			case Up: 
				if(board[*x][*y-1] != 1 && *dir!=4) {
					*y = *y-1;
					*dir = 2;
					hasMoved++;
				}
				break;
			case Left: 
				if(board[*x-1][*y] != 1 && *dir!=1) {
					*x = *x-1;
					*dir = 3;
					hasMoved++;
				}
				break;
			case Down: 
				if(board[*x][*y+1] != 1 && *dir!=2) {
					*y = *y+1;
					*dir=4;
					hasMoved++;
				}
				break;
			default:break;
		}
	}
	// In case going through tunnel
	if(*x==19 && i==Right) {
		*x=0;
	} else if (*x==0 && i==Left) {
		*x=19;
	}
}

// Draws pacman at (x,y) with italian colors
void drawPacMan(int x, int y, int state) {
	double theta_change = 2 * M_PI / 1000, end_theta = 2 * M_PI;
	double shift = (double)(2*state-1) * M_PI / 4.0;
	double current_theta = 0 + shift, next_theta = current_theta + theta_change;
	
	GREEN;
	
	x*=SECTION;
	y*=SECTION;
	if(state != 0) {
		end_theta = 3 * M_PI / 2.0;
	}
	
	while (current_theta < end_theta + shift) {
		if(current_theta > (shift+end_theta)/3.0)
			WHITE;
		if(current_theta > (shift+end_theta)*2.0/3)
			RED;
		gfx_line(x+RADIUS*cos(current_theta),y-RADIUS*sin(current_theta),x+RADIUS*cos(next_theta),y-RADIUS*sin(next_theta));
		current_theta = next_theta;
		next_theta += theta_change;
		gfx_line(x,y,x+RADIUS*cos(current_theta),y-RADIUS*sin(current_theta));
	}
}

// Draws a ghost at (x,y)
void drawGhost(int x, int y, int state, int pacmanX, int pacmanY) {
	double theta = 0, dtheta = M_PI / 20.0,temp;
	int i;

	x*=SECTION;
	y*=SECTION;

	//Pick the ghosts color	
	switch(state) {
		case 0:BLUE;break;
		case 1:RED;break;
		case 2:PINK;break;
		case 3:CYAN;break;
		case 4:ORANGE;break;
	}

	//head
	while(theta < M_PI) {
		gfx_line(x+RADIUS*cos(theta),y-RADIUS*sin(theta),x+RADIUS*cos(theta+dtheta),y-RADIUS*sin(theta+dtheta));
		theta += dtheta;
	}

	//theta towards pacman
	theta = atan(((double)(y/(int)(SECTION)-pacmanY)) / (pacmanX-x/(int)(SECTION)));
	if(x/(int)(SECTION) > pacmanX)
		theta+=M_PI;

	//sides
	gfx_line(x-RADIUS,y,x-RADIUS,y+RADIUS);
	gfx_line(x+RADIUS,y,x+RADIUS,y+RADIUS);

	//bottom
	for(i=0;i<=3;++i) {
		gfx_line(x+(RADIUS/2.0*i-RADIUS),y+RADIUS,x+(RADIUS/2.0*(i+.5)-RADIUS),y+RADIUS*3.0/4);
		gfx_line(x+(RADIUS/2.0*(i+.5)-RADIUS),y+RADIUS*3.0/4,x+(RADIUS/2.0*(i+1)-RADIUS),y+RADIUS);
	}

	//eyes
	gfx_circle(x-RADIUS/3.0,y-RADIUS/2.0,RADIUS/4.0);
	gfx_circle(x+RADIUS/3.0,y-RADIUS/2.0,RADIUS/4.0);
	
	//pupil
	gfx_circle(x-RADIUS/3.0+RADIUS/8.0*cos(theta),y-RADIUS/2.0-RADIUS/8.0*sin(theta),RADIUS/8.0);
	gfx_circle(x+RADIUS/3.0+RADIUS/8.0*cos(theta),y-RADIUS/2.0-RADIUS/8.0*sin(theta),RADIUS/8.0);
}
