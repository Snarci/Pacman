#include "pacman.h"

bulletSet ghostBullets[MAX_GHOSTS][4]; // UP, DOWN, LEFT, RIGHT bullets
int livello=1; // Scala da 1 a 3
int maxFantasmi=3; // Scala da 3 a

//2980
int main(void) {

  int i,j;
  char c;
	bool startGame;
	pthread_t pacman_ID,ghost_ID,pills_ID,power_up_ID,vite_ID;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutexR = PTHREAD_MUTEX_INITIALIZER; // Mutex per le risorse

	Pos pac[MAX_ENTITIES];
  FILE *fp=NULL;
  vittoria=0;
	vite=3;
	score=0;
	armatura=9;
  realPoints=0;
  initscr();
  start_color();
  refresh();
  srand(time(NULL));
  noecho();
  curs_set(0);


  init_color(COLOR_BLACK, 0,0,0);
  init_color(COLOR_BLUE, 33*(1000/255),33*(1000/255),222*(1000/255)); // Colore blu mappa

  init_color(COLOR_YELLOW,1000,1000,0); // Colore PacMan
  init_color(COLOR_CYAN, 222*(1000/255), 161*(1000/255), 133*(1000/255)); // Colore Inky
  init_color(COLOR_GREEN,0,1000,0); // Colore Pinky

  init_pair(1, COLOR_CYAN, COLOR_BLACK); // colore pallini to FIX
  init_pair(2, COLOR_BLUE,COLOR_BLACK); // Colore mappa
  init_pair(3, COLOR_BLUE, COLOR_YELLOW); // Colore pacman

	// colori menu
	init_pair(ROSSO, COLOR_RED, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
  //colori fantasmi
  init_pair(PAIR_1_GHOST, COLOR_CYAN, COLOR_CYAN);
  init_pair(PAIR_2_GHOST, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(PAIR_3_GHOST, COLOR_GREEN, COLOR_GREEN);
  init_pair(PAIR_4_GHOST, COLOR_RED, COLOR_RED);
  init_pair(PAIR_5_GHOST, COLOR_WHITE, COLOR_WHITE);
	init_pair(PAIR_1_BULLET, COLOR_BLACK, COLOR_CYAN);
	init_pair(PAIR_2_BULLET, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(PAIR_3_BULLET, COLOR_BLACK, COLOR_GREEN);
	init_pair(PAIR_4_BULLET, COLOR_BLACK, COLOR_RED);
	init_pair(PAIR_5_BULLET, COLOR_BLACK, COLOR_WHITE);

  init_pair(BIANCO, COLOR_WHITE, COLOR_BLACK);
  init_pair(NERO, COLOR_BLACK, COLOR_BLACK);
  init_pair(VERDE, COLOR_GREEN, COLOR_BLACK);
  init_pair(BLU, COLOR_WHITE, COLOR_BLUE);
// inizializzo dimensioni dello schermo
  win = newwin(MAXY,MAXX,0,0);

  curs_set(0);
  //keypad(stdscr, TRUE);
  wattron(win,COLOR_PAIR(1)); // Pallini
  wattron(win,COLOR_PAIR(2)); // Box mappa
//iniziallizzo la matrice contenente la mappa
  fp=fopen("convert.txt", "r");

  if(fp==NULL)
    return -1;

  for(i=0;i<MAXY;i++){
    for(j=0;j<MAXX;j++){

    	c=' ';
    	fscanf(fp,"%c",&c);
      if(c!=EOF)
        map[i][j]=c;
      if(c==' ')
        map[i][j]=SPAZIO;
    }
  }
  fclose(fp);

	startGame = menu();

	if(startGame){
	  create_map_window(map,win);

		inizializzaBullets(); // Inizializza i proietili dei fantasmi
	  pthread_create(&pacman_ID,NULL,&pacman,(void* )pac);
	  pthread_create(&ghost_ID,NULL,&handle_ghosts,(void* )pac);
	 	pthread_create(&pills_ID,NULL,&spawn_pills,(void* )pac);
		pthread_create(&power_up_ID,NULL,&spawn_power_up,(void* )pac);
		pthread_create(&vite_ID,NULL,&vite_armatura,(void* )pac);
	  while(vite > 0 && realPoints != 2980);

		/*
		if(vite <= 0){
			vittoria=1;
			sleep(0.5);

			wclear(win);
			wattron(win, COLOR_PAIR(BIANCO));
			wattron(win, A_BLINK);
			create_map_window(map,win);
			wrefresh(win);
			wattroff(win, A_BLINK);
			wattroff(win, COLOR_PAIR(BIANCO));

		}*/

		while(vite > 0 && realPoints  != 2980);

    if(vite > 0 ){
      vittoria=1;
      sleep(0.5);
			pthread_mutex_lock(&mutex);
      wclear(win);
      wattron(win, COLOR_PAIR(BIANCO));
      wattron(win, A_BLINK);
      create_map_window(map,win);
      mvprintw(MAXY, MAXX/2, "                                                         ");
      wattron(win, A_BOLD | A_STANDOUT | COLOR_PAIR(COLOR_YELLOW));
      mvwprintw(win,MAXY/2-2, MAXX/2-4, "VITTORIA");
      wattroff(win, A_BOLD | A_STANDOUT | COLOR_PAIR(COLOR_YELLOW));
			wattroff(win, A_BLINK);
			mvwprintw(win, MAXY, 0, "                                      ");

			mvwprintw(win,MAXY/2-1, MAXX/2-14, "Premere Invio per Concludere");


      wrefresh(win);

      wattroff(win, COLOR_PAIR(BIANCO));
			pthread_mutex_unlock(&mutex);



    }else{
      vittoria=1;
      sleep(0.5);
			pthread_mutex_lock(&mutex);

      wclear(win);
      wattron(win, COLOR_PAIR(BIANCO));
      wattron(win, A_BLINK);
      create_map_window(map,win);
      mvprintw(MAXY, MAXX/2, "                                                         ");
      wattron(win, A_BOLD | A_STANDOUT | COLOR_PAIR(ROSSO));
      mvwprintw(win,MAXY/2-2, MAXX/2-4, "SCONFITTA");
			wattroff(win, A_BOLD | A_STANDOUT | COLOR_PAIR(ROSSO));
			wattroff(win, A_BLINK);
			mvwprintw(win, MAXY, 0, "                                      ");
			mvwprintw(win,MAXY/2-1, MAXX/2-14, "Premere Invio per Concludere.");

      wrefresh(win);

      wattroff(win, COLOR_PAIR(BIANCO));

			pthread_mutex_unlock(&mutex);

    }
		pthread_mutex_lock(&mutex);
		c = getch();
		pthread_mutex_unlock(&mutex);

		while(c != ENTER){
			pthread_mutex_lock(&mutex);
			c = getch();
			pthread_mutex_unlock(&mutex);

		}

	}

  wrefresh(win);
  getch();
  delwin(win);
  endwin();

}


bool menu(void){

	char c;
	int i,j,k;
	int gradient;
	int scelta=0, scelta1=0;
	char denominazioneLivello[3][7] = {"Easy", "Normal", "Hard"};

	while(scelta != -1){

		scelta = 0;
		pthread_mutex_lock(&mutex);
		wclear(win);
		wattron(win, COLOR_PAIR(YELLOW));
		wattron(win, A_BOLD);
		mvwprintw(win, MAXY/2,MAXX/2, "PACMAN");
		wattroff(win, A_BOLD);
		wattron(win, COLOR_PAIR(ROSSO));
		mvwprintw(win, MAXY/2+1, MAXX/2, "> Start Game"); // scelta 0
		wattron(win, COLOR_PAIR(BIANCO));
		mvwprintw(win, MAXY/2+2, MAXX/2, "  Settings"); // scelta 1
		mvwprintw(win, MAXY/2+3, MAXX/2, "  Help"); // scelta 2
		mvwprintw(win, MAXY/2+5, MAXX/2, "Press Enter to Continue...");

		wrefresh(win);
		c = getch();
		pthread_mutex_unlock(&mutex);

		while(c != ENTER){

			if(c == ENTER)
				break;
			if(c == SU){
				if(scelta == 0)
					scelta = 2;
				else
					if(scelta == 1)
						scelta = 0;
					else
						if(scelta == 2)
							scelta = 1;
			}

			if(c == GIU){
				if(scelta == 0)
					scelta = 1;
					else
						if(scelta == 1)
							scelta = 2;
						else
							if(scelta == 2)
								scelta = 0;
			}


			if(scelta == 0){

				pthread_mutex_lock(&mutex);
				wattron(win, COLOR_PAIR(BIANCO));
				wattron(win, COLOR_PAIR(ROSSO));
				mvwprintw(win, MAXY/2+1, MAXX/2, "> Start Game");
				wattron(win, COLOR_PAIR(BIANCO));
				mvwprintw(win, MAXY/2+2, MAXX/2, "  Settings");
				mvwprintw(win, MAXY/2+3, MAXX/2, "  Help");
				pthread_mutex_unlock(&mutex);
			}
			if(scelta == 1){
				pthread_mutex_lock(&mutex);
				wattron(win, COLOR_PAIR(BIANCO));
				mvwprintw(win, MAXY/2+1, MAXX/2, "  Start Game");
				wattron(win, COLOR_PAIR(ROSSO));
				mvwprintw(win, MAXY/2+2, MAXX/2, "> Settings");
				wattron(win, COLOR_PAIR(BIANCO));
				mvwprintw(win, MAXY/2+3, MAXX/2, "  Help");
				pthread_mutex_unlock(&mutex);
			}
			if(scelta == 2){

				pthread_mutex_lock(&mutex);
				wattron(win, COLOR_PAIR(BIANCO));
				mvwprintw(win, MAXY/2+1, MAXX/2, "  Start Game");
				mvwprintw(win, MAXY/2+2, MAXX/2, "  Settings");
				wattron(win, COLOR_PAIR(ROSSO));
				mvwprintw(win, MAXY/2+3, MAXX/2, "> Help");
				pthread_mutex_unlock(&mutex);

			}
			pthread_mutex_lock(&mutex);
			wrefresh(win);
			c=getch();
			pthread_mutex_unlock(&mutex);

		}


		if(scelta == 0){

			// TRANSITION
			gradient = 1000	/MAXX;
			for(i=0; i<MAXX; i++){
				init_color(COLOR_CYAN, i*2%gradient,2*i%gradient,2*i%gradient);
				init_pair(SLIDE_EFFECT_C, COLOR_CYAN, COLOR_BLACK);
				for(j=i, k=0; j>=0 && k<=i; j--,k++){
					wattron(win, COLOR_PAIR(SLIDE_EFFECT_C));
					mvwaddch(win, j,k, '/');
					wattroff(win, COLOR_PAIR(SLIDE_EFFECT_C));
				}
				wrefresh(win);
				usleep(15000);
			}

			for(i=0; i<MAXX; i++){
				init_color(COLOR_CYAN,i*2%gradient,i*gradient, i*gradient);
				init_pair(SLIDE_EFFECT_C, COLOR_CYAN, COLOR_BLACK);
				for(j=MAXX, k=i; j>=i && k<=MAXX; j--,k++){
					wattron(win, COLOR_PAIR(SLIDE_EFFECT_C));
					mvwaddch(win, j,k, '/');
					wattroff(win, COLOR_PAIR(SLIDE_EFFECT_C));
					wattroff(win, A_BLINK);
				}
				wrefresh(win);
				usleep(15000);
			}

			wclear(win);
			init_color(COLOR_CYAN, 222*(1000/255), 161*(1000/255), 133*(1000/255));
			return true;
		}

	/*if(scelta == 1){
		wclear(win);
		wattron(win, COLOR_PAIR(YELLOW));
		wattron(win, A_BOLD);
		mvwprintw(win, MAXY/2, MAXX/2, "IMPOSTAZIONI");
		wattroff(win, A_BOLD);
		wattron(win, COLOR_PAIR(ROSSO));
		mvwprintw(win, MAXY/2+1, MAXX/2, "> Vite: <%d>", livello);
		wattron(win, COLOR_PAIR(BIANCO));
		mvwprintw(win, MAXY/2+2, MAXX/2, "  Max Fantasmi: <%d>", maxFantasmi);
		mvwprintw(win, MAXY/2+3, MAXX/2, "  Indietro");
		mvwprintw(win, MAXY/2+5, MAXX/2, "Press Enter to Continue...");

		pthread_mutex_lock(&mutex);
		wrefresh(win);
		c = getch();
		pthread_mutex_unlock(&mutex);


		while(c != ENTER && scelta1 != 2){

		if(c == ENTER)
			break;
		if(c == SU){
			if(scelta1 == 0)
				scelta1 = 2;
			else
			if(scelta1 == 1)
				scelta1 = 0;
			else
			if(scelta1 == 2)
				scelta1 = 1;
		}
		if(c == GIU){
			if(scelta1 == 0)
				scelta1 = 1;
			else
				if(scelta1 == 1)
					scelta1 = 2;
			else
				if(scelta1 == 2)
					scelta1 = 0;
		}
		if(scelta1 == 0){
			wattron(win, COLOR_PAIR(ROSSO));
			mvwprintw(win, MAXY/2+1, MAXX/2, "> Livello: ");
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+2, MAXX/2, "  Max Fantasmi: ");
			mvwprintw(win, MAXY/2+3, MAXX/2, "  Indietro");
		}
		if(scelta1 == 1){
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+1, MAXX/2, "  Livello: ");
			wattron(win, COLOR_PAIR(ROSSO));
			mvwprintw(win, MAXY/2+2, MAXX/2, "> Max Fantasmi: ");
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+3, MAXX/2, "  Indietro");
		}
		if(scelta1 == 2){
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+1, MAXX/2, "  Livello: ");
			mvwprintw(win, MAXY/2+2, MAXX/2, "  Max Fantasmi: ");
			wattron(win, COLOR_PAIR(ROSSO));
			mvwprintw(win, MAXY/2+3, MAXX/2, "> Indietro");
		}

		pthread_mutex_lock(&mutex);
		wrefresh(win);
		c = getch();
		pthread_mutex_unlock(&mutex);
		}


		if(scelta1 == 2){
			wclear(win);
		}


	}*/

		if(scelta == 1){

			wclear(win);
			wattron(win, A_BOLD);
			wattron(win, COLOR_PAIR(YELLOW));
			mvwprintw(win, MAXY/2, MAXX/2, "IMPOSTAZIONI");
			wattroff(win, A_BOLD);
			wattroff(win, COLOR_PAIR(YELLOW));
			wattron(win, COLOR_PAIR(ROSSO));
			mvwprintw(win, MAXY/2+1, MAXX/2, "> Vite: <%d>", vite);
			wattroff(win, COLOR_PAIR(ROSSO));
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+2, MAXX/2, "Armature: %d", armatura);
			mvwprintw(win, MAXY/2+3, MAXX/2, "Indietro");
			mvwprintw(win, MAXY/2+5, MAXX/2, "Press Enter to Continue...");
			wattroff(win, COLOR_PAIR(BIANCO));

			pthread_mutex_lock(&mutex);
			wrefresh(win);
			c = getch();
			pthread_mutex_unlock(&mutex);
			scelta1=0;
			while(scelta1 != 2){




				if(c == SU){
					if(scelta1 == 0)
						scelta1 = 2;
					else
						if(scelta1 == 1)
							scelta1 = 0;
						else
							if(scelta1 == 2)
								scelta1 = 1;

				}
				if(c == GIU){
					if(scelta1 == 0)
						scelta1 = 1;
						else
							if(scelta1 == 1)
								scelta1 = 2;
							else
								if(scelta1 == 2)
									scelta1 = 0;
				}
				if(c == SINISTRA){

					switch(scelta1){

						case 0:
							if(vite > 1)
								vite--;
							break;
						case 1:
							if(armatura > 1)
								armatura--;
							break;

					}

				}
				if(c == DESTRA){

					switch(scelta1){

						case 0:
							if(vite < 10)
								vite++;
							break;
						case 1:
							if(armatura < 10)
								armatura++;
							break;

					}

				}

				wclear(win);
				if(scelta1 == 0){
					wattron(win, A_BOLD);
					wattron(win, COLOR_PAIR(YELLOW));
					mvwprintw(win, MAXY/2, MAXX/2, "IMPOSTAZIONI");
					wattroff(win, A_BOLD);
					wattroff(win, COLOR_PAIR(YELLOW));
					wattron(win, COLOR_PAIR(ROSSO));
					mvwprintw(win, MAXY/2+1, MAXX/2, "> Vite: <%d>", vite);
					wattroff(win, COLOR_PAIR(ROSSO));
					wattron(win, COLOR_PAIR(BIANCO));
					mvwprintw(win, MAXY/2+2, MAXX/2, "Armature: %d", armatura);
					mvwprintw(win, MAXY/2+3, MAXX/2, "Indietro");
					mvwprintw(win, MAXY/2+5, MAXX/2, "Press Enter to Continue...");
				}
				if(scelta1 == 1){

					wattron(win, A_BOLD);
					wattron(win, COLOR_PAIR(YELLOW));
					mvwprintw(win, MAXY/2, MAXX/2, "IMPOSTAZIONI");
					wattroff(win, A_BOLD);
					wattroff(win, COLOR_PAIR(YELLOW));
					wattron(win, COLOR_PAIR(BIANCO));
					mvwprintw(win, MAXY/2+1, MAXX/2, "Vite: %d", vite);
					wattroff(win, COLOR_PAIR(BIANCO));
					wattron(win, COLOR_PAIR(ROSSO));
					mvwprintw(win, MAXY/2+2, MAXX/2, "> Armature: <%d>", armatura);
					wattroff(win, COLOR_PAIR(ROSSO));
					wattron(win, COLOR_PAIR(BIANCO));
					mvwprintw(win, MAXY/2+3, MAXX/2, "Indietro");
					mvwprintw(win, MAXY/2+5, MAXX/2, "Press Enter to Continue...");
					wattroff(win, COLOR_PAIR(BIANCO));

				}
				if(scelta1 == 2){

					wattron(win, A_BOLD);
					wattron(win, COLOR_PAIR(YELLOW));
					mvwprintw(win, MAXY/2, MAXX/2, "IMPOSTAZIONI");
					wattroff(win, A_BOLD);
					wattroff(win, COLOR_PAIR(YELLOW));
					wattron(win, COLOR_PAIR(BIANCO));
					mvwprintw(win, MAXY/2+1, MAXX/2, "Vite: %d", vite);
					mvwprintw(win, MAXY/2+2, MAXX/2, "Armature: %d", armatura);
					wattroff(win, COLOR_PAIR(BIANCO));
					wattron(win, COLOR_PAIR(ROSSO));
					mvwprintw(win, MAXY/2+3, MAXX/2, "> Indietro");
					wattroff(win, COLOR_PAIR(ROSSO));
					wattron(win, COLOR_PAIR(BIANCO));
					mvwprintw(win, MAXY/2+5, MAXX/2, "Press Enter to Continue...");
					wattroff(win, COLOR_PAIR(BIANCO));
				}

				pthread_mutex_lock(&mutex);
				wrefresh(win);
				c = getch();
				pthread_mutex_unlock(&mutex);





			}




		}


		if(scelta == 2){

			wclear(win);
			wattron(win, COLOR_PAIR(YELLOW));
			wattron(win, A_BOLD);
			mvwprintw(win, MAXY/2, MAXX/2, "COMANDI");
			wattroff(win, A_BOLD);
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+1, MAXX/2, "UpArrow/w    - Su");
			mvwprintw(win, MAXY/2+2, MAXX/2, "DownArrow/a  - Giù");
			mvwprintw(win, MAXY/2+3, MAXX/2, "LeftArrow/s  - Sinistra");
			mvwprintw(win, MAXY/2+4, MAXX/2, "RightArrow/d - Destra");
			mvwprintw(win, MAXY/2+5, MAXX/2, "Space/p      - Sparo");
			wattroff(win, COLOR_PAIR(BIANCO));
			wattron(win, COLOR_PAIR(YELLOW));
			mvwprintw(win, MAXY/2+7, MAXX/2, "CREDITI");
			wattroff(win, COLOR_PAIR(YELLOW));
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+8, MAXX/2, "Gioco sviluppato da:");
			mvwprintw(win, MAXY/2+9, MAXX/2, "Luca Melis");
			mvwprintw(win, MAXY/2+10, MAXX/2, "Luca Zedda");
			wattroff(win, COLOR_PAIR(BIANCO));
			wattron(win, COLOR_PAIR(ROSSO));
			mvwprintw(win, MAXY/2+11, MAXX/2, "> Indietro");
			wattron(win, COLOR_PAIR(BIANCO));
			mvwprintw(win, MAXY/2+13, MAXX/2, "Press Enter to Continue...");
			pthread_mutex_lock(&mutex);
			wrefresh(win);
			c = getch();
			pthread_mutex_unlock(&mutex);
			while(c != ENTER){
				pthread_mutex_lock(&mutex);
				c = getch();
				pthread_mutex_unlock(&mutex);
			}

		} // Menu Help
	}
	return false;
}

