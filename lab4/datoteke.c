#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "datoteke.h"
#include "disk.h"

//neka u simulaciji (radi jednostavnosti) datotečna tablica bude u memoriji
static struct opisnik datotecna_tablica[MAX_DATOTEKA];

//slobodni blokovi na disku
static char slobodni_blokovi[BLOCKS];

void inicijaliziraj_prazni_disk()
{
	int i, j;

	memset(datotecna_tablica, 0, sizeof(struct opisnik) * MAX_DATOTEKA);
	for (i = 0; i < MAX_DATOTEKA; i++)
		for (j = 0; j < BLOCKS; j++)
			datotecna_tablica[i].blokovi[j] = -1;
	memset(slobodni_blokovi, 0, BLOCKS);
}
int otvori_datoteku(char *ime, int nacin)
{
	int i, slobodni_opisnik = -1;
	
	int status;
	FILE *fp;
	
	if (nacin != ZA_CITANJE && nacin != ZA_PISANJE) {
		fprintf(stderr, "'nacin' mora biti ZA_CITANJE ili ZA_PISANJE!\n");
		return -1;
	}
	for (i = 0; i < MAX_DATOTEKA; i++)
		if (datotecna_tablica[i].ime[0] == 0)
			slobodni_opisnik = i;
		
	if (i < MAX_DATOTEKA) { //postoji, otvori je kako je zadano
		if (datotecna_tablica[i].kazaljka > -1) {
			fprintf(stderr, "Datoteka je vec otvorena!\n");
			return -1;
		}
		datotecna_tablica[i].kazaljka = 0;
		datotecna_tablica[i].nacin_rada = nacin;
		return i;
	}
	else if (nacin == ZA_CITANJE) {
		fp = fopen(ime, "r");
		if(fp == NULL)
		{
		fprintf(stderr, "Datoteka ne postoji, ne moze se citati!\n");
		return -1;
		}
			else
			{
			char *ispis = malloc(sizeof(char) * 1024);
			status = fscanf(fp, "%s", ispis);
			while(status != -1)
			{
			printf("%s\n",ispis);
			status = fscanf(fp, "%s", ispis);
			}
			free(ispis);
			fclose(fp);
			}
	}
	else { //nacin == ZA_PISANJE
		if (slobodni_opisnik >= 0) {
			fprintf(stdout, "Datoteka ne postoji, stvaram novu!\n");
			i = slobodni_opisnik;
			strncpy(datotecna_tablica[i].ime, ime, IME_DULJINA);
			datotecna_tablica[i].velicina = 0;
			datotecna_tablica[i].kazaljka = 0;
			datotecna_tablica[i].nacin_rada = nacin;
			//ne treba inicijalizirati "blokove"
			return i;
		}
		else {
			fprintf(stderr, "Nema mjesta za novu datoteku\n");
			return -1;
		}
	}
	return i; //ne bi trebao nikada doci do tu...
}
int zatvori_datoteku(int id)
{
	datotecna_tablica[id].kazaljka = -1;

	return 0;
}

int procitaj(int id, void *p, size_t koliko)
{
	char buffer[BLOCK_SIZE];

	if (id < 0 || id >= MAX_DATOTEKA ||
		datotecna_tablica[id].ime[0] == 0 ||
		datotecna_tablica[id].kazaljka == -1)
	{
		fprintf(stderr, "Krivi id\n");
	}
	
	size_t jos = koliko;
	size_t kopirati, min;
	int blok_dat;
	int dohvaceno;
	
	blok_dat = datotecna_tablica[id].kazaljka / BLOCK_SIZE;
	
	while(jos > 0 && datotecna_tablica[id].kazaljka < datotecna_tablica[id].velicina && datotecna_tablica[id].blokovi[blok_dat] >= 0)
	{
	dohvaceno = dohvati_blok(id, buffer);
	if(dohvaceno == -1)
	printf("Nije proslo!\n");
	if(jos < datotecna_tablica[id].velicina - datotecna_tablica[id].kazaljka)
	min = jos;
	else
	min = datotecna_tablica[id].velicina - datotecna_tablica[id].kazaljka;
	if(min < BLOCK_SIZE - datotecna_tablica[id].kazaljka % BLOCK_SIZE)
	kopirati = min;
	else
	kopirati = BLOCK_SIZE - datotecna_tablica[id].kazaljka % BLOCK_SIZE;
	while(datotecna_tablica[id].kazaljka < koliko || datotecna_tablica[id].kazaljka < datotecna_tablica[id].blokovi[MAX_DATOTEKA] || datotecna_tablica[id].kazaljka < datotecna_tablica[id].velicina)
	p += kopirati;
	datotecna_tablica[id].kazaljka += kopirati;
	jos -= kopirati;
	blok_dat++;
	}
	
	return koliko - jos;
}
int zapisi(int id, void *p, size_t koliko)
{
	char buffer[BLOCK_SIZE];

	if (id < 0 || id >= MAX_DATOTEKA ||
		datotecna_tablica[id].ime[0] == 0 ||
		datotecna_tablica[id].kazaljka == -1)
	{
		fprintf(stderr, "Krivi id\n");
	}
	
	size_t jos = koliko;
	size_t kopirati;
	int max_vel_datoteke;
	int blok_dat;
	int dohvaceno;
	int zapisano;
	
	blok_dat = datotecna_tablica[id].kazaljka / BLOCK_SIZE;
	max_vel_datoteke = BLOCK_SIZE * BLOCKS;
	
	while(jos > 0 && datotecna_tablica[id].kazaljka < max_vel_datoteke)
	{
	if(datotecna_tablica[id].blokovi[blok_dat] == -1)
	{
	datotecna_tablica[id].blokovi[blok_dat] = slobodni_blokovi[BLOCKS];
	if(datotecna_tablica[id].blokovi[blok_dat] == -1)
	{
	printf("Nema slobodnih blokova!\n");
	exit(1);
	}
	}
	dohvaceno = dohvati_blok(id, buffer);
	if(dohvaceno == -1)
	printf("Nije proslo1!\n");
	if(jos < BLOCK_SIZE - datotecna_tablica[id].kazaljka % BLOCK_SIZE)
	kopirati = jos;
	else
	kopirati = BLOCK_SIZE - datotecna_tablica[id].kazaljka % BLOCK_SIZE;
	zapisano = pohrani_blok(id, datotecna_tablica[id].ime);
	if(zapisano == -1)
	printf("Nije proslo2!\n");
	p += kopirati;
	datotecna_tablica[id].kazaljka += kopirati;
	jos -= kopirati;
	blok_dat++;
	}
	
	if(datotecna_tablica[id].kazaljka > datotecna_tablica[id].velicina)
	datotecna_tablica[id].velicina = datotecna_tablica[id].kazaljka;
	
	return koliko - jos;
}
