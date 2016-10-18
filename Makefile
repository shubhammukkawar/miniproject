all:project
project:project.c
	gcc project.c -lncurses -lmenu -lform -o project
clean:
	rm *.o 
		