void * ghost(void * id){

  int* pid_ghost = (int*) id;
  int id_ghost=(*pid_ghost);
  ghost_mode[id_ghost]=0;
  while(vittoria==0){

    if(pos_ghosts[id_ghost][2]==1 && (pos_ghosts[id_ghost][0]==pacy && pos_ghosts[id_ghost][1]==pacx) && ghost_mode[id_ghost]==1) {
			pthread_mutex_lock(&mutexR);
      ghost_mode[id_ghost]=2;
			pthread_mutex_unlock(&mutexR);
			pthread_mutex_lock(&mutex);
      wrefresh(win);
			pthread_mutex_unlock(&mutex);
    }

    switch (ghost_mode[id_ghost]) {
      case 0:
        normal_path(id_ghost);
        break;
      case 1:
        print_frozen_ghost(id_ghost);
        break;
      case 2:
        pthread_mutex_lock(&mutex);
        wrefresh(win);
        pthread_mutex_unlock(&mutex);
				reach_base(id_ghost,2);
        break;
    }
  }
}

void normal_path(int id_ghost){
  //Pos * pos = (Pos*) param;
  int corex,corey;
  //fine ciclo per la generazione casuale di un movimento disponibile
  int flag=0;
  //posizione globale del fantasma poi da sostituire con matrice

  // flag di possibilitò movimento ovvero se posso muovermi in una direzione
  int movsu=0,movgiu=0,movdes=0,movsin=0;
  //inizializzo pacman
  int ghostx=pos_ghosts[id_ghost][1];
  int ghosty=pos_ghosts[id_ghost][0];
  int j;
  //inizializzazione colori fantasmi
  int color;
  switch (id_ghost) {
    case 0:
    color=PAIR_1_GHOST;
    break;
    case 1:
    color=PAIR_2_GHOST;
    break;
    case 2:
    color=PAIR_3_GHOST;
    break;
    case 3:
    color=PAIR_4_GHOST;
    break;
    case 4:
    color=PAIR_5_GHOST;
    break;
  }

	pthread_mutex_lock(&mutex);
  wattron(win,COLOR_PAIR(color));
  mvwaddch(win,ghosty,ghostx-1, '(');
  mvwaddch(win,ghosty,ghostx, 'o');
  mvwaddch(win,ghosty,ghostx+1, '<');
	pthread_mutex_unlock(&mutex);


    // aggoiono le posizioni ausiliarie con quelle globali

    corex=pos_ghosts[id_ghost][1];
    corey=pos_ghosts[id_ghost][0];
    // azzero gli spostamenti disponibili
    movsu=0;
    movgiu=0;
    movsin=0;
    movdes=0;
//cercod i capire se posso muovermi in una direzione e imposto flag a 1 così poui dopo posso ciclare per
// scegliere in quale direzione andarre
    if((map[pos_ghosts[id_ghost][0]-1][pos_ghosts[id_ghost][1]+1]==SPAZIO && map[pos_ghosts[id_ghost][0]-1 ][pos_ghosts[id_ghost][1]-1]==SPAZIO ) && status_ghosts[id_ghost][0]!=GIU){
      movsu=1;
      flag=1;
    }
    if((map[pos_ghosts[id_ghost][0]+1][pos_ghosts[id_ghost][1]+1]==SPAZIO && map[pos_ghosts[id_ghost][0]+1][pos_ghosts[id_ghost][1]-1]==SPAZIO) && status_ghosts[id_ghost][0]!=SU){
      movgiu=1;
      flag=1;
    }
    if((map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]+3+1]==SPAZIO && map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]+3-1]==SPAZIO) && status_ghosts[id_ghost][0]!=SINISTRA && (pos_ghosts[id_ghost][0]!=16 || pos_ghosts[id_ghost][1]!=64) ){
      movdes=1;
      flag=1;
    }
    if((map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]-3+1]==SPAZIO && map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]-3-1]==SPAZIO) && status_ghosts[id_ghost][0]!=DESTRA && (pos_ghosts[id_ghost][0]!=16 || pos_ghosts[id_ghost][1]!=19) ){
      movsin=1;
      flag=1;
    }


    //geenreo casualmente una direzione e se è disponibile ci vado

    while(flag){

      // numero compreso tra gli spostamenti
      status_ghosts[id_ghost][1]=65+rand()%4;
      //se lo sposstamnto è fattibile esce dal ciclo impostando la condizione a falsa
      switch(status_ghosts[id_ghost][1]){
        case SINISTRA:
        if(movsin==1){
          flag=0;
        }
        break;
        case DESTRA:
        if(movdes==1){
          flag=0;
        }
        break;
        case SU:
        if(movsu==1){
          flag=0;
        }
        break;
        case GIU:
        if(movgiu==1){
          flag=0;

        }
        break;
      }
    }


