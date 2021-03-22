 #include "pacman.h"


char NEXT_MOVE=' ';
char prec='x'; // -1 indica che non è stata fatta ancora alcuna mossa. Pacman non potrà sparare

bool signalShotSent;
bulletSet pacmanBullets[4]={ {true, -1, -1, -1}, {true, -1, -1, -1}, {true, -1, -1, -1}, {true, -1, -1, -1} }; // UP, DOWN, LEFT, RIGHT bullets


// THREAD: questa funzione si autoassegna un proiettile di PacMan da gestire in base alla lista di quelli disponibili in pacmanBullets
void * pacmanShotsBullet(void * arg){

	int index;
	int k;
	int posX=0, posY=0;
	int oldX, oldY;
	int centro = map[posY][posX];
	int dx = map[posY][posX+1];
	int sx = map[posY][posX-1];
	pthread_t self;

	self = pthread_self();

	// Ciclo di assegnamento direzione proiettile.
	for(k=0; k<MAX_BULLETS_PER_TIME_PACMAN; k++){

		if(pacmanBullets[k].available){
			if( (map[pacy-1][pacx] == SPAZIO || map[pacy-1][pacx] == FOOD) && k == UP)
				break;
			if( (map[pacy+1][pacx] == SPAZIO || map[pacy+1][pacx] == FOOD) && k == DOWN)
				break;

			if( (map[pacy][pacx-2] == SPAZIO || map[pacy][pacx-2] == FOOD) && k == LEFT)
				break;

			if( (map[pacy][pacx+2] == SPAZIO || map[pacy][pacx+2] == FOOD) && k == RIGHT)
				break;
		}
	}
	if(pacmanBullets[k].available == false) // Controllo qualità bullet
		pthread_exit(NULL);

	pacmanBullets[k].available = false;
	pacmanBullets[k].pid = self;
	pacmanBullets[k].x = pacx;
	pacmanBullets[k].y = pacy;

	posX = pacmanBullets[k].x;
	posY = pacmanBullets[k].y;

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
	while( (sx == SPAZIO || sx == FOOD) && (centro == SPAZIO || centro == FOOD) && ( dx == SPAZIO || dx == FOOD) && vittoria==0){


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
		//collisione NUOVO
		for(index=0;index!=MAX_GHOSTS;index++){


			if((posX==pos_ghosts[index][1] || posX==pos_ghosts[index][1]-3 || posX==pos_ghosts[index][1]+3)  && (posY==pos_ghosts[index][0] || posY==pos_ghosts[index][0]-1	 || posY==pos_ghosts[index][0]+1)){
				pthread_mutex_lock(&mutexR);
				ghost_mode[index]=2;
				pthread_mutex_unlock(&mutexR);
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


		wattron(win, COLOR_PAIR(3));
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
		pacmanBullets[k].x = posX;
		pacmanBullets[k].y = posY;

		usleep(SLEEP_TIME-12000); // Velocità proiettile

	}


pthread_mutex_lock(&mutex);
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
	default:
		break;
	}
pthread_mutex_unlock(&mutex);

pthread_mutex_lock(&mutexR);
pacmanBullets[k].pid = -1;
pacmanBullets[k].available = true;
pacmanBullets[k].x = -1;
pacmanBullets[k].y = -1;
pthread_mutex_unlock(&mutexR);

pthread_mutex_lock(&mutex);
wrefresh(win);
pthread_mutex_unlock(&mutex);

pthread_exit(NULL);
}

void * movimenti(void * param){

  char c;

  while(vittoria==0){

    pthread_mutex_lock(&mutex);
    c=getch();
    pthread_mutex_unlock(&mutex);

    if(c == ERR)
      c = prec;
		if(c == ' ')
			c = SHOOT;
		if(c == 'w')
			c = SU;
		if(c == 's')
			c = GIU;
		if(c == 'a')
			c = SINISTRA;
		if(c == 'd')
			c = DESTRA;
    if(c == SU || c == GIU || c == DESTRA || c == SINISTRA || (c == SHOOT && prec != 'x')){


			// Lo sparo non deve alterare la mossa precedente di pacman
    	if(c != prec && c != SHOOT)
      	prec = c;

		  // Invio sparo e aspetto che il segnale venga letto dalla funzione pacman
			if(c == SHOOT)
				signalShotSent = true;

			if(signalShotSent)
      	c=SHOOT;
			NEXT_MOVE = c;
    }
  }
}

void * pacman(void * param){


  int corex = 0;
  int corey = 0;
  int prevScore=0;

  char c,prev;
  char lastMove;


  const pacStruct pacFigureRightOpen = {'-','-','{'};
  const pacStruct pacFigureRightClose = {'-','-','-'};
  const pacStruct pacFigureLeftOpen = {'}','-','-'};
  const pacStruct pacFigureLeftClose = {'-','-','-'};
  const pacStruct pacFigureUpOpen = {'-','v','-'};
  const pacStruct pacFigureUpClose = {'-','-','-'};
  const pacStruct pacFigureDownOpen = {'-','^','-'};
  const pacStruct pacFigureDownClose = {'-','-','-'};
  pacStatus pacstatus=CLOSE;
  int statusCount=0;
  pacStruct pacFigure = pacFigureRightClose;
  pthread_t movimenti_ID;
	pthread_t upBullet, downBullet, leftBullet, rightBullet; // PID dei diversi proiettili


  // Inizializzo posizione Pacman - SpawnPoint
  pacx=19;
  pacy=16;
	pthread_create(&movimenti_ID, NULL, &movimenti, NULL);

  while(vittoria==0){




    // Aggiorno posizione globale di PacMan
    corex=pacx;
    corey=pacy;
		pthread_mutex_lock(&mutex);
    nodelay(stdscr, TRUE);
    pthread_mutex_unlock(&mutex);
    lastMove=' ';

    c=NEXT_MOVE;

		// Confermo corretta ricezione del segnale dello sparo
		//mvwprintw(win, 5, 20, "1) x: %d y: %d 2) x: %d y: %d 3) x: %d y: %d 4) x: %d y: %d", pacmanBullets[0].x, pacmanBullets[0].y, pacmanBullets[1].x, pacmanBullets[1].y, pacmanBullets[2].x, pacmanBullets[2].y, pacmanBullets[3].x, pacmanBullets[3].y);
		if(c == SHOOT && signalShotSent == false)
			continue;

		if(c == SHOOT)
			signalShotSent = false;

    switch(c){

      case SU:

        if(corey-2 > 0)
          if(map[corey-1][corex] == SPAZIO || map[corey-1][corex] == FOOD){
            corey--;
            prev=c;
            lastMove=SU;
          }
        break;

      case GIU:

        if(corey+2 < MAXY)
          if(map[corey+1][corex] == SPAZIO || map[corey+1][corex] == FOOD){
            corey++;
            prev=c;
            lastMove=GIU;
          }
        break;

      case SINISTRA:

        if(corex-5 > 0 || (corey == 16 && corex > 0))
          if(map[corey][corex-2] == SPAZIO || map[corey][corex-2] == FOOD){
            corex -= 3;
            prev=c;
            lastMove=SINISTRA;
          }
        break;

      case DESTRA:

        if(corex+5 < MAXX || (corey == 16 && corex < MAXX))
          if(map[corey][corex+2] == SPAZIO || map[corey][corex+2] == FOOD){
            corex += 3;
            prev=c;
            lastMove=DESTRA;
          }
        break;
      case SHOOT:

				// Proiettile sparato verso l'alto
				//if(pacmanBullets[UP].available)
					if(map[corey-1][corex] == SPAZIO || map[corey-1][corex] == FOOD)
						pthread_create(&upBullet, NULL, &pacmanShotsBullet, NULL);


				// Proiettile sparato verso il basso
				//if(pacmanBullets[DOWN].available)
					if(map[corey+1][corex] == SPAZIO || map[corey+1][corex] == FOOD)
						pthread_create(&downBullet, NULL, &pacmanShotsBullet, NULL);


				// Proiettile sparato verso sinistra
				//if(pacmanBullets[LEFT].available) // TODO: controllare correttezza del valore -5
					if(map[corey][corex-2] == SPAZIO || map[corey][corex-2] == FOOD)
						pthread_create(&leftBullet, NULL, &pacmanShotsBullet, NULL);


				// Proiettile sparato verso destra
				//if(pacmanBullets[RIGHT].available)
					if(map[corey][corex+1] == SPAZIO || map[corey][corex+1] == FOOD)
						pthread_create(&rightBullet, NULL, &pacmanShotsBullet, NULL);

	      break;

    }


        // Controllo se la posizione è rimasta invariata perchè il movimento tentato in precedenza non era valido
        if(pacx == corex && pacy == corey && prev != c && c != SHOOT){ // TODO: controllare se questo c!=space non crei problemi

          switch(prev){

            case SU:

              if(corey-2 > 0)
                if(map[corey-1][corex] == SPAZIO || map[corey-1][corex] == FOOD){
                  corey--;
                  lastMove=SU;
                }
              break;

            case GIU:

              if(corey+2 < MAXY)
                if(map[corey+1][corex] == SPAZIO || map[corey+1][corex] == FOOD){
                  corey++;
                  lastMove=GIU;
                }
              break;

            case SINISTRA:

              if(corex-5 > 0 || (corey == 16 && corex > 0))
                if(map[corey][corex-2] == SPAZIO || map[corey][corex-2] == FOOD){
                  corex -= 3;
                  lastMove=SINISTRA;
                }
              break;

            case DESTRA:

              if(corex+5 < MAXX || (corey == 16 && corex < MAXX))
                if(map[corey][corex+2] == SPAZIO || map[corey][corex+2] == FOOD){
                  corex+=3;
                  lastMove=DESTRA;
                }
              break;

          }
        }



        // Effetto Pacman
        if(corey == 16){
          if(corex == 82) // Pacman passa da destra a sinistra
            corex = 1;
          else
            if(corex == 1) // Pacman passa da sinistra a destra
              corex = 82;
        }


        prevScore=score; // Utile per capire quando il punteggio cambia e far lampeggiare lo score

        // Vado avanti se la posizione è valida
        if(map[corey][corex] == SPAZIO || map[corey][corex] == FOOD){
          if(map[corey][corex+1] == SPAZIO && map[corey][corex-1] == SPAZIO){

            if(map[corey][corex]==FOOD){ // Mangia un pallino, assegno punteggio

              pthread_mutex_lock(&mutexR);
              map[corey][corex] = SPAZIO;
							pthread_mutex_unlock(&mutexR);
              score+=10;
							realPoints+=10;

            }


        // Stabilisco direzione di PacMan, aggiorno la grafica di conseguenza

				if(statusCount >2 ){
        	if( pacstatus == CLOSE || (pacx == corex && pacy == corey) )
          	pacstatus = OPEN;
      		else
          	pacstatus = CLOSE;
					statusCount=0;
				}
				statusCount++;

        switch(lastMove){ // Controllo l'ultima mossa valida per stabilire direzione

          case SU:

            if(pacstatus == OPEN)
              pacFigure=pacFigureUpOpen;
            else
              pacFigure=pacFigureUpClose;
            break;

          case GIU:

            if(pacstatus == OPEN)
              pacFigure=pacFigureDownOpen;
            else
              pacFigure=pacFigureDownClose;
            break;

          case SINISTRA:

            if(pacstatus == OPEN)
              pacFigure=pacFigureLeftOpen;
            else
              pacFigure=pacFigureLeftClose;
            break;

          case DESTRA:

            if(pacstatus == OPEN)
              pacFigure=pacFigureRightOpen;
            else
              pacFigure=pacFigureRightClose;
            break;

        }

        pthread_mutex_lock(&mutex);

        // Aggiorno graficamente la posizione di Pacman

        wattron(win,COLOR_PAIR(3));
				wattron(win, A_BOLD);
				if(lastMove == DESTRA)
        	mvwaddch(win,corey,corex-1, ACS_DEGREE);
				else
					mvwaddch(win,corey,corex-1, pacFigure.head);

        mvwaddch(win,corey,corex, pacFigure.middle);
				if(lastMove == SINISTRA)
					mvwaddch(win,corey,corex+1, ACS_DEGREE);
				else
        	mvwaddch(win,corey,corex+1, pacFigure.tail);
				wattroff(win, A_BOLD);
        wrefresh(win);

        if(pacx != corex || pacy != corey){

          wattron(win,COLOR_PAIR(2));
          mvwaddch(win,pacy,pacx-1, ' ');
          mvwaddch(win,pacy,pacx, ' ');
          mvwaddch(win,pacy,pacx+1, ' ');

        }
          // Aggiorno posizione globale di Pacman
          pacx=corex;
          pacy=corey;

    }
  }

    // Aggiorno punteggio stampato
    if(prevScore != score)
      attron(A_BOLD);
    mvprintw(MAXY,0,"SCORE: %d",score);
    if(prevScore != score)
      attroff(A_BOLD);
    wrefresh(win);
    pthread_mutex_unlock(&mutex);

		if(c != SHOOT)
    	usleep(SLEEP_TIME);

  }
}
