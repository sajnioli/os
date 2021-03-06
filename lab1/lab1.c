#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>

char *dat_stat, *dat_obrada;
int broj = 0;
int nije_kraj=1;

/* funkcije za rad s datotekama */
int procitaj_status();
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
	int i=0, x;
	if (argc < 3) {
		printf("Koristenje: %s <status-datoteka> <datoteka-s-podacim>",
			argv[0]);
		return 1;
	}
	dat_stat = argv[1];
	dat_obrada = argv[2];

	/* simulacija rada */
	/* pocetak */
	while(nije_kraj) {
	broj = procitaj_status();
	if (broj == 0) {
		printf("program bio prekinut, potraga za brojem...\n");
		broj = pronadji_zadnji_broj();
		broj = (int) sqrt(broj);
	}
	zapisi_status(0); //radim
	printf("krecem s radom, zadnji broj=%d\n", broj);

	//simulacija obrade
	for (i = 0; i < 5; i++) {
		broj++;
		x = broj * broj;
		dodaj_broj(x);
		sleep(5);
	}

	//kraj
	zapisi_status(broj);
	printf("kraj rada, zadnji broj=%d\n", broj);

	
		printf("Program: iteracija %d\n", i++);
		sleep(1);
	}

	printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

	return 0;
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
