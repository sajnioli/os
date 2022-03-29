1.SLUČAJ

student@ubuntu:~/os-lab/lab1$ make pokreni
cc lab1.c -Wall -O -lm -o lab1
Program s PID=3358 krenuo s radom
krecem s radom, zadnji broj=1
kraj rada, zadnji broj=6
Program: iteracija 5
krecem s radom, zadnji broj=6
kraj rada, zadnji broj=11
Program: iteracija 5
krecem s radom, zadnji broj=11
Pocetak obrade signala 10
Obrada signala 10: 1/5
Obrada signala 10: 2/5
Obrada signala 10: 3/5
Obrada signala 10: 4/5
Obrada signala 10: 5/5
Kraj obrade signala 10
12

2.SLUČAJ

student@ubuntu:~/os-lab/lab1$ make pokreni
cc lab1.c -Wall -O -lm -o lab1
Program s PID=3507 krenuo s radom
krecem s radom, zadnji broj=1
kraj rada, zadnji broj=6
Program: iteracija 5
krecem s radom, zadnji broj=6
Primio signal SIGTERM, pospremam prije izlazska iz programa
kraj rada, zadnji broj=11
Program: iteracija 5
Program s PID=3507 zavrsio s radom


status.txt -> 11


3.SLUČAJ

student@ubuntu:~/os-lab/lab1$ make pokreni
cc lab1.c -Wall -O -lm -o lab1
Program s PID=3443 krenuo s radom
krecem s radom, zadnji broj=1
kraj rada, zadnji broj=6
Program: iteracija 5
krecem s radom, zadnji broj=6
Primio signal SIGINT, prekidam rad