flag=1;


    // mi muovo in quella direzione aggiornando la posizione locale

      switch(status_ghosts[id_ghost][1]){
        case SINISTRA:
        corex-=3;
        break;
        case DESTRA:
        corex+=3;
        break;
        case SU:
        corey--;
        break;
        case GIU:
        corey++;
        break;
        }
        //rimbalzo
        for(j=0;j<MAX_GHOSTS;j++){
          if(id_ghost!=j){
            if(pos_ghosts[id_ghost][0]==pos_ghosts[j][0] && pos_ghosts[id_ghost][1]==pos_ghosts[j][1] && ghost_mode[j] == 0){
              switch(status_ghosts[id_ghost][1]){
                case SINISTRA:
                status_ghosts[id_ghost][1]=DESTRA;
                break;
                case DESTRA:
                status_ghosts[id_ghost][1]=SINISTRA;
                break;
                case SU:
                status_ghosts[id_ghost][1]=GIU;
                break;
                case GIU:
                status_ghosts[id_ghost][1]=SU;
                break;
              }
              switch(status_ghosts[id_ghost][1]){
                case SINISTRA:
                corex-=3;
                break;
                case DESTRA:
                corex+=3;
                break;
                case SU:
                corey--;
                break;
                case GIU:
                corey++;
                break;
                }

                switch(status_ghosts[j][0]){
                  case SINISTRA:
                  status_ghosts[j][0]=DESTRA;
                  break;
                  case DESTRA:
                  status_ghosts[j][0]=SINISTRA;
                  break;
                  case SU:
                  status_ghosts[j][0]=GIU;
                  break;
                  case GIU:
                  status_ghosts[j][0]=SU;
                  break;
                }

                switch(status_ghosts[j][0]){
                  case SINISTRA:
                  pos_ghosts[j][1]-=3;
                  break;
                  case DESTRA:
                  pos_ghosts[j][1]+=3;
                  break;
                  case SU:
                  pos_ghosts[j][0]--;
                  break;
                  case GIU:
                  pos_ghosts[j][0]++;
                  break;
                  }


            }
          }
        }
                  //scrivo nella nuova posizione il corpo del fantasma
                  if(corey!=pacy || corex!=pacx){
									pthread_mutex_lock(&mutex);
                  wattron(win,COLOR_PAIR(color));
                  mvwaddch(win,corey,corex-1, '(');
                  mvwaddch(win,corey,corex, 'o');
                  mvwaddch(win,corey,corex+1, '<');
									pthread_mutex_unlock(&mutex);
                }
                  if(pos_ghosts[id_ghost][1] != corex || pos_ghosts[id_ghost][0] != corey){
                    // se mi sono effettivamente spostato allora cancello la vecchia posizione salvata nella posizione globalmente
                    // nota bene che non è stata ancora modificata
                    if(pacx == pos_ghosts[id_ghost][1] && pacy == pos_ghosts[id_ghost][0] );


                    else{

										pthread_mutex_lock(&mutex);
                    wattron(win,COLOR_PAIR(2));
                    mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1]-1, ' ');
                    mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1]+1, ' ');
                    //se al centro c'era un pallino lo rimetto altrimenti metto spazio
                    if(map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]]==FOOD){
                      wattroff(win, COLOR_PAIR(2));
                      wattron(win,COLOR_PAIR(1));
                      mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1], ACS_BULLET);
                      wattroff(win, COLOR_PAIR(1));
                      wattron(win,COLOR_PAIR(2));
                    }else{
                      mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1], ' ');
                    }
										pthread_mutex_unlock(&mutex);
                    //salvo la posizione dell'ultima mossa

                  }

                }
                  //aggiono finalmente gli indici globali

                  status_ghosts[id_ghost][0]=status_ghosts[id_ghost][1];

									pthread_mutex_lock(&mutex);
                  wrefresh(win);
									pthread_mutex_unlock(&mutex);

									pthread_mutex_lock(&mutexR);
                  pos_ghosts[id_ghost][1]=corex;
                  pos_ghosts[id_ghost][0]=corey;
									pthread_mutex_unlock(&mutexR);

        usleep(150000);


}

