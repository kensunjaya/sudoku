#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#include <unistd.h>


#define maxAttemps 10
#define maxMistake 3
#define gotoxy(x,y) printf("\033[%d;%dH", y, x)
#define maxEntries 100

//warna
#define BHCYN "\e[1;96m" //BoldCyn
#define BLU "\e[0;34m" //regBlu
#define reset "\e[0m" //reset

long long int timer; // timer waktu pengerjaan

typedef struct {
	int value;
	int posX;
	int posY;
	bool isFilled;
} Cell;

Cell cells[9][9];
int mistakes = 0;
int posX = 3;
int posY = 4;


int randint(int a, int b);
int check3x3(int baris, int kolom);
void unfill(int difficulty);
void fillCells(int difficulty);
void controller(int difficulty);
void printGrid(int difficulty);
void printMenu(char difficulty[3][20], int index, int size);
int selector(char options[3][20], int size);
void selectDifficulty(char difficulty[3][20]);
int GenerateRandomNum(int max);
void generateUserId(char *username, char *userId) ;
long long int userId();
void mainMenu();


void enterToContinue() {
	char key;
	printf("Press <enter> to continue ...");
	do {
		key = getch();
	} while (key != '\r');
}

void initPos() { // inisialisasi setiap koordinat cell pada terminal
	int i,j, x=posX, y=posY;
	for (i=0;i<9;i++) {
		if ((i % 3 == 0) && (i != 0)) {
			y += 1;
		}
		for (j=0;j<9;j++) {
			if ((j % 3 == 0) && (j != 0)) {
				x += 2;
			}
			cells[i][j].posX = x;
			cells[i][j].posY = y;
			x+=2;
		}
		x = posX;
		y += 1;
	}
}

void emptyGrid() {
	int i,j;
	for (i=0;i<9;i++) {
		for (j=0;j<9;j++) {
			cells[i][j].value = 0;
			cells[i][j].isFilled = true;
		}
	}
}

int randint(int a, int b) { // inclusive (batas atas dan bawah termasuk)
	return ((rand() % (b - a + 1) + a));
}

int checkFilledAll() { // cek apakah user berhasil mengisi semua cells atau belum
	int i, j;
	for (i=0;i<9;i++) {
		for (j=0;j<9;j++) {
			if (!cells[i][j].isFilled) {
				return 0;
			}
		}
	}
	return 1;
}

int check3x3(int baris, int kolom) { //check apakah ada duplikat di tiap matriks 3x3
    int e, f, g, h;
    for (e=baris*3;e<baris*3+3;e++) {
    	for (f=kolom*3;f<kolom*3+3;f++) {
    		for (g=baris*3;g<baris*3+3;g++) {
    			for (h=kolom*3;h<kolom*3+3;h++) {
    				if ((e == g) && (f == h)) {
    					continue;
					}
    				if (cells[e][f].value == cells[g][h].value) {
    					return 1;
					}
				}
			}
		}
	}
	return 0;
}

void clearCells() { // clear cells kecuali diagonalnya
	int i, j;
	for (i=0;i<3;i++) {
		for (j=3;j<9;j++) {
			cells[i][j].value = 0;
		}
	}
	for (i=3;i<6;i++) {
		for (j=0;j<3;j++) {
			cells[i][j].value = 0;
		}
	}
	for (i=3;i<6;i++) {
		for (j=6;j<9;j++) {
			cells[i][j].value = 0;
		}
	}
	for (i=6;i<9;i++) {
		for (j=0;j<6;j++) {
			cells[i][j].value = 0;
		}
	}
}

void fillDiagonal() {
	int i, j;
	do {
		for (i=0;i<3;i++) {
			for (j=0;j<3;j++) {
				cells[i][j].value = randint(1, 9);
			}
		}
	} while (check3x3(0, 0));
	do {
		for (i=3;i<6;i++) {
			for (j=3;j<6;j++) {
				cells[i][j].value = randint(1, 9);
			}
		}
	} while (check3x3(1, 1));
	do {
		for (i=6;i<9;i++) {
			for (j=6;j<9;j++) {
				cells[i][j].value = randint(1, 9);
			}
		}
	} while (check3x3(2, 2));
    
}

void unfill(int difficulty) {
	int i, j;
	for (i=0;i<9;i++) {
		for (j=0;j<9;j++) {
			if (rand() % (difficulty + 1) != 1) {
				cells[i][j].isFilled = false;
			}
		}
	}
}


