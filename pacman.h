#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>


#define COLOR_1 1
#define COLOR_2 2

#define MOVE 1
#define MOVE_TIME 2
#define SLEEP_TIME 130000
#define DIM_PACMAN 3
#define MAX_BULLETS_PER_TIME_PACMAN 4
#define MAX_BULLETS_PER_TIME_GHOST 4
#define DELAY_BETWEEN_SHOOT_GHOST 4 // 4 secondi tra uno sparo e un altro
#define SLIDE_EFFECT_C 25
#define SLIDE_COLOR_FG 50
#define ROSSO 13

#define YELLOW 26
#define PAIR_1_BULLET 27
#define PAIR_2_BULLET 28
#define PAIR_3_BULLET 29
#define PAIR_4_BULLET 30
#define PAIR_5_BULLET 31

#define MAXX 85
#define MAXY 34
#define N_MOVIMENTI 8
#define SPAZIO 'd'
#define LINEA_VERTICALE 'e'
#define LINEA_ORIZZONTALE 'r'
#define BORDO_UR 'w'
#define BORDO_UL 'q'
#define BORDO_LL 'a'
#define BORDO_LR 's'
#define SCAN_LINE1 't'
#define SCAN_LINE3 'y'
#define SCAN_LINE7 'u'
#define SCAN_LINE9 'i'
#define FOOD 'z'
#define SU 65 			// Freccia su
#define GIU 66 			// Freccia giù
#define SINISTRA 68	// Freccia sinistra
#define DESTRA 67   // Freccia destra
#define SHOOT 'p'   // Tasto s per sparare
#define ENTER 10    // Tasto invio
#define SPACE 32    // Spazio
//DA DEFINIRE

#define SPARO 99
#define MAX_ENTITIES 4
#define BIANCO 11
#define NERO 12
#define VERDE 14
#define BLU 15
// CONTROLLO.h

#define SPARO 99
#define MAX_ENTITIES 4
#define MAX_GHOSTS 4
#define MIN_GHOSTS 4
#define DISTANCE 6
#define TIME_SPAWN 0
#define PAIR_1_GHOST 5
#define PAIR_2_GHOST 6
#define PAIR_3_GHOST 7
#define PAIR_4_GHOST 8
#define PAIR_5_GHOST 9
#define LAST 3
#define NEXT 4
#define PREC_XG 7
#define PREC_YG 6
#define RAND_SHOOT_TIME 2
#define MAX_GHOST_BULLETS_ALIVE 10
#define MAX_PACMAN_BULLETS_ALIVE 10
#define DELAY_SPAWN_PILL 3
#define DELAY_SPAWN_PUP 7
#define MAX_PILLS 2
#define MAX_POWER_UP 4
#define INV_TIME 20
#define TIMER 10000
#define BASEX 40
#define BASEY 12
#define BACK_BASE 2

int vite;
int score;
int armatura;
int realPoints;
int vittoria;
int pos_ghosts[MAX_GHOSTS][3];//last4 next5 prevy6 prevx7
int status_ghosts[MAX_GHOSTS][4];// 0 last 1 next 2 back_base 3 operativo
int num_bullets[2]; //0 pacman 1 ghosts
int pos_pills[MAX_PILLS+1][2];// y,x last 0=npills
int pos_power_up[MAX_PILLS+3][3];// y,x last 0=npills
int ghost_mode[MAX_GHOSTS]; //0 normale //1 freezzati //2 back base // 3 rinascita
// END CONTROLLO.h


typedef struct
{
  int x;
  int y;
} Pos;

typedef struct{

  char head,middle,tail;

} pacStruct;


typedef enum {UP, DOWN, LEFT, RIGHT} DirezioniPossibili;

typedef struct{

	bool available; // Indica se il proiettile è disponibile per la creazione
  int x, y;       // Include coordinate del proiettile
	pthread_t pid;  // Pid del thread

} bulletSet;

typedef struct{

	int idGhost;
	int out;


} reachBaseStruct;

typedef struct{

	int ghost;

} ghostTransferID;

typedef enum {OPEN, CLOSE} pacStatus;
typedef enum {Y, X} generalNamePos;

pthread_mutex_t mutex;
pthread_mutex_t mutexR;
WINDOW *win;
char map[MAXY][MAXX];


int n_entities;
int pacx;
int pacy;
int ghostx;
int ghosty;

//Pos pos[MAX_ENTITIES];

void * pacman(void * Info);
void create_map_window(char map[MAXY][MAXX],WINDOW* win);
void copy_map(char mapin[MAXY][MAXX],char mapout[MAXY][MAXX]);
void * ghost(void * Info);
void * movimenti(void* param);
void normal_path(int id_ghost);
//static void * pacmanBulletCreate(void * arg);
//static void * pacmanBulletSetCreate(void * arg);
void * pacmanShotsBullet(void * arg);
void * ghostShotsBullet(void * arg);
void * fireGhostBullets(void * arg);
void inizializzaBullets(void);

// INIZIO CONTROLLO.H
void * handle_ghosts(void * null);
void * handle_shoot_ghosts(void * null);
void * spawn_pills(void * null);
void * animate_pills(void * null);
void * spawn_power_up(void * null);
void * animate_power_up(void * null);
void * shoot(void * id);
void * handle_refresh(void * null);
void * ghost_to_base(void * null);

bool menu(void);
void * vite_armatura(void * null);
void * new_freeze(void * null );
void print_frozen_ghost(int id);
void reach_base(int id_ghost,int out);
// FINE CONTROLLO.H