void create_map_window(char map[MAXY][MAXX], WINDOW  *win){

  int i,j;
  for(i=0;i<MAXY;i++){
    for(j=0;j<MAXX;j++){
			pthread_mutex_lock(&mutex);
      wattron(win,COLOR_PAIR(2));

      switch (map[i][j]) {
        case SPAZIO:
          mvwaddch(win,i, j, ' ');
          break;
        case LINEA_VERTICALE:
          mvwaddch(win,i, j,ACS_VLINE);
          break;
        case LINEA_ORIZZONTALE:
          mvwaddch(win,i, j,ACS_HLINE);
          break;
        case BORDO_LL:
          mvwaddch(win,i, j,ACS_LLCORNER);
          break;
        case BORDO_LR:
          mvwaddch(win,i, j,ACS_LRCORNER);
          break;
        case BORDO_UL:
          mvwaddch(win,i, j,ACS_ULCORNER);
          break;
        case BORDO_UR:
          mvwaddch(win,i, j,ACS_URCORNER);
          break;
        case SCAN_LINE1:
          mvwaddch(win,i, j,ACS_S1);
          break;
        case SCAN_LINE3:
          mvwaddch(win,i, j,ACS_S3);
          break;
        case SCAN_LINE7:
          mvwaddch(win,i, j,ACS_S7);
          break;
        case SCAN_LINE9:
          //mvwaddch(win,i, j,ACS_S9);
          break;
        case FOOD:
          wattroff(win, COLOR_PAIR(2));
          wattron(win,COLOR_PAIR(1));
          mvwaddch(win,i,j, ACS_BULLET);
          wattroff(win, COLOR_PAIR(1));
          wattron(win,COLOR_PAIR(2));
          break;

        }
				pthread_mutex_unlock(&mutex);

      }

    }
}

