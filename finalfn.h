// David Mattia
// Function declarations
// Also defines structs

#define SECTIONS 20

typedef struct {
	int x;
	int y;
	int dir;
} ghost;

// Makes clickable rectangular boxes
typedef struct {
	int upperleftX;
	int upperleftY;
	int bottomrightX;
	int bottomrightY;
} Box;

void drawPacMan(int,int,int);
void drawGhost(int,int,int,int,int);
void updatePosition(int*,int*,int*,int,int[][SECTIONS]);
void addBarrier(int,int,int[][SECTIONS]);
void drawBoard(int[][SECTIONS]);
void setupBoard(int[][SECTIONS]);
void gameOver(int[][SECTIONS]);
int isWon(int[][SECTIONS]);
int pacmanDied(ghost[],int,int);
void updateRandomGhost(int*,int*,int*,int[][SECTIONS]);
void updateGhost(int,int,int*,int*,int*,int[][SECTIONS]);
void drawGhosts(ghost[],int,int,int);
void moveGhosts(ghost[],int,int,int,int,int[][SECTIONS]);
void drawLives(int);
void reset(ghost[],int*,int*,char*,int,int[][SECTIONS]);
int playPacman();
int mainMenu();
Box drawBox(int,int,int,int);
int isInBox(Box,int,int);
void highScores();
void centeredText(int,int,char[]);
void drawScore(int);
void updateHighScores(int);
void newHighScore(char[]);
void trim(char[]);
void soccerBackground();
void aboutSection();
