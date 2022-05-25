#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "signali.h"
#include "datoteke.h"

void promjena();

int novi_broj_dretvi;
int broj_dretvi = 3;
int postavljeni_broj_dretvi = 20;
int radi = 0;

pthread_mutex_t m;
pthread_cond_t red;

char *dat_stat, *dat_obrada, *dat_mreza;
int broj = 0;
int nije_kraj = 1;

/* funkcije koje rade dretve */
void *obrada(void *);
void *mreza(void *);
void *upr(void *);

/* funkcije za rad s datotekama */
int procitaj_status();
int dohvati_iz_cijevi();
void zapisi_status(int broj);
void dodaj_broj(int broj);
int pronadji_zadnji_broj();
/* funkcije za obradu signala, navedene ispod main-a */
void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);


int main(int argc, char *argv[])
{
	
	struct sigaction act;

	/* 1. maskiranje signala SIGUSR1 */

	/* kojom se funkcijom signal obradjuje */
	act.sa_handler = obradi_dogadjaj;

	/* koje jos signale treba blokirati dok se signal obradjuje */
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGTERM);

	act.sa_flags = 0; /* naprednije mogucnosti preskocene */

	/* maskiranje signala - povezivanje sucelja OS-a */
	sigaction(SIGUSR1, &act, NULL);

	/* 2. maskiranje signala SIGTERM */
	act.sa_handler = obradi_sigterm;
	sigemptyset(&act.sa_mask);
	sigaction(SIGTERM, &act, NULL);

	/* 3. maskiranje signala SIGINT */
	act.sa_handler = obradi_sigint;
	sigaction(SIGINT, &act, NULL);
	
	printf("Program s PID=%ld krenuo s radom\n", (long) getpid());
	
	if (argc < 4) {
		fprintf(stderr, "Koristenje: %s <status-datoteka> "
		 "<datoteka-s-podacima> <cjevovod>\n", argv[0]);
		exit(1);
	}
	dat_stat = argv[1];
	dat_obrada = argv[2];
	dat_mreza = argv[3];
	
	postavi_signale();
	
	zapisi_status(0);
	
	printf("Obrada kreće za 3 sekunde...\n");
	printf("Upiši broj tijekom izvođenja za promjenu broja!\n");
	sleep(3);
	
	pthread_t radna[postavljeni_broj_dretvi], mrezna, upravljacka;
	
	int i, id[postavljeni_broj_dretvi];
	
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&red, NULL);
	
	for(i = 0; i <= broj_dretvi; i++)
	{
	id[i]=i+1;
	if(pthread_create(&radna[i], NULL, obrada, &id[i]) !=0 )
	return 1;
	}
	
	if(pthread_create(&mrezna, NULL, mreza, NULL) !=0 )
	return 1;
	if(pthread_create(&upravljacka, NULL, upr, NULL) !=0 )
	return 1;
	
	
	while(nije_kraj){
	
	//upis 
	scanf("%d",&broj);
	
	if(broj > 0)
	zapisi_status(broj);
	
	else
	nije_kraj=0;
	
	}
	
	for(i = 0; i <= broj_dretvi; i++)
	{
	pthread_join(radna[i], NULL);
	}
	
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&red);
	
	return 0;
	
}
void promjena()
{
	pthread_mutex_lock(&m);

	int i, id[postavljeni_broj_dretvi];
	
	pthread_t radna[postavljeni_broj_dretvi];
	
	srand(time(0));
 	novi_broj_dretvi=(rand() % 6) + 1;
  	
  	if(broj_dretvi < postavljeni_broj_dretvi)
  	{
  		
  		for(i = broj_dretvi; i <= broj_dretvi + novi_broj_dretvi; i++)
  		{
  		id[i]=i+1;
		pthread_create(&radna[i], NULL, obrada, &id[i]);
  		}
  		
 	}
 	
 	broj_dretvi += novi_broj_dretvi;
 	
 	pthread_mutex_unlock(&m);
}
int procitaj_status()
{
	int broj;
	FILE *fp;

	fp = fopen(dat_stat, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_stat);
		exit(1);
	}
	if (fscanf(fp, "%d", &broj) != 1) {
		printf("Nije procitan broj iz %s!\n", dat_stat);
		exit(1);
	}
	fclose(fp);
	return broj;
}