void * handle_ghosts(void * null){

  int rand_posx,rand_posy;
  int n_ghosts=0;
  int i,j;
  char check_map[MAXY][MAXX];
  char c;

  pthread_t ids[MAX_GHOSTS];
	pthread_t idBulletCall[MAX_GHOSTS];
	ghostTransferID *ghostID = (ghostTransferID*) malloc(sizeof(ghostTransferID));

  for(i=0;i!=MAX_GHOSTS;i++){
    pos_ghosts[i][2]=0;
    status_ghosts[i][3]=1;
    status_ghosts[i][2]=1;
  }

  //creo mappa controllo iniziale di spawn
  FILE* fp=NULL;
    FILE* log;

  fp=fopen("convert.txt", "r");
  if(fp==NULL)
    return 0;
log=fopen("log.txt", "a");
    for(i=0;i<MAXY;i++){
      for(j=0;j<MAXX;j++){

        c=' ';
        fscanf(fp,"%c",&c);

        if(c!=EOF)
          check_map[i][j]=c;

        if(c==' ')
          check_map[i][j]=SPAZIO;


        }
      }
      fclose(fp);
      fclose(log);
      int newi,newj;
      while(vittoria==0){
      if(n_ghosts<MAX_GHOSTS){
      for(i=0;i<MAXY;i++){
        for(j=0;j<MAXX;j++){
          if(n_ghosts<MAX_GHOSTS){
          newi=i;
          newj=j;
          i=1+rand()%MAXY-2;
          j=1+rand()%MAXX-2;
          if(map[i][j]==SPAZIO && check_map[i][j]==FOOD && pos_ghosts[n_ghosts][2]==0){

            rand_posy=i;
            rand_posx=j;
            i=MAXY;
            j=MAXX;


            if(n_ghosts>=MIN_GHOSTS)
             sleep(TIME_SPAWN); //todo





            pos_ghosts[n_ghosts][2]=1;

            pos_ghosts[n_ghosts][0]=rand_posy;
            pos_ghosts[n_ghosts][1]=rand_posx;

            pos_ghosts[n_ghosts][2]=1;
            pthread_create(&ids[n_ghosts],NULL,&ghost,(void*)&n_ghosts);

						ghostID->ghost = n_ghosts;
						pthread_create(&idBulletCall[n_ghosts], NULL, &fireGhostBullets, (void*) ghostID);


            //altrimenti la creazione del thread essendo piu' lenta pesca un valore nullo
            sleep(3);
            n_ghosts++;
          }
        }else{
          newi=MAXY;
          newj=MAXX;
        }
        i=newi;
        j=newj;
      }

    }

    }
    }

}

// THREAD: questa funzione si autoassegna un proiettile di PacMan da gestire in base alla lista di quelli disponibili in pacmanBullets
void inizializzaBullets(void){

	int ghost, bullet;

	for(ghost=0; ghost<MAX_GHOSTS; ghost++){
		for(bullet=0; bullet<MAX_BULLETS_PER_TIME_GHOST; bullet++){
		ghostBullets[ghost][bullet].available = true;
		ghostBullets[ghost][bullet].x = -1;
		ghostBullets[ghost][bullet].y = -1;
		ghostBullets[ghost][bullet].pid = -1;
		}
	}
}
// Thread che controlla gli spari di un singolo fantasma
void * fireGhostBullets(void * ghostID){

	ghostTransferID *messageIn = (ghostTransferID*) malloc(sizeof(ghostTransferID));
	int ghost = ((ghostTransferID*) ghostID)->ghost; // id del fantasma


	pthread_t bullet_1, bullet_2, bullet_3, bullet_4;
	sleep(3); // Aspetta 3 secondi prima di iniziare a sparare, aspetta che il fantasma venga spawnato
	while(vittoria==0){

		if(pos_ghosts[ghost][2] && ghost_mode[ghost] == 0){

			messageIn->ghost=ghost;
			pthread_create(&bullet_1, NULL, &ghostShotsBullet, (void*) messageIn);
			pthread_create(&bullet_2, NULL, &ghostShotsBullet, (void*) messageIn);
			pthread_create(&bullet_3, NULL, &ghostShotsBullet, (void*) messageIn);
			pthread_create(&bullet_4, NULL, &ghostShotsBullet, (void*) messageIn);

		}
		sleep(DELAY_BETWEEN_SHOOT_GHOST+rand()%4);
	}
}
// Thread che controlla un singolo proiettile
void * ghostShotsBullet(void * ghostID){

	int k;
	int posX, posY;
	int oldX, oldY;
	int ghost;
	pthread_t self;
	int centro = map[posY][posX];
	int dx = map[posY][posX+1];
	int sx = map[posY][posX-1];
	int colpito=0;
	self = pthread_self();
	ghost = ((ghostTransferID*) ghostID)->ghost; // id del fantasma
	// Ciclo di assegnamento direzione proiettile.
	for(k=0; k<MAX_BULLETS_PER_TIME_GHOST; k++){

		if(ghostBullets[ghost][k].available){
			if( (map[pos_ghosts[ghost][Y]-1][pos_ghosts[ghost][X]] == SPAZIO || map[pos_ghosts[ghost][Y]-1][pos_ghosts[ghost][X]] == FOOD) && k == UP)
				break;
			if( (map[pos_ghosts[ghost][Y]+1][pos_ghosts[ghost][X]] == SPAZIO || map[pos_ghosts[ghost][Y]+1][pos_ghosts[ghost][X]] == FOOD) && k == DOWN)
				break;

			if( (map[pos_ghosts[ghost][Y]][pos_ghosts[ghost][X]-2] == SPAZIO || map[pos_ghosts[ghost][Y]][pos_ghosts[ghost][X]-2] == FOOD) && k == LEFT)
				break;

			if( (map[pos_ghosts[ghost][Y]][pos_ghosts[ghost][X]+1] == SPAZIO || map[pos_ghosts[ghost][Y]][pos_ghosts[ghost][X]+1] == FOOD) && k == RIGHT)
				break;
		}
	}
	if(ghostBullets[ghost][k].available == false ) // Controllo qualità bullet
		pthread_exit(NULL);

	ghostBullets[ghost][k].available = false;
	ghostBullets[ghost][k].pid = self;
	ghostBullets[ghost][k].x = pos_ghosts[ghost][X];
	ghostBullets[ghost][k].y = pos_ghosts[ghost][Y];

	posX = ghostBullets[ghost][k].x;
	posY = ghostBullets[ghost][k].y;

	switch(k){
		case UP:
			posY--;
			break;
		case DOWN:
		 	posY++;
			break;
    case LEFT:
    	posX-=3;
    	break;
    case RIGHT:
    	posX+=3;
    	break;
	}

	oldX = posX;
	oldY = posY;
	centro = map[posY][posX];
	dx = map[posY][posX+1];
	sx = map[posY][posX-1];
	while( (sx == SPAZIO || sx == FOOD) && (centro == SPAZIO || centro == FOOD) && ( dx == SPAZIO || dx == FOOD ) && vittoria==0){


		// Controllo se la posizione del proiettile non va oltre i bordi della mappa
		if(map[posY-1][posX] != SPAZIO && map[posY-1][posX] != FOOD && k == UP)
			break;
		if(map[posY+1][posX] != SPAZIO && map[posY+1][posX] != FOOD && k == DOWN)
			break;
		if(map[posY][posX-2] != SPAZIO && map[posY][posX-2] != FOOD && k == LEFT)
			break;
		if(map[posY][posX+2] != SPAZIO && map[posY][posX+2] != FOOD && k == RIGHT)
			break;


		centro = map[posY][posX];
		dx = map[posY][posX+1];
		sx = map[posY][posX-1];
		switch(k){
			case UP:
				posY--;
				break;
			case DOWN:
			 	posY++;
				break;
	    case LEFT:
	    	posX-=3;
	    	break;
	    case RIGHT:
	    	posX+=3;
	    	break;
		}


		// Effetto Pacman per i proiettili
		if(posY == 16){
			if(posX == 82) // Pacman passa da destra a sinistra
				posX = 1;
			else
				if(posX == 1) // Pacman passa da sinistra a destra
					posX = 82;
		}


		if(posY == pacy && posX == pacx){

			if(colpito == 0){

				pthread_mutex_lock(&mutexR);

				if(armatura>0){
					armatura--;
				}else{
					vite--;
					armatura=9;
				}
				colpito=1;
				pthread_mutex_unlock(&mutexR);
				pthread_mutex_lock(&mutex);
				mvprintw(MAXY, MAXX/2, "                        ");
				mvprintw(MAXY,MAXX/2,"VITE %d ARMATURA %d",vite,armatura);
				pthread_mutex_unlock(&mutex);
			}
		}
		pthread_mutex_lock(&mutex);
		wattron(win,COLOR_PAIR(2));
		switch (map[oldY][oldX]) {
			case SPAZIO:
				mvwaddch(win,oldY,oldX, ' ');
				break;
			case LINEA_VERTICALE:
				mvwaddch(win,oldY,oldX,ACS_VLINE);
				break;
			case LINEA_ORIZZONTALE:
				mvwaddch(win,oldY,oldX,ACS_HLINE);
				break;
			case BORDO_LL:
				mvwaddch(win,oldY,oldX,ACS_LLCORNER);
				break;
			case BORDO_LR:
				mvwaddch(win,oldY,oldX,ACS_LRCORNER);
				break;
			case BORDO_UL:
				mvwaddch(win,oldY,oldX,ACS_ULCORNER);
				break;
			case BORDO_UR:
				mvwaddch(win,oldY,oldX,ACS_URCORNER);
				break;
			case SCAN_LINE1:
				mvwaddch(win,oldY,oldX,ACS_S1);
				break;
			case SCAN_LINE3:
				mvwaddch(win,oldY,oldX,ACS_S3);
				break;
			case SCAN_LINE7:
				mvwaddch(win,oldY,oldX,ACS_S7);
				break;
			case SCAN_LINE9:
				mvwaddch(win,oldY,oldX,ACS_S9);
				break;
			case FOOD:
				wattroff(win, COLOR_PAIR(2));
				wattron(win,COLOR_PAIR(1));
				mvwaddch(win,oldY,oldX,ACS_BULLET);
				wattroff(win, COLOR_PAIR(1));
				wattron(win,COLOR_PAIR(2));
				break;

			}

		if(ghost == 0)
			wattron(win, COLOR_PAIR(PAIR_1_BULLET));
		if(ghost == 1)
			wattron(win, COLOR_PAIR(PAIR_2_BULLET));
		if(ghost == 2)
			wattron(win, COLOR_PAIR(PAIR_3_BULLET));
		if(ghost == 3)
			wattron(win, COLOR_PAIR(PAIR_4_BULLET));
		if(ghost == 4)
			wattron(win, COLOR_PAIR(PAIR_5_BULLET));

		wattron(win, A_BOLD);
		switch(k){
			case UP:
				mvwaddch(win,posY,posX, '^');
				break;
			case DOWN:
				mvwaddch(win,posY,posX, 'v');
				break;
			case LEFT:
				mvwaddch(win,posY,posX, '<');
				break;
			case RIGHT:
				mvwaddch(win,posY,posX, '>');
				break;
		}
		wattroff(win, COLOR_PAIR(3));
		wattroff(win, A_BOLD);
		wrefresh(win);
		pthread_mutex_unlock(&mutex);

		oldX = posX;
		oldY = posY;
		ghostBullets[ghost][k].x = posX;
		ghostBullets[ghost][k].y = posY;

		usleep(SLEEP_TIME-12000); // Velocità proiettile

	}


wattron(win,COLOR_PAIR(2));
switch (map[posY][posX]) {
	case SPAZIO:
		mvwaddch(win,posY,posX, ' ');
		break;
	case LINEA_VERTICALE:
		mvwaddch(win,posY,posX,ACS_VLINE);
		break;
	case LINEA_ORIZZONTALE:
		mvwaddch(win,posY,posX,ACS_HLINE);
		break;
	case BORDO_LL:
		mvwaddch(win,posY,posX,ACS_LLCORNER);
		break;
	case BORDO_LR:
		mvwaddch(win,posY,posX,ACS_LRCORNER);
		break;
	case BORDO_UL:
		mvwaddch(win,posY,posX,ACS_ULCORNER);
		break;
	case BORDO_UR:
		mvwaddch(win,posY,posX,ACS_URCORNER);
		break;
	case SCAN_LINE1:
		mvwaddch(win,posY,posX,ACS_S1);
		break;
	case SCAN_LINE3:
		mvwaddch(win,posY,posX,ACS_S3);
		break;
	case SCAN_LINE7:
		mvwaddch(win,posY,posX,ACS_S7);
		break;
	case SCAN_LINE9:
		mvwaddch(win,posY,posX,ACS_S9);
		break;
	case FOOD:
		wattroff(win, COLOR_PAIR(2));
		wattron(win,COLOR_PAIR(1));
		mvwaddch(win,posY,posX,ACS_BULLET);
		wattroff(win, COLOR_PAIR(1));
		wattron(win,COLOR_PAIR(2));
		break;

	}
ghostBullets[ghost][k].pid = -1;
ghostBullets[ghost][k].available = true;
ghostBullets[ghost][k].x = -1;
ghostBullets[ghost][k].y = -1;
pthread_mutex_lock(&mutex);
wrefresh(win);
pthread_mutex_unlock(&mutex);
pthread_exit(NULL);
}