void fillCells(int difficulty) {
	if (difficulty == 4) {
		mainMenu();
		return;
	}
	int temp, i, j, x, k, attemps = 0;
	bool duplicate, startover = false;
	for (i=0;i<9;i++) {
		for (j=0;j<9;j++) {
			if (cells[i][j].value != 0) {
				continue;
			}
			if (startover) {
				i = 0;
				j = 0;
				startover = false;
			}
			do {
				attemps += 1;
				duplicate = false;
				temp = randint(1, 9);
				for (x=0;x<9;x++) {
					if (cells[i][x].value == temp) {
						duplicate = true;
						break;
					}
				}
				if (!duplicate) {
					for (x=0;x<9;x++) {
						if (cells[x][j].value == temp) {
							duplicate = true;
							break;
						}
					}
				}
				if ((attemps >= maxAttemps) && (duplicate)) {
					for (k=1;k<=9;k++) {
						for (x=0;x<9;x++) {
							if (cells[i][x].value == 0) {
								continue;
							}
							if (cells[i][x].value != k) {
								duplicate = false;
							}
							else {
								duplicate = true;
								break;
							}
						}
						if (!duplicate) {
							for (x=0;x<9;x++) {
								if (cells[x][j].value == 0) {
									continue;
								}
								if (cells[x][j].value != k) {
									duplicate = false;
								}
								else {
									duplicate = true;
									break;
								}
							}
						}
						if (!duplicate) {
							temp = k;
							break;
						}
					}
				}

			} while ((duplicate) && (attemps < maxAttemps));
			if (duplicate) {
				clearCells();
				startover = true;
			}
			else {
				cells[i][j].value = temp;
			}
			attemps = 0;	
		}
	}
	for (i=0;i<3;i++) {
		for (j=0;j<3;j++) {
			if (check3x3(i, j)) {
				system("cls");
				printf("                                                                          \n");
			    printf(",---.                         |    o              ,---.     o    |       \n");
			    printf("|  _.,---.,---.,---.,---.,---.|--- .,---.,---.    |  _.,---..,---|       \n");
			    printf("|   ||---'|   ||---'|    ,---||    ||   ||   |    |   ||    ||   |       \n");
			    printf("`---'`---'`   '`---'`    `---^`---'``   '`---|    `---'`    ``---'    ooo\n");
			    printf("                                         `---'                           \n");

				clearCells();
				fillCells(difficulty);
				return;
			}
		}
	}
	system("cls");
	unfill(difficulty);
	printGrid(difficulty);
	gotoxy(posX, posY);
	timer = time(NULL);
	controller(difficulty);
	
}

void controller(int difficulty) {
	int key, i=0, j=0;
	while (key != 'q') {
		key = getch();
		if (key == 0 || key == 224) { 
            key = getch(); 

            switch (key) {
                case 72: {
                	if (i >= 0) {
						i-=1;
						gotoxy(cells[i][j].posX, cells[i][j].posY);
	
					}
					break;
				}

                
                case 80: {
                	if (i < 9) {
						i+=1;
						gotoxy(cells[i][j].posX, cells[i][j].posY);
						
					}
					break;
				}
                    
				
                case 75: {
                	if (j >= 0) {
						j-=1;
						gotoxy(cells[i][j].posX, cells[i][j].posY);
						
					}
					break;
				}
                case 77: {
                	if (j < 9) {
						j+=1;
						gotoxy(cells[i][j].posX, cells[i][j].posY);
					}
					break;
				}
                default:
                    break;
            }
        } 
		else {
            if (isdigit(key)) {
				if (key - 48 == cells[i][j].value) {
					cells[i][j].isFilled = true;
					if (checkFilledAll()) {
						timer = time(NULL) - timer;
						printGrid(difficulty);
						gotoxy(1, 18);
						printf("You completed the puzzle in %lld seconds\n\n", timer);
						mistakes = 0;
						userId(timer);
						printf("\n");
						enterToContinue();
					
						mainMenu();
						return;
					}
				}
				else {
					if (!cells[i][j].isFilled) {
						mistakes += 1;
						if (mistakes >= maxMistake) {
							system("cls");
							printf("GAME OVER\n\n");
							mistakes = 0;

							enterToContinue();
							mainMenu();
							return;
						}
					}
				}
				printGrid(difficulty);
				gotoxy(cells[i][j].posX, cells[i][j].posY);
			}
        }
	}
}

