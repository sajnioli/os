oliver@oliver-virtual-machine:~/os-lab/lab4$ make pokreni
cc -Wall -MMD   -c -o lab4.o lab4.c
cc -Wall -MMD   -c -o datoteke.o datoteke.c
cc -Wall -MMD   -c -o disk.o disk.c
cc -O lab4.o datoteke.o disk.o  -o lab4

Nova naredba:
Unesi ime datoteke ili 'kraj' za kraj:
datoteka1.txt
Zelis otvoriti datoteku datoteka1.txt za citanje (c) ili pisanje (p)?
p
Datoteka ne postoji, stvaram novu!
Unesi tekst za upisivanje u datoteku (do 80 znakova)
asdasd
Koliko puta da ga upisem u datoteku?
5

Nova naredba:
Unesi ime datoteke ili 'kraj' za kraj:
datoteka1.txt
Zelis otvoriti datoteku datoteka1.txt za citanje (c) ili pisanje (p)?
c
asdasd
asdasd
asdasd
asdasd
asdasd

Nova naredba:
Unesi ime datoteke ili 'kraj' za kraj:
kraj