void * spawn_pills(void * null){
  FILE* fp=NULL;
  int i, j;
  int flag=0;
  char c;
  char check_map[MAXY][MAXX];
  pthread_t  animation;
  pos_pills[MAX_PILLS][0]=0;
  fp=fopen("convert.txt", "r");
  if(fp==NULL)
  return 0;
  for(i=0;i<MAXY;i++){
    for(j=0;j<MAXX;j++){

      c=' ';
      fscanf(fp,"%c",&c);

      if(c!=EOF)
      check_map[i][j]=c;

      if(c==' ')
      check_map[i][j]=SPAZIO;


    }
  }
  fclose(fp);
  pthread_create(&animation,NULL,&animate_pills,NULL);
    //genero posizione power pill
    int indice;
    for(indice=0;indice<pos_pills[MAX_PILLS][0];indice++){
      pos_pills[indice][0]=0;
      pos_pills[indice][1]=0;
    }
    while(vittoria==0){
      while(pos_pills[MAX_PILLS][0]<MAX_PILLS){
        sleep(DELAY_SPAWN_PILL);
        flag=1;
        while(flag){
          i=1+rand()%MAXY-2;
          j=1+rand()%MAXX-2;
          if(map[i][j]==SPAZIO && check_map[i][j]==FOOD){
            flag=0;
            for(indice=0;indice<MAX_PILLS;indice++){
              if(pos_pills[indice][0]==0 && pos_pills[indice][1]==0){
                pthread_mutex_lock(&mutexR);
                pos_pills[indice][0]=i;
                pos_pills[indice][1]=j;
                  pos_pills[MAX_PILLS][0]++;
                pthread_mutex_unlock(&mutexR);
                indice=MAX_PILLS;
              }
            }
          }
        }
      }
    }
  }

void * animate_pills(void * null){

    int i;

    while(vittoria==0){
      //se una pillola è stata mangiata
      for(i=0;i<MAX_PILLS;i++){
        if(pos_pills[i][0]==pacy && pos_pills[i][1]==pacx){

            //qui ci metterai lo status change di pacman per ora metto solo lo score
          pthread_mutex_lock(&mutexR);
          score+=50;
          pos_pills[i][0]=0;
          pos_pills[i][1]=0;
          pos_pills[MAX_PILLS][0]--;

          pthread_mutex_unlock(&mutexR);
          i=MAX_PILLS;
        }
      }
        for(i=0;i<MAX_PILLS;i++){
          if(pos_pills[i][0]!=0 && pos_pills[i][1]!=0 ){

						pthread_mutex_lock(&mutex);
            wattron(win,COLOR_PAIR(ROSSO));
            mvwaddch(win,pos_pills[i][0],pos_pills[i][1]-1, 'o');
            mvwaddch(win,pos_pills[i][0],pos_pills[i][1]+1, 'o');
            wattron(win,COLOR_PAIR(VERDE));
            mvwaddch(win,pos_pills[i][0],pos_pills[i][1], '^');
          	pthread_mutex_unlock(&mutex);
      }
    }
    usleep(10000);
  }
}