void printGrid(int difficulty) {
	int i, j, k, l;
	system("cls");
	printf("Difficulty : ");
	switch (difficulty) {
		case 1: {
			puts("Easy");
			break;
		}
		case 2: {
			puts("Normal");
			break;
		}
		case 3: {
			puts("Hard");
			break;
		}
		default: {
			return;
			break;
		}
	}
	printf("Mistakes   : %d\n", mistakes);
	for (l=0;l<25;l++) {
		if (l % 8 == 0) {
			printf("+");
		}
		else {
			printf("-");
		}
	}
	puts("");
	for (k=0;k<9;k++) {
		for (l=0;l<9;l++) {
			if (l == 0) {
				printf("| ");
			}
			if (cells[k][l].isFilled) {
				printf("%d", cells[k][l].value);
			}
			else {
				printf(" ");
			}
			
			if ((l+1)%3==0) {
				printf(" | ");
			}
			else {
				printf(" ");
			}
		}
		
		if ((k+1)%3==0) {
			puts("");
			for (l=0;l<25;l++) {
				if (l % 8 == 0) {
					printf("+");
				}
				else {
					printf("-");
				}
			}
		}
		puts("");
	}

	puts("");
}

void printTitle() {
	system("cls");
	printf("\n"BLU"   /"BHCYN"$$$$$$                  "BLU"/"BHCYN"$$           "BLU"/"BHCYN"$$                "reset);
    printf("\n"BLU"  /"BHCYN"$$"BLU"__  "BHCYN"$$                "BLU"| "BHCYN"$$          "BLU"| "BHCYN"$$                \n"reset);
    printf(BLU" | "BHCYN"$$  "BLU"\\__/ /"BHCYN"$$   "BLU"/"BHCYN"$$  "BLU"/"BHCYN"$$$$$$$  "BLU"/"BHCYN"$$$$$$ "BLU"| "BHCYN"$$   "BLU"/"BHCYN"$$ "BLU"/"BHCYN"$$   "BLU"/"BHCYN"$$\n" reset);
    printf(BLU" |  "BHCYN"$$$$$$ "BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$ "BLU"/"BHCYN"$$"BLU"__  "BHCYN"$$ "BLU"/"BHCYN"$$"BLU"__  "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"/"BHCYN"$$"BLU"/| "BHCYN"$$  "BLU"| "BHCYN"$$\n"reset);
    printf(BLU"  \\____  "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"\\ "BHCYN"$$"BLU"| "BHCYN"$$$$$$"BLU"/ | "BHCYN"$$  "BLU"| "BHCYN"$$\n"reset);
    printf(BLU"  /"BHCYN"$$  "BLU"\\ "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$"BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$"BLU"| "BHCYN"$$"BLU"_  "BHCYN"$$ "BLU"| "BHCYN"$$  "BLU"| "BHCYN"$$\n"reset);
    printf(BLU" |"BHCYN"  $$$$$$"BLU"/|"BHCYN"  $$$$$$"BLU"/|"BHCYN"  $$$$$$$"BLU"|"BHCYN"  $$$$$$"BLU"/|"BHCYN" $$"BLU" \\"BHCYN"  $$"BLU"|"BHCYN"  $$$$$$"BLU"/\n"reset);
    printf(BLU"  \\______/  \\______/  \\_______/ \\______/ |__/  \\__/ \\______/ \n\n\n"reset);
}

void printMenu(char options[4][20], int index, int size) {
	int i;
	int len = 0;
	printTitle();
	
    for (i=0;i<size;i++) {
		if (i==index) {
			gotoxy(5 + (i*12) + len - 2, 15);
			
    		printf(BLU"< ");
    		printf(BHCYN"%s", options[i]);
    		printf(BLU" >");
		}
		else {
			gotoxy(5 + (i*12) + len, 15);
			printf(BHCYN"%s", options[i]);
		}
    	len += strlen(options[i]);
		gotoxy(0, 0);
	}
}

int selector(char options[4][20], int size) {
	int key;
	int index = 0;
	do {
		key = getch();
		if (key == 0 || key == 224) {
            key = getch(); 
            if (key == 77) {
				if (index < size - 1) {
					index++;
				}
				
			}
			else if (key == 75) {
				if (index > 0) {
					index--;
				}
			}
        } 
		printMenu(options, index, size);
	} while (key != '\r');
	return index;
}

void selectDifficulty(char difficulty[3][20]) {
	srand(time(NULL));
	emptyGrid();
	initPos();
	fillDiagonal();
	printMenu(difficulty, 0, 4);
	fillCells(selector(difficulty, 4)+1);
}


int GenerateRandomNum(int max) {
    return rand() % (max + 1);
}
void generateUserId(char *username, char *userId) {
    char character[3];
    character[0] = toupper(username[0]);
    character[1] = toupper(username[1]);
    character[2] = '\0';

    int randomNumbers = GenerateRandomNum(999);

    char randomNumberStr[4];
    sprintf(randomNumberStr, "%03d", randomNumbers);

    strcpy(userId, character);
    strcat(userId, randomNumberStr);
}

long long int userId(long long int duration){
    char username[256];
    char userId[6];

    printf("Enter your username: ");
    scanf("%[^\n]", username); getchar();

    generateUserId(username, userId);

    FILE *file = fopen("highscores.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return 1;
    }

    fprintf(file, "%s#%s#%lld\n", username, userId, duration);

    fclose(file);
}

void highscore(char *username, char *userId, int timer) {
    FILE *fptr = fopen("highscores.txt", "a");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fptr, "%s#%s#%d\n", username, userId, timer);
    fclose(fptr);
}

