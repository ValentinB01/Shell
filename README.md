# MyShell - Interpretator de Comenzi (Custom C Shell)

Un shell personalizat scris în C, creat pentru a emula comportamentul unui terminal clasic de Linux/Unix (precum bash). Proiectul oferă un mediu interactiv pentru rularea comenzilor externe și include suport pentru funcționalități avansate precum redirectări, pipeline-uri, procese în background, alias-uri și variabile de mediu.

## Funcționalități Principale

* **Rularea executabilelor:** Execută orice program din sistem aflat în `$PATH` (ex: `ls`, `grep`, `cat`).
* **Pipeline-uri (`|`):** Permite înlănțuirea mai multor comenzi, redirecționând ieșirea uneia către intrarea celeilalte.
* **Redirectări I/O (`<`, `>`, `>>`):** Redirecționează input-ul și output-ul standard din și către fișiere.
* **Operatori Logici (`&&`, `||`):** Execuție condiționată a comenzilor pe baza codului de retur (exit status).
* **Procese în Background (`&`):** Rulează comenzi în fundal fără a bloca prompt-ul terminalului. Gestiunea automată a proceselor "zombie" este asigurată prin manipularea semnalului `SIGCHLD`.
* **Variabile de Mediu:** Setează variabile cu `export NUME=Valoare` și folosește-le în comenzi prin expandare (ex: `echo $NUME`).
* **Alias-uri:** Scurtează comenzile lungi (ex: `alias ll='ls -la'`). Suportă comenzi built-in pentru `alias` și `unalias`.
* **Navigare Avansată:** Comanda `cd` permite schimbarea directoarelor, inclusiv revenirea la directorul anterior folosind `cd -`.
* **Istoric & Autocompletare:** Integrează GNU Readline pentru a oferi autocompletare la tasta TAB și navigare prin istoricul comenzilor folosind săgețile direcționale.
* **Gestiunea Semnalelor:** Răspunde corect la întreruperi: previne închiderea accidentală a shell-ului la `CTRL+C` și ignoră `CTRL+Z` pentru procesul părinte.

## Cerințe de Sistem

Pentru a compila și rula acest shell, ai nevoie de un mediu bazat pe Linux și de biblioteca **GNU Readline**.

Pe sisteme bazate pe Debian/Ubuntu, instalează dependențele cu:
sudo apt-get update
sudo apt-get install build-essential libreadline-dev


## Compilare și Rulare
Proiectul folosește un Makefile pentru a automatiza procesul de compilare.
Clonează repository-ul și navighează în folderul proiectului.

Compilează codul folosind comanda:
make

Pornește shell-ul:
./myshell

Pentru a curăța fișierele obiect și executabilul, folosește:
make clean

## Exemple de Utilizare
Iată câteva exemple de lucruri pe care le poți testa direct în MyShell:

### Folosirea Pipeline-urilor și a redirectărilor:
MyShell:/home/user$ ls -la | grep ".c" > fisiere_c.txt

### Procese în Background:
MyShell:/home/user$ sleep 10 &.
[background] PID 12345.
MyShell:/home/user$.

### Crearea Alias-urilor și a Variabilelor de Mediu:
* MyShell:/home/user$ export USERNAME=User
* MyShell:/home/user$ alias salut='echo Salutare, $USERNAME!'
* MyShell:/home/user$ salut
* Salutare, User!

### Navigare Rapidă:
MyShell:/home/user$ cd /tmp
MyShell:/tmp$ cd -
/home/user