void zapisi_status(int broj)
{
	FILE *fp;

	fp = fopen(dat_stat, "w");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_stat);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", dat_stat);
		exit(1);
	}
	fclose(fp);
}

void dodaj_broj(int broj)
{
	FILE *fp;

	fp = fopen(dat_obrada, "a");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_obrada);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", dat_obrada);
		exit(1);
	}
	fclose(fp);
}

int pronadji_zadnji_broj()
{
	FILE *fp;
	int broj = -1;

	fp = fopen(dat_obrada, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", dat_obrada);
		exit(1);
	}
	while(!feof(fp))
		if (fscanf(fp, "%d", &broj) != 1)
			break;
	fclose(fp);
	return broj;
}
void obradi_dogadjaj(int sig)
{
	int i;
	printf("Pocetak obrade signala %d\n", sig);
	for (i = 1; i <= 5; i++) {
		printf("Obrada signala %d: %d/5\n", sig, i);
		sleep(1);
	}
	printf("Kraj obrade signala %d\n", sig);
	printf("%d\n", broj);
}

void obradi_sigterm(int sig)
{
	printf("Primio signal SIGTERM, pospremam prije izlazska iz programa\n");
	nije_kraj = 0;
}

void obradi_sigint(int sig)
{
	printf("Primio signal SIGINT, prekidam rad\n");
	exit(1);
}

void *obrada(void *p)
{	
	int i = 1, x;
	int broj;
	int nije_kraj = 1;
	int izlaz = 0;
	
	int id=*((int*)p);
	
	if(id < 1 || id > postavljeni_broj_dretvi)
	return NULL;
	
	while(nije_kraj) {
	
	pthread_mutex_lock(&m);
	
		while(radi || broj % id != 0)
		pthread_cond_wait(&red, &m);
		
		if(broj_dretvi > postavljeni_broj_dretvi)
			izlaz = 1;
		
	radi=id;
	pthread_mutex_unlock(&m);
	
	if(izlaz)
	{
	printf("Premašen je postavljeni broj dretvi!\n");
	pthread_exit(NULL);
	}
	
	broj = procitaj_status();
	if (broj == 0) {
		printf("program bio prekinut, potraga za brojem...\n");
		broj = pronadji_zadnji_broj();
		broj = (int) sqrt(broj);
	}
	zapisi_status(0);
	
 	printf("Dretva RADNA[%d]: krecem s radom, zadnji broj=%d\n", id, broj);

		//obrada
		broj++;
		x = broj * broj;
		dodaj_broj(x);

	//kraj
	zapisi_status(broj);
	printf("Dretva RADNA[%d]: iteracija %d\n", id, i++);
	printf("Dretva RADNA[%d]: kraj rada, zadnji broj=%d\n", id, broj);

		sleep(3);
	
	pthread_mutex_lock(&m);
	radi = 0;
	pthread_mutex_unlock(&m);
	pthread_cond_broadcast(&red);
	sleep(1);

	}

	printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

	return NULL;
}

void *mreza(void *p)
{
	int broj;
	int i = 1, x;
	
	broj = dohvati_iz_cijevi();
	zapisi_status(broj);
	
	while(broj > 0){
	
	printf("Dretva MREŽNA_1: krecem s radom, zadnji broj=%d\n", broj);
	
		x = broj * broj;
		dodaj_broj(x);
		broj++;
	
	zapisi_status(broj);
	
	printf("Dretva MREŽNA_1: iteracija %d\n", i++);
	printf("Dretva MREŽNA_1: kraj rada, zadnji broj=%d\n", broj);

	broj = dohvati_iz_cijevi();
	}
	
	return NULL;
}

void *upr(void *p)
{
	while(1){
	
	sleep(1);
	printf("Dretva MREŽNA_2: krecem s radom za 3 sekunde...\n");
	sleep(3);
	printf("Dretva MREŽNA_2: Postavljam radni broj dretvi...\n");
	
	promjena();
	
	if(broj_dretvi > postavljeni_broj_dretvi)
	pthread_exit(NULL);
	
	sleep(2);
	}
	
	return NULL;
}