void displayHighScores() {
	char username[maxEntries][100];
	char userId[maxEntries][100];
	int timer[maxEntries];
	
    FILE *fptr = fopen("highscores.txt", "r");
    if (fptr == NULL) {
        printf("No highscores found!\n");
        return;
    }

    int entries = 0;
    int i, j;

    while (entries < maxEntries && fscanf(fptr, "%[^#]#%[^#]#%d\n", username[entries], userId[entries], &timer[entries]) == 3) {
    	for ( i=0;i<entries;i++) {
			if (strcmp(username[i], username[entries]) == 0) {
				if (timer[i] > timer[entries]) {
					timer[i] = timer[entries];
					entries--;
					break;
				}
				else {
					entries--;
				}
			}
		}
        entries++;
    }
    fclose(fptr);

    for (i = 0; i < entries - 1; i++) {
        for (j = 0; j < entries - i - 1; j++) {
            if (timer[j] > timer[j + 1]) {
                int tempTimer = timer[j];
                timer[j] = timer[j + 1];
                timer[j + 1] = tempTimer;

                char tempUsername[50];
                strcpy(tempUsername, username[j]);
                strcpy(username[j], username[j + 1]);
                strcpy(username[j + 1], tempUsername);

                char tempUserId[20];
                strcpy(tempUserId, userId[j]);
                strcpy(userId[j], userId[j + 1]);
                strcpy(userId[j + 1], tempUserId);
            }
        }
    }
			printf(BHCYN);
			printf("===============================================\n");
			printf("|| User Id  || Username      || Highscore    ||\n");
			printf("===============================================\n");   
	for (i = 0; i < entries; i++) {
		printf("|| %-9s|| %-14s|| %-13d||\n", userId[i], username[i], timer[i]);
		printf("===============================================\n");   

    }
    reset;
}

void howToPlay(){
	puts("========================= HOW TO PLAY =============================");
	puts("|| 1. Each row must contain the numbers 1-9 exactly once each    ||");
	puts("|| 2. Each column must contain the numbers 1-9 exactly once each ||");
	puts("|| 3. Each 3x3 box must contain the numbers 1-9 exactly once each||");
	puts("===================================================================\n\n");
	puts("============================= RULE ================================");
	puts("|| 1. Use Numbers 1-9                                            ||");
	puts("|| 2. Don't Repeat Any Numbers                                   ||");
	puts("|| 3. Don't Guess                                                ||");
	puts("|| 4. Use Process of Elimination                                 ||");
	puts("===================================================================\n\n");

}