void * spawn_power_up(void * null){

    FILE* fp=NULL;
    int i, j;
    char c;
    char check_map[MAXY][MAXX];
    pthread_t animatio;
    pos_power_up[MAX_POWER_UP][0]=0;
    fp=fopen("convert.txt", "r");
    if(fp==NULL)
    return 0;
    for(i=0;i<MAXY;i++){
      for(j=0;j<MAXX;j++){

        c=' ';
        fscanf(fp,"%c",&c);

        if(c!=EOF)
        check_map[i][j]=c;

        if(c==' ')
          check_map[i][j]=SPAZIO;


      }
    }
    fclose(fp);
      //genero posizione power pill
      int indice;

			// Inizializzo powerUp
      for(indice=0;indice<MAX_POWER_UP;indice++){
        pos_power_up[indice][0]=0;
        pos_power_up[indice][1]=0;
				pos_power_up[indice][2]=1;
      }
			pthread_create(&animatio,NULL,&animate_power_up,NULL);

      while(vittoria==0){
        while(pos_power_up[MAX_POWER_UP][0]<MAX_POWER_UP){

              for(indice=0;indice<MAX_POWER_UP;indice++){
                if(pos_power_up[indice][0] == 0 && pos_power_up[indice][1] == 0){

                  switch (indice) {
                    case 0:
	                    pos_power_up[indice][0]=4;
	                    pos_power_up[indice][1]=4;
	                    break;
                    case 1:
	                    pos_power_up[indice][0]=25;
	                    pos_power_up[indice][1]=4;
	                    break;
                    case 2:
	                    pos_power_up[indice][0]=4;
	                    pos_power_up[indice][1]=MAXX-6;
	                    break;
                    case 3:
	                    pos_power_up[indice][0]=25;
	                    pos_power_up[indice][1]=MAXX-6;
	                    break;
                  }
                  pthread_mutex_lock(&mutexR);
                  pos_power_up[MAX_POWER_UP][0]++;
                  pthread_mutex_unlock(&mutexR);
                  indice=MAX_POWER_UP;
                }
          }
        }
      }
    }

void * animate_power_up(void * null){
    //fai uscire il punteggio sus schermo
      int i;

      while(pos_power_up[MAX_POWER_UP][0] > 0 && vittoria==0){
        //se una pillola è stata mangiata
        for(i=0;i<MAX_POWER_UP;i++){


          if(pos_power_up[i][0]==pacy && pos_power_up[i][1]==pacx && pos_power_up[i][2] == 1){

            pthread_mutex_lock(&mutexR);

            pos_power_up[i][0]=0;
            pos_power_up[i][1]=0;
            pos_power_up[MAX_POWER_UP][0]--;
						pos_power_up[i][2]=0;
            pthread_mutex_unlock(&mutexR);

            new_freeze(NULL);
            i=MAX_POWER_UP;
          }
        }


        for(i=0;i<MAX_POWER_UP;i++){

          if(pos_power_up[i][0]!=0 && pos_power_up[i][1]!=0 && 	pos_power_up[i][2] == 1){

						pthread_mutex_lock(&mutex);
						wattron(win, COLOR_PAIR(BIANCO));
						wattron(win, A_BLINK);
						wattron(win, A_BOLD);
						mvwaddch(win, pos_power_up[i][0], pos_power_up[i][1], ACS_DIAMOND);
						wattroff(win, A_BLINK);
						wattroff(win, A_BOLD);
						pthread_mutex_unlock(&mutex);
        }
      }

          usleep(10000);
      }
    }

void * new_freeze(void * null){

	int indice;
	int start_time = clock()/CLOCKS_PER_SEC;
	int current_time = clock()/CLOCKS_PER_SEC;


	for(indice=0;indice<MAX_GHOSTS;indice++){
	  pthread_mutex_lock(&mutexR);
	  if(pos_ghosts[indice][2]==1 && ghost_mode[indice]!=2){

	      ghost_mode[indice]=1;

	  }
	  pthread_mutex_unlock(&mutexR);

	}
	//current_time<=start_time+INV_TIME
	while(current_time<=start_time+INV_TIME ){

	  current_time=clock()/CLOCKS_PER_SEC;
	  //per ogni fantasma

	}
	for(indice=0;indice<MAX_GHOSTS;indice++){
	pthread_mutex_lock(&mutexR);
	if(pos_ghosts[indice][2]==1 && ghost_mode[indice]!=2){

	  ghost_mode[indice]=0;

	}
	pthread_mutex_unlock(&mutexR);
	}

}

void reach_base(int id_ghost,int out){

		int su,g,d,s;
		int base_x;
		int base_y;
    int color;

		pthread_mutex_lock(&mutexR);
    switch (id_ghost) {
      case 0:
      color=PAIR_1_GHOST;
      break;
      case 1:
      color=PAIR_2_GHOST;
      break;
      case 2:
      color=PAIR_3_GHOST;
      break;
      case 3:
      color=PAIR_4_GHOST;
      break;
      case 4:
      color=PAIR_5_GHOST;
      break;
    }

    switch (out) {
      case 0:
      switch (id_ghost%4) {
        case 0:
          base_x=34;
          base_y=16;
          break;
        case 1:
        base_x=40;
        base_y=16;
        break;
        case 2:
        base_x=43;
        base_y=16;
        break;
        case 3:
        base_x=49;
        base_y=16;
        break;
    }
    break;
    case 1:
    switch (id_ghost%4) {
      case 0:
      base_x=40;
      base_y=12;
      break;
      case 1:
      base_x=40;
      base_y=12;
      break;
      case 2:
      base_x=43;
      base_y=12;
      break;
      case 3:
      base_x=43;
      base_y=12;
      break;

    }
    break;
    case 2:
    switch (id_ghost%4) {
      case 0:
      base_x=40;
      base_y=12;
      break;
      case 1:
      base_x=40;
      base_y=12;
      break;
      case 2:
      base_x=43;
      base_y=12;
      break;
      case 3:
      base_x=43;
      base_y=12;
      break;

    }
    break;


  }
  pthread_mutex_unlock(&mutexR);

    int movsu=0;
    int movgiu=0;
    int movsin=0;
    int movdes=0;
    int flag;
    int difference_x;
    int difference_y;
    int direction=0;
    int corex=pos_ghosts[id_ghost][1];
    int corey=pos_ghosts[id_ghost][0];
    while(corex!=base_x || corey!=base_y ){
      movsu=0;
      movgiu=0;
      movsin=0;
      movdes=0;


       direction=0;
       corex=pos_ghosts[id_ghost][1];
       corey=pos_ghosts[id_ghost][0];
    if(corex!=base_x || corey!=base_y){
    //movimenti possibili
    //status_ghosts[id_ghost][0]=0;
    if((map[pos_ghosts[id_ghost][0]-1][pos_ghosts[id_ghost][1]+1]==SPAZIO && map[pos_ghosts[id_ghost][0]-1][pos_ghosts[id_ghost][1]-1]==SPAZIO || map[pos_ghosts[id_ghost][0]-1][pos_ghosts[id_ghost][1]]=='i'  ) && status_ghosts[id_ghost][0]!=GIU){
      movsu=1;
      flag=1;
    }
    if((map[pos_ghosts[id_ghost][0]+1][pos_ghosts[id_ghost][1]+1]==SPAZIO && map[pos_ghosts[id_ghost][0]+1][pos_ghosts[id_ghost][1]-1]==SPAZIO || map[pos_ghosts[id_ghost][0]+1][pos_ghosts[id_ghost][1]]=='i'  ) && status_ghosts[id_ghost][0]!=SU){
      movgiu=1;
      flag=1;
    }
    if((map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]+3+1]==SPAZIO && map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]+3-1]==SPAZIO) && status_ghosts[id_ghost][0]!=SINISTRA && (pos_ghosts[id_ghost][0]!=16 || pos_ghosts[id_ghost][1]!=64) ){
      movdes=1;
      flag=1;
    }
    if((map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]-3+1]==SPAZIO && map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]-3-1]==SPAZIO) && status_ghosts[id_ghost][0]!=DESTRA && (pos_ghosts[id_ghost][0]!=16 || pos_ghosts[id_ghost][1]!=19) ){
      movsin=1;
      flag=1;
    }
    difference_x=pos_ghosts[id_ghost][1]-base_x;
    difference_y=pos_ghosts[id_ghost][0]-base_y;
    //difference_x/=2.2;
    //qui per gli abs

    int abs_x=difference_x;
    int abs_y=difference_y;

    if(difference_x<0){
      abs_x=difference_x*(-1);
    }

    if(difference_y<0){
      abs_y=difference_y*(-1);
    }




  if(difference_x<0){
    d=sqrt((abs_x-1)*(abs_x-1)+(abs_y)*(abs_y));
    s=sqrt((abs_x+1)*(abs_x+1)+(abs_y)*(abs_y));
  }else{
    d=sqrt((abs_x+1)*(abs_x+1)+(abs_y)*(abs_y));
    s=sqrt((abs_x-1)*(abs_x-1)+(abs_y)*(abs_y));
  }
  if(difference_y>0){

    su=sqrt((abs_x)*(abs_x)+(abs_y-1)*(abs_y-1));
    g=sqrt((abs_x)*(abs_x)+(abs_y+1)*(abs_y+1));
  }else{
    su=sqrt((abs_x)*(abs_x)+(abs_y+1)*(abs_y+1));
    g=sqrt((abs_x)*(abs_x)+(abs_y-1)*(abs_y-1));
  }

