all:project
project:project.c
	gcc -g project.c -lncurses -lmenu -lform -o project
clean:
	rm -rf *o 
		
