student@ubuntu:~/os-lab/lab2$ make pokreni
cc -Wall -MMD   -c -o lab2.o lab2.c
cc -Wall -MMD   -c -o signali.o signali.c
cc -Wall -MMD   -c -o datoteke.o datoteke.c
cc -O lab2.o signali.o datoteke.o -lm -lpthread -o lab2
Program s PID=5441 krenuo s radom
Obrada kreće za 5 sekundi...
Upisi broj:5
Dretva RADNA: krecem s radom, zadnji broj=5
Dretva RADNA: iteracija 1
Dretva RADNA: kraj rada, zadnji broj=6
Dretva RADNA: krecem s radom, zadnji broj=6
Dretva RADNA: iteracija 2
Dretva RADNA: kraj rada, zadnji broj=7
Dretva RADNA: krecem s radom, zadnji broj=7
Dretva RADNA: iteracija 3
Dretva RADNA: kraj rada, zadnji broj=8
Dretva RADNA: krecem s radom, zadnji broj=8
Dretva RADNA: iteracija 4
Dretva RADNA: kraj rada, zadnji broj=9
Dretva RADNA: krecem s radom, zadnji broj=9
Dretva RADNA: iteracija 5
Dretva RADNA: kraj rada, zadnji broj=10
Dretva MREŽNA: krecem s radom, zadnji broj=232     ->  echo 232 > cijev
Dretva MREŽNA: iteracija 1
Dretva MREŽNA: kraj rada, zadnji broj=233
Program s PID=5441 zavrsio s radom
Dretva RADNA: krecem s radom, zadnji broj=233
Dretva RADNA: iteracija 6
Dretva RADNA: kraj rada, zadnji broj=234
Dretva RADNA: krecem s radom, zadnji broj=234
Dretva RADNA: iteracija 7
Dretva RADNA: kraj rada, zadnji broj=235
^CPrimio signal SIGINT, prekidam rad