/*

if(difference_x<0){
  d=(abs_x-1)*(abs_y);
  s=(abs_x+1)*(abs_y);
}else{
  d=(abs_x+1)*(abs_y);
  s=(abs_x-1)*(abs_y);
}
if(difference_y>0){
  su=(abs_x)*(abs_y-1);
  g=(abs_x)*(abs_y+1);
}else{
  su=(abs_x)*(abs_y+1);
  g=(abs_x)*(abs_y-1);
}
*/


    if(movsu==0){
      su=MAXX*MAXY;
    }
    if(movdes==0){
      d=MAXX*MAXY;
    }
    if(movsin==0){
      s=MAXX*MAXY;
    }
    if(movgiu==0){
      g=MAXX*MAXY;
    }


    int min=MAXX*MAXY;
    int save_i;
    int ord[4];
    int ord_ord[4];
    int i,j;
    ord[0]=d;
    ord[1]=s;
    ord[2]=su;
    ord[3]=g;
    for(i=0;i!=4;i++){
      for(j=0;j!=4;j++){
        if(ord[j]<min){
          min=ord[j];
          save_i=j;
        }
      }
      ord[save_i]=MAXX*MAXY;
      ord_ord[i]=min;
      min=MAXX*MAXY;
    }
    for(i=0;i!=4;i++){
      if(ord_ord[i]==d && movdes==0){
        ord_ord[i]=0;
      }
      if(ord_ord[i]==s && movsin==0){
        ord_ord[i]=0;
      }
      if(ord_ord[i]==su && movsu==0){
        ord_ord[i]=0;
      }
      if(ord_ord[i]==g && movgiu==0){
        ord_ord[i]=0;
      }
    }

    for(i=0;i<4;i++){
      if(ord_ord[i]==d){
        status_ghosts[id_ghost][1]=DESTRA;
        i=10;
      }
      if(ord_ord[i]==s){
        status_ghosts[id_ghost][1]=SINISTRA;
        i=10;
          }
      if(ord_ord[i]==su){
        status_ghosts[id_ghost][1]=SU;
        i=10;
      }
      if(ord_ord[i]==g){
        status_ghosts[id_ghost][1]=GIU;
        i=10;
      }
    }




    // mi muovo in quella direzione aggiornando la posizione locale
      if( map[pos_ghosts[id_ghost][0]-1][pos_ghosts[id_ghost][1]]=='i'  && movsu==1 && out==1){
        status_ghosts[id_ghost][1]=SU;
      }
      if( map[pos_ghosts[id_ghost][0]+1][pos_ghosts[id_ghost][1]]=='i'  && movgiu==1 && out==0){
        status_ghosts[id_ghost][1]=GIU;
      }
      switch(status_ghosts[id_ghost][1]){
        case SINISTRA:
        corex-=3;
        break;
        case DESTRA:
        corex+=3;
        break;
        case SU:
        corey--;
        break;
        case GIU:
        corey++;
        break;
        }


    //scrivo nella nuova posizione il corpo del fantasma
    if(corey!=pacy || corex!=pacx){
    //pthread_mutex_lock(&mutex);
		pthread_mutex_lock(&mutex);
    wattron(win,COLOR_PAIR(BIANCO));
    mvwaddch(win,corey,corex-1, '(');
    mvwaddch(win,corey,corex, 'o');
    mvwaddch(win,corey,corex+1, '<');
    wattroff(win,COLOR_PAIR(BIANCO));
		pthread_mutex_unlock(&mutex);
    //pthread_mutex_unlock(&mutex);
    }
    if(pos_ghosts[id_ghost][1]!=corex || pos_ghosts[id_ghost][0]!=corey){
      // se mi sono effettivamente spostato allora cancello la vecchia posizione salvata nella posizione globalmente
      // nota bene che non è stata ancora modificata
      if(pacx==pos_ghosts[id_ghost][1] && pacy==pos_ghosts[id_ghost][0]){

      }
      else{

			pthread_mutex_lock(&mutex);
      wattron(win,COLOR_PAIR(COLOR_BLUE));
      mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1]-1, ' ');
      mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1]+1, ' ');
      wattroff(win,COLOR_PAIR(COLOR_BLUE));
      //se al centro c'era un pallino lo rimetto altrimenti metto spazio
      pthread_mutex_unlock(&mutex);

      if(map[pos_ghosts[id_ghost][0]][pos_ghosts[id_ghost][1]]==FOOD){
        pthread_mutex_lock(&mutex);
    		wattron(win,COLOR_PAIR(COLOR_BLUE));
        wattron(win,COLOR_PAIR(1));
        mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1], ACS_BULLET);
        wattroff(win, COLOR_PAIR(1));
      	wattroff(win,COLOR_PAIR(COLOR_BLUE));
        pthread_mutex_unlock(&mutex);
      }else{
        pthread_mutex_lock(&mutex);
        wattron(win,COLOR_PAIR(COLOR_BLUE));
        mvwaddch(win,pos_ghosts[id_ghost][0],pos_ghosts[id_ghost][1], ' ');
        wattroff(win,COLOR_PAIR(COLOR_BLUE));
        pthread_mutex_unlock(&mutex);
      }
      //salvo la posizione dell'ultima mossa

    }

    }
    //aggiono finalmente gli indici globali
pthread_mutex_lock(&mutexR);
status_ghosts[id_ghost][0]=status_ghosts[id_ghost][1];
pthread_mutex_unlock(&mutexR);

pthread_mutex_lock(&mutex);
wrefresh(win);
pthread_mutex_unlock(&mutex);
pthread_mutex_lock(&mutexR);
pos_ghosts[id_ghost][1]=corex;
pos_ghosts[id_ghost][0]=corey;
pthread_mutex_unlock(&mutexR);
usleep(90000);

  }
  }
  if(out==2){
    reach_base(id_ghost,0);
  }if(out==0){
    sleep(3);
    reach_base(id_ghost,1);
    ghost_mode[id_ghost]=0;
  }

}

void print_frozen_ghost(int indice){

  for(indice=0;indice<MAX_GHOSTS;indice++){
	  if(pos_ghosts[indice][2]==1 && ghost_mode[indice]==1){
	    pthread_mutex_lock(&mutex);
	    wattron(win,COLOR_PAIR(BLU));
	    mvwaddch(win,pos_ghosts[indice][0],pos_ghosts[indice][1]-1, '(');
	    mvwaddch(win,pos_ghosts[indice][0],pos_ghosts[indice][1], 'o');
	    mvwaddch(win,pos_ghosts[indice][0],pos_ghosts[indice][1]+1, '<');
	    wattroff(win,COLOR_PAIR(BLU));
	    pthread_mutex_unlock(&mutex);
	  }
	}
}


void * vite_armatura(void * null){


		int start_time=clock()/CLOCKS_PER_SEC;
		int current_time=clock()/CLOCKS_PER_SEC;
		int x;


		pthread_mutex_lock(&mutex);
		mvprintw(MAXY,MAXX/2,"VITE %d ARMATURA %d",vite,armatura);

		pthread_mutex_unlock(&mutex);

		while(vittoria==0){
		for(x=0;x!=MAX_GHOSTS;x++){
			if(pacy == pos_ghosts[x][0] && pacx==pos_ghosts[x][1] && ghost_mode[x]==0){
				current_time=clock()/CLOCKS_PER_SEC;
				if(current_time>=start_time+10){

					pthread_mutex_lock(&mutexR);
					vite--;
					pthread_mutex_unlock(&mutexR);

					pthread_mutex_lock(&mutex);
					mvprintw(MAXY, MAXX/2, "                        ");
					mvprintw(MAXY,MAXX/2,"VITE %d ARMATURA %d",vite,armatura);
					pthread_mutex_unlock(&mutex);

					start_time=clock()/CLOCKS_PER_SEC;
				}
			}
		}
	}
}