void mainMenu() {
	int index = 0;
	char difficulty[4][20] = {"Easy", "Normal", "Hard", "Back"};
	char options[4][20] = {"PLAY", "HOW TO PLAY", "HIGH SCORE", "EXIT"};
	printMenu(options, index, 4);
	
	index = selector(options, 4);
	
	switch(index) {
		case 0: {
			printf(BHCYN);
			selectDifficulty(difficulty);
			break;
		}
		case 1: {
		system("cls");
		howToPlay();
		enterToContinue();
		mainMenu();
			break;
		}
		case 2: {
			system("cls");
			displayHighScores();
			enterToContinue();
			mainMenu();
			break;
		}
		case 3: {
			system("cls");
			printf(BHCYN);
			puts("Creator :");
			puts("- Kenneth Sunjaya");
			puts("- Lavinia Nataniela Novyandi");
			
						
			printf("                            .:^:.. ..^^:..\n");
			sleep(0.001);
			printf("                          ...   ...:.     .......    .\n");
			sleep(0.001);
			printf("                            .   . ^.   :..:~~77~~~::..::.. ..\n");
			sleep(0.001);
			printf("                          ::::..~..:..:~7~.: :^ ~^:~..:....:^~^:^~^\n");
			sleep(0.001);
			printf("                        ..      .. .^!777^~~!?~^~7:^::.  .5BGB##&#7.\n");
			sleep(0.001);
			printf("                             .:~!!. .:~^.77?P57P7!^::~::?5B&&#GGG5.\n");
			sleep(0.001);
			printf("                          .:^~~^^: YJJGBGP5YY7J7!^^^?7YG##P7^7#&B~:\n");
			sleep(0.001);
			printf("                        .^~~:  ~J55PJ~:.       ..~JPBBBY^. .:.JG!:!!^..\n");
			sleep(0.001);
			printf("                     ..^7~  .::???.         .~!JG&&#J:       .7!:.^77?!^..\n");
			sleep(0.001);
			printf("                   .:~7!:. ^Y^.  ..      .!JPB&&#Y:          .  :7!: ~G5?!:.\n");
			sleep(0.001);
			printf("                 .:~7?7..~!!       .:..^?YP&&&G7         .        ~J: .BP7J!:.\n");
			sleep(0.001);
			printf("               .:^777~^::7^          :JB&&#BG^ .                   .J! !YYYJ?^.\n");
			sleep(0.001);
			printf("              .:!JJJJ: :7G.         ?#&&&&B~                         ?5..^!~~!!:.\n");
			sleep(0.001);
			printf("             .:!YYY5~ .J&Y      . .5BBBBBG^                           YP  :?J!~!^.\n");
			sleep(0.001);
			printf("             .:!J?YJ^..P&!        P&&&&&#!             ..              #7 .^55?^~:\n");
			sleep(0.001);
			printf("              .^7!??^..P#? .  .   ~#&&&&&J:..   ...  .   ..  ...     . 5#..:J57~!:.\n");
			sleep(0.001);
			printf("              .:!J~~^.?G#5      .:.^JGBBBP5Y!^75GBGP5PJYPGPP5YYJ?!^    !&:.:??~?~:.\n");
			sleep(0.001);
			printf("               .^?5^:.JPBP!       .:::.. .!YYY&&&###&&&&YG##&&&&#B5:   !&:.^~!5J~.\n");
			sleep(0.001);
			printf("                .^??:.^^?77..                 ^:.  ..P&G:^&&&&&&&&7    PG ~7:JP7:.\n");
			sleep(0.001);
			printf("                .:^7? .~~J?7!                 .      7#^ P&&&&&&G^    :&^ 7?5?!:.\n");
			sleep(0.001);
			printf("                  .^J5.:!JPBBY            .:.       ^5^ 7###&#P^      G7..7G5!:.\n");
			sleep(0.001);
			printf("                   .^?5~^~:~PBP~.?!~: ^^7~^!^     .7: ~#&&#5~        ~~:.:PY~:.\n");
			sleep(0.001);
			printf("                    .^!5J7..~!B&PJY?J^J?YGY?7!:..^^:?#@&G7.        .~.::^Y!^.\n");
			sleep(0.001);
			printf("                     .:^??J!77JY57 .: :?#BYGG!7~:7G&#5~.           .~^!J5!:.\n");
			sleep(0.001);
			printf("                       .:~?JY~..... :.^!J^^!7~7PGPJ~.          ... ^?Y57:.\n");
			sleep(0.001);
			printf("                          :~75P~:.  ^^!!^~!JYJ57:..^^....:^!:  ...5GP7^.\n");
			sleep(0.001);
			printf("                          ::.:~^7.JY^?77Y5Y7^..::~??5J???5J: :?Y!J5!:.\n");
			sleep(0.001);
			printf("                           .  .^.7G&###B5!^:.:.::......~?J?YB#BY7^:.\n");
			sleep(0.001);
			printf("                              .^^YJ5###BG5J7!7JYPPGYJ??YPPPY?7~:.\n");
			sleep(0.001);
			printf("                              ..:^~^^........^~!~!!!!~^^::...\n");
			sleep(0.001);
			printf("\n");
			sleep(0.001);
			printf("                        o---------------------------------------------o\n");
			sleep(0.001);
			printf("                        |      Breaking and Overcoming Challenges     |\n");
			sleep(0.001);
			printf("                        |    Through Courage Hardwork and Persistence |\n");
			sleep(0.001);
			printf("                        o---------------------------------------------o\n");
			sleep(0.001);


			exit(0);
			break;
		}
	}
}


int main() {
	mainMenu();
	return 0;
}