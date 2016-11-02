/*****************************************************************************
 * Copyright (C) Shubham Mukkawar shubhammukkawar@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include<stdio.h>
#include<menu.h>
#include<form.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<ctype.h>
#include<time.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
#define STARTX 30
#define STARTX1 105
#define STARTY 5
#define STARTY2 2
#define WIDTH 30
#define ITEM_FIELDS 8
#define SIZE 150
#define NEW 10
#define MODIFY 20
#define DISPLAY 30
#define SORT 40
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Globals Declaration*/
int count_item; // Global variable for no. of items
int count_tax; // Global variable for no. of taxes
char *set_value[6];
int current_hour,current_minute,day,month,year,invoice_number=1;
double tax_rate, discount, subtotal , grand_total, total_transaction;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Structures Declaration*/
typedef struct tax {
	char *name;
	double value;
}tax;
typedef struct item {
	int number;
	char *name;
	char *location;
	char *brand;
	int quantity;
	double rate;
}item; 
typedef struct bill {
	int number;
	char *brand;
	char *name;
	int quantity;
	double rate;
	double amount;
}bill; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Functions prototype*/
void print_time();
void init_screen();
void init_colorpair();
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int readline(FILE *fp, char *line, int count);
char *inttostring(char str[], int num);
char *trim(char *str);

void home_display();

void item_master();
void newitem(int mode);
void locations(WINDOW *w);
int  item_store(char **str);
void item_modify();
void modify_info(char **str);

void inventory_display();
void item_display(int mode);
void item_search();

void tax_menu() ;
int  tax_store(char **str) ;
void tax_print(WINDOW *w) ;

void bill_display();
void bill_additem();
void bill_view();
void bill_attributes();
void info_form();
void bill_print();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	int ch;
	
	WINDOW *menu_win;
	ITEM **my_items;			
	MENU *my_menu;
        int n_choices, i;
        char *choices[] = {"Home", "Item master", "Inventory" , "Tax master", "Bill", "Exit"};
                    
	init_screen();		// Initialize screen 	
	init_colorpair();	// Initialize colors
	
	// TEXT DISPLAY 
	attron(COLOR_PAIR(1) | A_STANDOUT | A_BOLD);
	mvprintw(1,75,"WELCOME TO GROCERY BILLING SYSTEM");
	attroff(COLOR_PAIR(1) | A_STANDOUT | A_BOLD);
	print_time();           /* Print current time and date */
	refresh();		/* update the screen */
	
	//bkgd(COLOR_PAIR(8)); //Changing stdscr's background color
	//refresh();
	//getmaxyx(stdscr,y,x);
	//printw("Window size is %d rows, %d columns.\n",y,x);
	
	/* Create the window to be associated with the menu */
	menu_win =newwin(5, 164, 5, 10);		
	box(menu_win, 0 , 0);
	wbkgd(menu_win, COLOR_PAIR(2));
	wrefresh(menu_win);
        keypad(menu_win, TRUE);
	
	/* Create items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]);
	my_items[i]=(ITEM *)NULL;	
	
	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set menu option not to show the description */
	menu_opts_off(my_menu, O_SHOWDESC);

	/* Set main window and sub window */
        set_menu_win(my_menu, menu_win);
        set_menu_sub(my_menu, derwin(menu_win, 1, 100, 2, 30));
	set_menu_format(my_menu, 1, 6);
	set_menu_mark(my_menu, " -> ");

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu,COLOR_PAIR(6) | A_REVERSE );
	set_menu_back(my_menu,COLOR_PAIR(1) );
	set_menu_grey(my_menu,COLOR_PAIR(2));

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(menu_win);
	//home_display();		// home screen as Default
	while((ch = wgetch(menu_win))) {		// 27 for Escape
	        print_time();				// Updating time
	        switch(ch) {
	        	
			case KEY_LEFT:
				menu_driver(my_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(my_menu, REQ_RIGHT_ITEM);
				break;
			case 10:					//ENTER 
				//mvprintw(LINES - 5, 0, "Item selected is : %s",item_name(current_item(my_menu)));
				pos_menu_cursor(my_menu);
				for(i = 1;i < 5;i++) {
						move(LINES - i,0);
						clrtoeol();
				}
				refresh();
				
				if(strcmp(item_name(current_item(my_menu)),"Home") == 0 ) 
						home_display();
				
				else if(strcmp(item_name(current_item(my_menu)),"Item master") == 0 ) 
					item_master();
				
				else if(strcmp(item_name(current_item(my_menu)),"Inventory") == 0 )
					inventory_display();
				
				else if(strcmp(item_name(current_item(my_menu)),"Tax master") == 0 ) 
					tax_menu();	
				
				else if(strcmp(item_name(current_item(my_menu)),"Bill") == 0 ) {
					bill_display();
				}
				else if(strcmp(item_name(current_item(my_menu)),"Exit") == 0 ) {
					/* Unpost and free all the memory taken up */
        				unpost_menu(my_menu);
        				free_menu(my_menu);
        				for(i = 0; i < n_choices; ++i)
                				free_item(my_items[i]);

					endwin();			/* clean up ncurses */	
					return 0;
				}
				break;
		}
                wrefresh(menu_win);
	}		
}	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Graphical Interface functions */
void init_screen() {
	initscr();			/* start curses mode/initialize ncurses*/
	cbreak();			/* Line buffering disabled */
	noecho();			/* Don't echo anything */
	keypad(stdscr, TRUE);		/* We can use ESC, keypad and all.	*/
	start_color();			/* starting colors */
}
/* It will initialize all the color pairs which can be use by any function */
void init_colorpair() {
   	init_pair(1, COLOR_CYAN, COLOR_BLACK);;	//init_pair(number, fgcolor, bgcolor)
	init_pair(2, COLOR_WHITE,COLOR_BLUE);
	init_pair(3, COLOR_BLUE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_BLACK); 
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6 ,COLOR_RED, COLOR_BLACK );
	init_pair(7, COLOR_BLACK, COLOR_CYAN);
	init_pair(8, COLOR_BLACK, COLOR_WHITE);	
}
/* It will print current date and time and also set the respective time globals which can be used by other functions.*/
void print_time(){
	time_t rawtime;
  	struct tm * timeinfo;
  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	attron(COLOR_PAIR(1) );
  	//mvprintw ( 1,120, "Current local time and date: %s", asctime (timeinfo) );
  	mvprintw ( 1,130, "[%02d %02d %4d %02d:%02d:%02d]",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
  	 					      timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  	attroff(COLOR_PAIR(1) );
  	current_hour = timeinfo->tm_hour;	// Setting time globals
  	current_minute = timeinfo->tm_min;
  	day= timeinfo->tm_mday;
  	month=timeinfo->tm_mon + 1;
  	year=timeinfo->tm_year + 1900;
}
/* It will print the given string in middle of the given width of specified colour (Used for headings)*/
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
	
	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color | A_BOLD | A_UNDERLINE);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color | A_BOLD | A_UNDERLINE);
	refresh();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Data processing functions */
char *trim(char *str) {
    char *end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) 		//Remove trailing whitespaces 
    	*end-- = '\0';	
    return str;
}
/*It will read line of given file and will store the given line as string and return the no. of characters read*/ 
int readline(FILE *fp, char *line, int count) {			
	int i = 0, ch;
	while((i < count) && ((ch = fgetc(fp)) != '\n') && (ch != EOF))
		line[i++] = ch;
	line[i] = '\0';
	return i;	
}
/* It will convert integer into a string */
char *inttostring(char str[], int num){
    int i, rem, len = 0, n;
    n = num;
    while (n != 0) {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++) {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
    return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Home Screen Display*/
void home_display() {
	WINDOW *home_win;
	FILE *fd, *fdr;
	FIELD *field[3];
	FORM  *home_form;
	char *number,*name,*brand,*quantity,*location,*rate; 
	int ch, rows, cols, i, j=0, start=0, height=20, count;
	char *user_name, *s[100], line[200];
	
	init_screen();
	init_colorpair();
	home_win =newwin(40, 164, 15, 10);	// Big Window 
	box(home_win, 0 , 0);	
	wbkgd(home_win, COLOR_PAIR(2));
	wrefresh(home_win);
	init_screen();		// Initialize screen 	
	init_colorpair();		// Initialize colors
	
	/* Initialize the fields */
	field[0] = new_field(1, 15, 3, STARTX, 0, 0);
	field[1] = new_field(1, 7, 5 , STARTX+3 , 0, 0);	// Enter button 
	field[2] = NULL;

	set_field_fore(field[0], COLOR_PAIR(2));
	set_field_back(field[0], COLOR_PAIR(2) | A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP); 
			
	set_field_fore(field[1], COLOR_PAIR(3));		
	set_field_back(field[1], COLOR_PAIR(3) | A_BOLD);
	set_field_buffer(field[1],0,"ENTER");
	field_opts_off(field[1], O_AUTOSKIP);
	field_opts_off(field[1], O_EDIT);
	
	home_form = new_form(field);
	scale_form(home_form, &rows, &cols);
	home_win = newwin(40, 164, 15, 10);
        keypad(home_win, TRUE);
	set_form_win(home_form, home_win);
        set_form_sub(home_form, derwin(home_win, rows, cols, 2, 2));
	box(home_win, 0, 0);
	print_in_middle(home_win, 1, 0, 75, "WELCOME", COLOR_PAIR(1));
	post_form(home_form);
	wrefresh(home_win);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );						// Instructions-home
	mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields/buttons");
	mvprintw(LINES -1, 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	
	mvwhline(home_win,11, 1,  ACS_HLINE, 163);
	mvwvline(home_win,1, 82,  ACS_VLINE, 10);			
	
	wattron(home_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(home_win, 3 + 2, STARTX - 20 , "USERNAME:");	
	wattroff(home_win , COLOR_PAIR(1) | A_BOLD);
	
	fdr=fopen("item.csv","r");	/*reading from a csv file */
	while(!feof(fdr)) {
		count = readline(fdr, line, 200);
		//printf("%s \n", line);		// "line" is the character array which stores the particular line.
		if(count == 0)
			break;
		number = strtok(line, ",");		
		name = strtok(NULL, ",");		
		location = strtok(NULL, ",");
		brand = strtok(NULL, ",");
		quantity = strtok(NULL, ",");
		rate = strtok(NULL, ",");
		if(atoi(quantity)==0) {
			s[j]=(char*)malloc(sizeof(char)* 30);
			strcpy(s[j],name);
			j++;
		}
	}
	fclose(fdr);
	while((ch = wgetch(home_win)) != 27) {
		switch(ch) {
			case KEY_DOWN:
				form_driver(home_form, REQ_NEXT_FIELD);
				form_driver(home_form, REQ_END_LINE);
				if((start+height) < j)
					start++;
				break;
			case KEY_UP:
				form_driver(home_form, REQ_PREV_FIELD);
				form_driver(home_form, REQ_END_LINE);
				if(start > 0)
					start--;
				break;
			case KEY_LEFT:
				form_driver(home_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(home_form, REQ_RIGHT_CHAR);
				break;	
			default:
				form_driver(home_form, ch);
				break;
			case KEY_BACKSPACE :
				form_driver(home_form,REQ_DEL_PREV);
				break;
			case 10 :
			if(current_field(home_form)==field[1] ) {
				user_name=field_buffer(field[0], 0);
				fd=fopen("username.txt","w");
				fprintf(fd,"%s",trim(user_name));
				fclose(fd);	
				/*Printing of messages according to current time */
				if((current_hour>=0) && (current_hour<12)) {
					wattron(home_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
					mvwprintw(home_win, 5, 100 , "Good Morning,%s",user_name);
					wattroff(home_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
				}
				else if((current_hour>=12) && (current_hour<16)) {
					wattron(home_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
					mvwprintw(home_win, 5, 100 , "Good Afternoon,%s",user_name);
					wattroff(home_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
				}
				else if((current_hour>16) && (current_hour<=24)) {
					wattron(home_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
					mvwprintw(home_win, 5, 100 , "Good Evening,%s",user_name);
					wattroff(home_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
				}		
			
			wattron(home_win , COLOR_PAIR(1) | A_BOLD | A_UNDERLINE | A_STANDOUT );
			mvwprintw(home_win, 12, 10 , "TODAY'S ALERTS");
			wattroff(home_win , COLOR_PAIR(1) | A_BOLD  | A_UNDERLINE | A_STANDOUT );
			mvwprintw(home_win, 14, 10 , "Total Transaction this session:Rs %6.2lf/-",total_transaction);
			wattron(home_win , COLOR_PAIR(1) | A_BOLD | A_UNDERLINE | A_STANDOUT );
			mvwprintw(home_win, 16, 25 , "ITEMS WITH NO STOCK LEFT");
			wattroff(home_win , COLOR_PAIR(1) | A_BOLD  | A_UNDERLINE | A_STANDOUT );
			for(i=start; i<(start+height) && i < j; i++) {	//j->item count with no stock
				wattron(home_win,COLOR_PAIR(5));
				mvwprintw(home_win,18 + (i-start),25 ,"%-3d)%-30s\n", i+1, s[i]);
				wattroff(home_win,COLOR_PAIR(5));
				box(home_win, 0 , 0);
				wrefresh(home_win);
			}
			}		
		}				
	}
	for(i=0;i<j;i++) {
		free(s[i]);
	}
	unpost_form(home_form);
	free_form(home_form);
	for(i = 0; i < 2; i++)
		free_field(field[i]); 
	wmove(home_win,0, 0); 
	wclear(home_win);
	wrefresh(home_win);
	delwin(home_win);	
	return;	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Item Master Display */
void item_master() {
	WINDOW *menu_win;
	MENU *item_menu;
	ITEM **my_items;
	int i, n_choices, ch;
	char *choices[] = {"New item" ,"Existing item" ,"Exit"};

	init_screen();
	init_colorpair();
	menu_win =newwin(3, 63, 11, 40);	// Menu Window
	 
	keypad(menu_win,TRUE);
	box(menu_win, 0 , 0);	
	wbkgd(menu_win, COLOR_PAIR(2));
	wrefresh(menu_win);
	
	n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]);
	my_items[i]=(ITEM *)NULL;	
	
	item_menu = new_menu((ITEM **)my_items);
	menu_opts_off(item_menu, O_SHOWDESC);
        set_menu_win(item_menu, menu_win);
        set_menu_sub(item_menu, derwin(menu_win, 1, 55, 1, 5));
	set_menu_format(item_menu, 1, 3);
	set_menu_mark(item_menu, " -> ");
	set_menu_fore(item_menu,COLOR_PAIR(6) | A_REVERSE );
	set_menu_back(item_menu,COLOR_PAIR(1) );
	//set_menu_grey(item_menu,COLOR_PAIR(2));
	post_menu(item_menu);
	wrefresh(menu_win);
	
	attron( COLOR_PAIR(5) | A_STANDOUT);
	mvprintw( LINES - 2, 0, "Press <ENTER> to select an option");
	mvprintw( LINES - 1, 0, "Use Arrow Keys to navigate ");
	attroff(COLOR_PAIR(5) | A_STANDOUT);
	refresh();
	
	while((ch = wgetch(menu_win))) {
	        switch(ch) {
	  		case KEY_LEFT:
				menu_driver(item_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(item_menu, REQ_RIGHT_ITEM);
				break;
			case 10:				 
				pos_menu_cursor(item_menu);
				
				if(strcmp(item_name(current_item(item_menu)),"New item") == 0 ) 
					newitem(NEW);
				
				else if(strcmp(item_name(current_item(item_menu)),"Existing item") == 0 ) 
					item_modify();
					
				else if(strcmp(item_name(current_item(item_menu)),"Exit") == 0 ) {
					unpost_menu(item_menu);
        				free_menu(item_menu);
       					for(i = 0; i < n_choices; ++i)
              					free_item(my_items[i]);
              				wbkgd(menu_win,COLOR_PAIR(1));
              				wclear(menu_win);
              				wrefresh(menu_win);	
					delwin(menu_win);
					
					return ;
				}
				break;
		}
                wrefresh(menu_win);
	}	
	return;
}
/*Item Master Functions */
void newitem(int mode) {
	FIELD *field[ITEM_FIELDS];
	FORM  *item_form;
	FILE *fd, *fdr;
	WINDOW *item_win;
	int ch, rows, cols, i, s = -1 ,nitems=0, count;
	char *number, *name,*brand,*location,*quantity,*rate ,*p, *q, *r, *t ; 
	char *item_title[] = {"Item no.:","Item name:","Location:","Brand:","Quantity:","Rate (in Rs) :"};
	char *value[7],*str,line[200],snum[30];
	
	init_screen();		// Initialize screen 	
	init_colorpair();		// Initialize colors
	
	item_win = newwin(40, 164, 15, 10);
	
	keypad(item_win, TRUE);
	/* Initialize the fields */
	for(i = 0; i < ITEM_FIELDS - 2; i++)
		field[i] = new_field(1, WIDTH, STARTY + i * 3, STARTX, 0, 0);
	field[ITEM_FIELDS - 2] = new_field(1, 10, STARTY + i * 3, STARTX + 10, 0, 0); // Button
	field[ITEM_FIELDS - 1] = NULL;
	
	/* Set field options */
	for(i = 0; i < ITEM_FIELDS - 2; i++) {
		set_field_fore(field[i], COLOR_PAIR(2));/* Put the field with blue background */
		set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
		field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this is filled up */
	}
	/* Setting the attributes of the button. */				      
	set_field_fore(field[ ITEM_FIELDS - 2], COLOR_PAIR(3));		// Button
	set_field_back(field[ ITEM_FIELDS - 2], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[ ITEM_FIELDS - 2], O_AUTOSKIP);
	field_opts_off(field[ ITEM_FIELDS - 2], O_EDIT);
	set_field_buffer(field[ITEM_FIELDS - 2], 0, "  ADD");/* Initialize the field */
	
	if(mode == MODIFY) {
		field_opts_off(field[0], O_EDIT); // Can't edit item number 
		field_opts_off(field[1], O_EDIT); // Can't edit item name
		field_opts_off(field[3], O_EDIT); // Can't edit brand
	}	
	/* Create the form and post it */
	item_form = new_form(field);
	
	/* Calculate the area required for the form */
	scale_form(item_form, &rows, &cols);
       
        //wbkgd(item_win, COLOR_PAIR(8));
	
	/* Set main window and sub window */
        set_form_win(item_form, item_win);
        set_form_sub(item_form, derwin(item_win, rows, cols, 2, 2));
	
	/* Print a border around the main window and print a title */
        box(item_win, 0, 0);
	print_in_middle(item_win, 1, 14, cols + 4, "ITEM DESCRIPTION", COLOR_PAIR(1));
	
	post_form(item_form);
	wrefresh(item_win);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );						// Instructions-item
	mvprintw(LINES -2 , 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1 , 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	refresh();
	
	/* Printing the labels of the fields */
	for(i = 0; i < ITEM_FIELDS - 2; i++) {
		wattron(item_win , COLOR_PAIR(1) | A_BOLD);
		mvwprintw(item_win, STARTY + 2 + (i * 3), STARTX - 20 ,"%s", item_title[i]);
		wattroff(item_win , COLOR_PAIR(1) | A_BOLD);
	}	
	
	locations(item_win);	// Printing the location box
	if(mode == MODIFY) {	/*Setting up buffers with the global strings containing item information.*/
		for(i = 0; i < ITEM_FIELDS - 2; i++) {
			set_field_buffer(field[i], 0,set_value[i] );
		}
	}
	if(mode == NEW) {		//Initializing item number 
		fd=fopen("item.csv","r");
		if(fd==NULL)
			nitems=0;
		while(!feof(fd)) {	
			readline(fd, line, 200);
			nitems++;	
		}
		str=inttostring(snum,nitems);
		set_field_buffer(field[0], 0, str);
		fclose(fd);
	}	
	/* Loop through to get user requests */
	while((ch = wgetch(item_win)) !=27 )
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(item_form, REQ_NEXT_FIELD);/* Go to the end of the present buffer */
				form_driver(item_form, REQ_END_LINE);/* Leaves nicely at the last character */
				break;
			case KEY_UP:
				form_driver(item_form, REQ_PREV_FIELD);/* Go to previous field */
				form_driver(item_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(item_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(item_form, REQ_RIGHT_CHAR);
				break;
			default:
				if(s!=-1) {
					wmove(item_win,STARTY + (s * 3) + 3 , STARTX + 2);
					wclrtoeol(item_win);
				}
				form_driver(item_form, ch);/* If this is a normal character, it gets printed*/
				break;
			case KEY_BACKSPACE :
				form_driver(item_form,REQ_DEL_PREV);
				break;
				
			case 10 :	
			if(current_field(item_form)==field[ITEM_FIELDS - 2] ) {
				for(i = 0; i < ITEM_FIELDS - 2; i++) {
					value[i]=field_buffer(field[i], 0);
				}
				if(mode == NEW)		// Item store function is called
					s=item_store(value);
				else {			// File is updated with current fields.
				/*Modifying Quantity of current item in item.csv file */
				fdr=fopen("item.csv","r+");
				fseek(fdr,0,SEEK_SET);	
				while(!feof(fdr)) {
					count = readline(fdr, line, 200);
					if(count == 0)
						break;
					number = strtok(line, ",");		
					name = strtok(NULL, ",");		
					location = strtok(NULL, ",");
					brand = strtok(NULL, ",");
					quantity = strtok(NULL, ",");
					rate = strtok(NULL, ",");
					
					if(!number || !name || !location||!brand || !quantity || !rate)
						break;
					r=trim(name);	
					t=trim(value[1]);
					p=trim(brand);	
					q=trim(value[2]);
					
					if(atoi(number)==atoi(trim(value[0])) && ((strcasecmp(r,t))==0) &&((strcasecmp(p,q))==0) ) {
						fseek(fdr,-(count+1) ,SEEK_CUR);
				fprintf(fdr,"%s,%s,%s,%s,%d,%lf",number,name,trim(value[2]),
								brand,atoi(trim(value[4])),atof(trim(value[5])));
						s=-1;
						fclose(fdr);		
						break;		
					}				
				}
				}	
				if( s == -1) {
					wattron(item_win , COLOR_PAIR(1));
					mvwprintw(item_win, STARTY + 22, STARTX, "Data saved successfully.");	
					wattroff(item_win , COLOR_PAIR(1));	
					for(i = 0; i < ITEM_FIELDS - 2; i++) {
						form_driver(item_form,REQ_NEXT_FIELD);
						form_driver(item_form,REQ_END_LINE);
						form_driver(item_form,REQ_CLR_FIELD);
					}
					if(mode == NEW) {
						nitems++;	// Updating the item no. and its field
						str=inttostring(snum,nitems);
						set_field_buffer(field[0], 0, str);
					}	
				}
				else {	
					wattron(item_win , COLOR_PAIR(6) | A_BOLD);			
					mvwprintw(item_win,STARTY + (s * 3) + 3 , STARTX + 2, "Error in the above field");
					wattroff(item_win , COLOR_PAIR(6) | A_BOLD);
				}		
			}
			break;	
		}
	}

	/* Unpost form and free memory */
	unpost_form(item_form);
	free_form(item_form);
	for(i = 0; i < ITEM_FIELDS - 1; i++)
		free_field(field[i]);
	
	wmove(item_win,0, 0); 
	wclear(item_win);
	wrefresh(item_win);
	delwin(item_win);
	return;
}
void locations(WINDOW *w) {	// Printing the possible storage locations on the screen
	int i=0;
	char *location_title[] = {"Shelf","Racks","Fridge/Refrigerator","Sections:Cereal,pulses,vegetable",
								"Sections:Fruits,daily usage,cosmetics"};
	
	mvwvline(w,1, 75,  ACS_VLINE, 38);
	mvwhline(w,15, 76,  ACS_HLINE, 87);
	print_in_middle(w, 1, 75, 90, "LOCATIONS", COLOR_PAIR(1));	
	wattron(w,COLOR_PAIR(5) );
	
	for(i=0; i<5; i++)
		mvwprintw(w ,3 + (2*i), 100, "%d)%s", i+1, location_title[i]);
	
	wattroff(w,COLOR_PAIR(5));
	return;
}
int item_store(char **str) { 		//returns -1 if successful else "i"->field number
	int i=0 , j, len ;
	FILE *fd;
	item *q;
	q=(item *)malloc(sizeof(item));
	
	while(i < 6) {			// Validation
	
		if( i==0 || i== 4 || i== 5 ) {
			len = strlen(str[i]);
    			for (j=0;j<len; j++) {	//In the above field,only digit or space or dot is allowed
        			if (!((isdigit(str[i][j])) || (str[i][j])=='.' || (str[i][j])==32))
        				return i;	
        		}	
        	}
        	i++;			
	}
	// Writing to file if all fields properly validated
	
	fd=fopen("item.csv","a");
		
	q->number=atoi(str[0]);
		q->name=(char*)malloc(sizeof(char)* 30);
	strcpy(q->name ,str[1]);
		q->location=(char*)malloc(sizeof(char)* 30);
	strcpy(q->location,str[2]);
		q->brand=(char*)malloc(sizeof(char)* 30);
	strcpy(q->brand,str[3]);
	q->quantity=atoi(str[4]);
	q->rate=atof(str[5]);
				/*Making a csv file */		
	fprintf(fd , "%d,%s,%s,%s,%d,%lf\n", q->number, trim(q->name), trim(q->location),trim(q->brand),q->quantity,q->rate);
	
	count_item++; 
	free(q->name);
	free(q->location);
	free(q->brand);
	free(q);
	fclose(fd);
	return -1;
}
/*Function will display modify info of item and will further call newitem in modify mode.*/ 
void item_modify() {
	FIELD *field[6];
	FORM  *modify_form;
	WINDOW *modify_win;
	int ch, rows, cols, i;
	char *value[4];
	
	init_screen();			
	init_colorpair();		
	
	modify_win = newwin(40, 164, 15, 10);
	
	keypad(modify_win, TRUE);

	field[0] = new_field(1, WIDTH, 5, STARTX1, 0, 0);	//Number
	field[1] = new_field(1, 5, 7, STARTX1 + 10, 0, 0);
	field[2] = new_field(1, WIDTH, 9, STARTX1, 0, 0);	//Name
	field[3] = new_field(1, WIDTH, 11, STARTX1, 0, 0);	//Brand
	field[4] = new_field(1, 10, 13, STARTX1 + 8, 0, 0); 
	field[5] = NULL;
	
	for(i = 0; i < 5; i++) {
		if(i==0 || i==2 || i==3) {	/*Setting attributes of text fields*/
			set_field_fore(field[i], COLOR_PAIR(2));
			set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
			field_opts_off(field[i], O_AUTOSKIP); 
		}
		else if(i==1 || i==4)	{	/*Setting attributes of buttons*/      
	set_field_fore(field[i], COLOR_PAIR(3));		
	set_field_back(field[i], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[i], O_AUTOSKIP);
	field_opts_off(field[i], O_EDIT);
		}
	}
	set_field_buffer(field[1], 0, "  AND");	
	set_field_buffer(field[4], 0, "  MODIFY");
	
	modify_form = new_form(field);
	scale_form(modify_form, &rows, &cols);
	
        set_form_win(modify_form, modify_win);
        set_form_sub(modify_form, derwin(modify_win, rows, cols,2, 2));
	
        box(modify_win, 0, 0);
	print_in_middle(modify_win, 1, 75, 90, "ENTER DETAILS", COLOR_PAIR(1));
	
	post_form(modify_form);
	wrefresh(modify_win);
	
	wattron(modify_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(modify_win, 7  , STARTX1 - 20 ,"Item number:");
	mvwprintw(modify_win, 11, STARTX1 - 20 ,"Item name:");
	mvwprintw(modify_win, 13, STARTX1 - 20 ,"Brand:");
	wattroff(modify_win , COLOR_PAIR(1) | A_BOLD);
	
	mvwvline(modify_win,1, 75,  ACS_VLINE, 38);
	mvwhline(modify_win,18, 76,  ACS_HLINE, 87);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );	
	mvprintw(LINES -3 , 0, "Find item no. from inventory search");					
	mvprintw(LINES -2 , 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1 , 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	refresh();

	while((ch = wgetch(modify_win)) !=27 ) {
		switch(ch)
		{	case KEY_DOWN:
				form_driver(modify_form, REQ_NEXT_FIELD);
				form_driver(modify_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(modify_form, REQ_PREV_FIELD);
				form_driver(modify_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(modify_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(modify_form, REQ_RIGHT_CHAR);
				break;
			default:
				form_driver(modify_form, ch);
				break;
			case KEY_BACKSPACE :
				form_driver(modify_form,REQ_DEL_PREV);
				break;
			case 10 :	
			if(current_field(modify_form)==field[4]) {
				for(i = 0; i < 4; i++) {
					value[i]=field_buffer(field[i], 0);
				}
				modify_info(value);		//Don't call these functions if buffer is empty
				newitem(MODIFY);
				for(i = 0; i < 4; i++) {
					form_driver(modify_form,REQ_NEXT_FIELD);
					form_driver(modify_form,REQ_END_LINE);
					form_driver(modify_form,REQ_CLR_FIELD);
				}
				unpost_form(modify_form);
				free_form(modify_form);
				for(i = 0; i < 5; i++)
					free_field(field[i]);
				wmove(modify_win,0, 0); 
				wclear(modify_win);
				wrefresh(modify_win);
				delwin(modify_win);
				return;
			}
			break;	
		}
	}
	unpost_form(modify_form);
	free_form(modify_form);
	for(i = 0; i < 5; i++)
		free_field(field[i]);
	wmove(modify_win,0, 0); 
	wclear(modify_win);
	wrefresh(modify_win);
	delwin(modify_win);
	return;		
}
/*It will set the globals set_value[i] with the item info. */
void modify_info(char **str) {
	int num;
	char *nam , *bran;
	FILE *fd;
	int count;
	char *number, *name,*location,*brand,*quantity,*rate; 
	char line[200];
	
	fd=fopen("item.csv","r");
	if(fd==NULL)
		return ;
	
	while(!feof(fd)) {	
			count = readline(fd, line, 200);
			if (count==0)
				break;	
			number = strtok(line, ",");		
			name = strtok(NULL, ",");		
			location = strtok(NULL, ",");
			brand = strtok(NULL, ",");
			quantity = strtok(NULL, ",");
			rate = strtok(NULL, ",");
		
			num=atoi(str[0]); //item number
			nam=str[2]; //item name
			bran=str[3]; //brand		
					
			if((num==atoi(number))|| (((strcasecmp(nam,name)) == 0 ) && ((strcasecmp(bran,brand)) == 0 )) ) {
      				set_value[0]=number; 
      				set_value[1]=name; 
      				set_value[2]=location;
      				set_value[3]=brand;
      				set_value[4]=quantity;
      				set_value[5]=rate;
				break;										
			}
	}		
	fclose(fd);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Inventory Display*/
void inventory_display() {
	WINDOW  *menu_win;
	MENU *invent_menu;
	ITEM **my_items;
	int i, n_choices, ch;
	char *choices[] = {"Display items", "Search", "Sort" , "Exit"};
	
	init_screen();
	init_colorpair();
	menu_win =newwin(3, 85, 11, 40);	// Menu Window
	
	keypad(menu_win,TRUE);
	box(menu_win, 0 , 0);	
	wbkgd(menu_win, COLOR_PAIR(2));
	wrefresh(menu_win);
	
	n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]);
	my_items[i]=(ITEM *)NULL;	
	
	invent_menu = new_menu((ITEM **)my_items);
	menu_opts_off(invent_menu, O_SHOWDESC);
        set_menu_win(invent_menu, menu_win);
        set_menu_sub(invent_menu, derwin(menu_win, 1, 70, 1, 5));
	set_menu_format(invent_menu, 1, 4);
	set_menu_mark(invent_menu, " -> ");
	set_menu_fore(invent_menu,COLOR_PAIR(6) | A_REVERSE );
	set_menu_back(invent_menu,COLOR_PAIR(1) );
	//set_menu_grey(invent_menu,COLOR_PAIR(2));
	post_menu(invent_menu);
	wrefresh(menu_win);
	attron( COLOR_PAIR(5) | A_STANDOUT);
	mvprintw( LINES - 2, 0, "Press <ENTER> to select an option");
	mvprintw( LINES - 1, 0, "Use Arrow Keys to navigate ");
	attroff(COLOR_PAIR(5) | A_STANDOUT);
	refresh();
	
	while((ch = wgetch(menu_win))) {
		
	        switch(ch) {
	  		case KEY_LEFT:
				menu_driver(invent_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(invent_menu, REQ_RIGHT_ITEM);
				break;
			case 10:				 
				pos_menu_cursor(invent_menu);
				for(i=0; i<2; i++) {
					move(LINES - i,0);
					clrtoeol();
				}
				if(strcmp(item_name(current_item(invent_menu)),"Display items") == 0 ) 
					item_display(DISPLAY);
				
				else if(strcmp(item_name(current_item(invent_menu)),"Search") == 0 ) 
					item_search();
				
				else if(strcmp(item_name(current_item(invent_menu)),"Sort") == 0 ) 
					item_display(SORT);
					
				else if(strcmp(item_name(current_item(invent_menu)),"Exit") == 0 ) {
					unpost_menu(invent_menu);
        				free_menu(invent_menu);
       					for(i = 0; i < n_choices; ++i)
              					free_item(my_items[i]);
              				wbkgd(menu_win,COLOR_PAIR(1));
              				wclear(menu_win);
              				wrefresh(menu_win);	
					delwin(menu_win);
					return ;
				}
				break;
		}
                wrefresh(menu_win);
	}	
}
/*Inventory Functions*/
void item_display(int mode) {
	WINDOW *display_win;
	int count, i=0, j=0,k=0, ch, start=0, height=30;
	char *title[] = {"Item no.","Item name","Location","Brand","Quantity","Rate (in Rs) "};
	char line[200];
	char *number,*name,*location,*brand,*quantity,*rate;   
	FILE *fd;
	item *q[300], *temp;
	
	for(i=0;i<3;i++) {		//Clearing background screen first
		move(LINES-i,0);
		clrtoeol();	
		refresh();
	}	
	/*Display part*/
	display_win=newwin(40, 164, 15, 10);
	box(display_win, 0 , 0);	
	wrefresh(display_win);
	wattron(display_win,COLOR_PAIR(1) | A_BOLD );
	mvwprintw(display_win,2,25,"%-10s %-30s %-30s %-15s %-10s %-15s",title[0],title[1],title[2],title[3],title[4],title[5]);
	wattroff(display_win,COLOR_PAIR(1) | A_BOLD );
	mvwhline(display_win,3, 0,  ACS_HLINE, 164);
	mvwhline(display_win,35, 0,  ACS_HLINE, 164);
	keypad(display_win,TRUE);
	
	wattron(display_win,COLOR_PAIR(5) | A_STANDOUT);
	mvwprintw(display_win,37, 1, "Press PAGE UP AND PAGE DOWN TO SCROLL");
	mvwprintw(display_win,38, 1, "Press ESC to exit to menu bar ");
	wattroff(display_win,COLOR_PAIR(5) | A_STANDOUT);
		
	/*Reading part*/
	fd=fopen("item.csv","r");	/*reading from a csv file */
	if(fd==NULL)
		return;
	box(display_win, 0 , 0);	//making borders again
	// Reading csv file and tokenizing the lines of files.
	while(!feof(fd)) {
		count = readline(fd, line, 200);
		//printf("%s \n", line);		// "line" is the character array which stores the particular line.
		if(count == 0)
			break;
		number = strtok(line, ",");		
		name = strtok(NULL, ",");		
		location = strtok(NULL, ",");
		brand = strtok(NULL, ",");
		quantity = strtok(NULL, ",");
		rate = strtok(NULL, ",");
		q[j]=(item *)malloc(sizeof(item));
		q[j]->name=(char*)malloc(sizeof(char)* 30);
		q[j]->location=(char*)malloc(sizeof(char)* 30);
		q[j]->brand=(char*)malloc(sizeof(char)* 30);
		q[j]->number=atoi(number);
		strcpy(q[j]->name,name);
		strcpy(q[j]->location,location);
		strcpy(q[j]->brand,brand);
		q[j]->quantity=atoi(quantity);
		q[j]->rate=atof(rate);
		j++;
		
	}
	if(mode == SORT) {	// Sort based on item name
		for(i=0; i<j; i++)
      			for(k=i+1; k<j; k++){
        			 if((strcmp(q[i]->name,q[k]->name))>0){
           				 temp=q[i];
          			         q[i]=q[k];
                                         q[k]=temp;
         			}
     			 }
	
	}
	wattron(display_win,COLOR_PAIR(5) | A_BOLD | A_STANDOUT);
	mvwprintw(display_win,20, 72, "Press ENTER key to continue.");
	mvwprintw(display_win,36, 120, "Total number of items :%d ",j);
	wattroff(display_win,COLOR_PAIR(5)| A_BOLD | A_STANDOUT);
		
	while((ch = wgetch(display_win)) != 27){	// Scrolling in inventory display		
		switch(ch) {
			case KEY_DOWN:
				if((start+height) < j)
					start++;
				break;
			case KEY_UP:
				if(start > 0)
					start--;
				break;
				
		}
		for(i=start; i<(start+height) && i < j; i++) {	//j->item count
		wattron(display_win,COLOR_PAIR(5));
		mvwprintw(display_win,4 + (i-start),25 ,"%-10d %-30s %-30s %-15s %-10d %-12.2lf\n", q[i]->number, q[i]->name,
		 							q[i]->location ,q[i]->brand,q[i]->quantity,q[i]->rate);
		wattroff(display_win,COLOR_PAIR(5));
		box(display_win, 0 , 0);
		wrefresh(display_win);
		}		
	}
	for(i=0;i<j;i++) {
		free(q[i]->name);
		free(q[i]->location);
		free(q[i]->brand);
		free(q[i]);
	}
	fclose(fd);
	wclear(display_win);
	wrefresh(display_win);
	delwin(display_win);
	return;
}
void item_search() {		// Includes Graphical as well as data processing part
	WINDOW *search_win;
	FIELD *field[4];
	FORM  *search_form;
	int ch, rows, cols, i;
	char *value[3], *s , *t , *r;
	FILE *fd;
	int count;
	char *number,*name,*location,*brand,*quantity,*rate; 
	char line[200];
	
	fd=fopen("item.csv","r");

	init_screen();		// Initialize screen 	
	init_colorpair();		// Initialize colors

	/* Initialize the fields */
	field[0] = new_field(1, 15, 3, STARTX, 0, 0);
	field[1] = new_field(1, 10, 3 , STARTX + 16, 0, 0);	// Enter button 
	field[2] = new_field(1, 15, 3 + 3, STARTX, 0, 0);
	field[3] = NULL;

	for(i = 0; i < 3;i++) {
		if(i!=1) {
			set_field_fore(field[i], COLOR_PAIR(2));/* Put the field with blue background */
			set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
			field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this is filled up */
		}	
	}
	set_field_fore(field[1], COLOR_PAIR(3));		// Button for ENTER
	set_field_back(field[1], COLOR_PAIR(3) | A_BOLD);
	set_field_buffer(field[1],0,"ENTER");
	field_opts_off(field[1], O_AUTOSKIP);
	field_opts_off(field[1], O_EDIT);
	
	/* Creating window + posting form*/
	search_form = new_form(field);
	scale_form(search_form, &rows, &cols);
	search_win = newwin(40, 164, 15, 10);
        keypad(search_win, TRUE);
	set_form_win(search_form, search_win);
        set_form_sub(search_form, derwin(search_win, rows, cols, 2, 2));
	box(search_win, 0, 0);
	print_in_middle(search_win, 1, 0, 75, "ENTER DETAILS", COLOR_PAIR(1));
	post_form(search_form);
	wrefresh(search_win);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );						// Instructions-tax
	mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields/buttons");
	mvprintw(LINES -1, 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	
	print_in_middle(search_win, 1, 75, 90, "SEARCH CRITERIA", COLOR_PAIR(1));	//standard tax box
	wattron(search_win , COLOR_PAIR(5) | A_BOLD );
	
	mvwprintw(search_win , 3, 110, "1) number ");
	mvwprintw(search_win , 5, 110, "2) name");
	mvwprintw(search_win , 7, 110, "3) location ");
	mvwprintw(search_win , 9, 110, "4) brand ");
	wattroff(search_win , COLOR_PAIR(5) | A_BOLD);
	
	mvwhline(search_win,11, 1,  ACS_HLINE, 163);
	mvwvline(search_win,1, 82,  ACS_VLINE, 10);			
	
	/* Printing the labels of the fields */
	wattron(search_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(search_win, 3 + 2, STARTX - 20 , "Search criteria :");	
	mvwprintw(search_win, 3 + 5, STARTX - 20 , "Enter value :");
	wattroff(search_win , COLOR_PAIR(1) | A_BOLD);
	/* Loop through to get user requests */
	while((ch = wgetch(search_win)) != 27) {
		switch(ch) {
			case KEY_DOWN:
				form_driver(search_form, REQ_NEXT_FIELD);
				form_driver(search_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(search_form, REQ_PREV_FIELD);
				form_driver(search_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(search_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(search_form, REQ_RIGHT_CHAR);
				break;	
			default:
				form_driver(search_form, ch);
				
				/* The search will refresh results on screen after change of one character.*/
				if(current_field(search_form)==field[2]) {
				form_driver(search_form, REQ_END_LINE);
				for(i=0;i<39;i++) {
					wmove(search_win,14+i,10);
					wclrtoeol(search_win);
				}
				box(search_win, 0, 0);
				i=0;
				fseek(fd,0,SEEK_SET);
				while(!feof(fd)) {
					print_in_middle(search_win, 12, 0, 164, "SEARCH RESULTS", COLOR_PAIR(1));
					count = readline(fd, line, 200);
					//printf("%s \n", line);
					if(count == 0)
						break;
					number = strtok(line, ",");		
					name = strtok(NULL, ",");		
					location = strtok(NULL, ",");
					brand = strtok(NULL, ",");
					quantity = strtok(NULL, ",");
					rate = strtok(NULL, ",");
					value[0]=field_buffer(field[0], 0);	// search criteria name
					value[1]=field_buffer(field[2], 0);	// string to search
					s=trim(value[1]);
					t=trim(value[0]);
					/* Search based on specified criteria given by user */
					if((strcmp(t,"name"))==0)
						r=name;
					else if((strcmp(t,"number"))==0)
						r=number;
					else if((strcmp(t,"location"))==0)
						r=location;
					else if((strcmp(t,"brand"))==0)
						r=brand;			
					/* If serch criteria satisfied , print on the screen */
					if((strncasecmp(r,s,strlen(s))) == 0) {
					wattron(search_win,COLOR_PAIR(5));
      mvwprintw(search_win,14+i,10,"%-10d %-30s %-30s %-20s %-8d %-8.2lf", atoi(number), name, location,brand,atoi(quantity),atof(rate));
					wattroff(search_win,COLOR_PAIR(5));	
						i++;								
					}
					else 
						continue;
				}
				}
				break;
			case KEY_BACKSPACE :
				form_driver(search_form,REQ_DEL_PREV);
				break;
			case 10 :
				if(current_field(search_form)==field[1] ) {
					value[0]=field_buffer(field[0], 0);	// search criteria name
					
				}
		}
	}
	unpost_form(search_form);
	free_form(search_form);
	for(i = 0; i < 3; i++)
		free_field(field[i]); 
	wmove(search_win,0, 0); 
	wclear(search_win);
	wrefresh(search_win);
	delwin(search_win);	
	return;	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Tax Master display*/
void tax_menu() {
	FIELD *field[6];
	FORM  *tax_form;
	FILE *td;
	WINDOW *tax_win;
	int ch, rows, cols, i, t;
	char *tvalue[2];
	
	init_screen();		// Initialize screen 	
	init_colorpair();		// Initialize colors

	/* Initialize the fields */
	field[0] = new_field(1, WIDTH, STARTY, STARTX, 0, 0);
	field[1] = new_field(1, WIDTH, STARTY + 3, STARTX, 0, 0);
	field[2] = new_field(1, 10, STARTY + 5, STARTX + 7, 0, 0); // Button for "SAVE"
	field[3] = new_field(1, 25, STARTY + 7, STARTX , 0, 0); // Button for "INCLUDE STANDARD TAXES"
	field[4] = new_field(1, 25, STARTY + 9, STARTX , 0, 0); // Button for "VIEW CURRENT TAXES"
	field[5] = NULL;

	for(i = 0; i < 5;i++) {
		if(i==0 || i==1) {
		
	set_field_fore(field[i], COLOR_PAIR(2));/* Put the field with blue background */
	set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
	field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this is filled up */
		}
		else if (i==2 || i==3 || i==4 ) {     
	set_field_fore(field[i], COLOR_PAIR(3));		// Button
	set_field_back(field[i], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[i], O_AUTOSKIP);
	field_opts_off(field[i], O_EDIT);
		}
	}
	
	set_field_buffer(field[2], 0, "  SAVE");	/* Initialize the field */
	set_field_buffer(field[3], 0, "  INCLUDE STANDARD TAXES");
	set_field_buffer(field[4], 0, "    VIEW CURRENT TAXES");
	
	/* Creating window + posting form*/
	tax_form = new_form(field);
	scale_form(tax_form, &rows, &cols);
	tax_win = newwin(40, 164, 15, 10);
        keypad(tax_win, TRUE);
	set_form_win(tax_form, tax_win);
        set_form_sub(tax_form, derwin(tax_win, rows, cols, 2, 2));
	box(tax_win, 0, 0);
	print_in_middle(tax_win, 1, 0, 75, "TAX DESCRIPTION", COLOR_PAIR(1));
	post_form(tax_form);
	wrefresh(tax_win);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );						// Instructions-tax
	mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields/buttons");
	mvprintw(LINES -1, 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	
	print_in_middle(tax_win, 1, 75, 90, "STANDARD TAXES", COLOR_PAIR(1));	//standard tax box
	wattron(tax_win , COLOR_PAIR(5) | A_BOLD );
	mvwprintw(tax_win , 3, 110, "1) Service Tax : 14.2 %%");
	mvwprintw(tax_win , 5, 110, "2) VAT         : 2    %%");
	mvwprintw(tax_win , 7, 110, "3) Other taxes : 0.5  %%");
	wattroff(tax_win , COLOR_PAIR(5) | A_BOLD);
	
	mvwvline(tax_win,1, 75,  ACS_VLINE, 38);
	mvwhline(tax_win,11, 76,  ACS_HLINE, 87);			
	
	/* Printing the labels of the fields */
	wattron(tax_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(tax_win, STARTY + 2, STARTX - 20 , "Tax Name	      :");	// Should handle spaces also
	mvwprintw(tax_win, STARTY + 5, STARTX - 20 , "Tax Rate (in %) :");
	wattroff(tax_win , COLOR_PAIR(1) | A_BOLD);
	/* Loop through to get user requests */
	while((ch = wgetch(tax_win)) != 27)
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(tax_form, REQ_NEXT_FIELD);
				form_driver(tax_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(tax_form, REQ_PREV_FIELD);
				form_driver(tax_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(tax_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(tax_form, REQ_RIGHT_CHAR);
				break;	
			default:
				form_driver(tax_form, ch);
				break;
			case KEY_BACKSPACE :
				form_driver(tax_form,REQ_DEL_PREV);
				break;
			case 10 :
			wmove(tax_win, 17,STARTX);
			wclrtoeol(tax_win);	
			if(current_field(tax_form)==field[2] ) {
					tvalue[0]=field_buffer(field[0], 0);
					tvalue[1]=field_buffer(field[1], 0);
					t=tax_store(tvalue);
					if (t == 0) {
						wattron(tax_win , COLOR_PAIR(1));
						wmove(tax_win ,STARTY + 6, STARTX + 2 );	//Clear the error message	
						wclrtoeol(tax_win );
						mvwprintw(tax_win , 17, STARTX, "Data saved successfully.");
						wattroff(tax_win  , COLOR_PAIR(1));
					}
					else {
						wattron(tax_win  , COLOR_PAIR(6));
						mvwprintw(tax_win ,STARTY + 6, STARTX + 2, "Error in the above field");
						wattroff(tax_win  , COLOR_PAIR(6));	
					}	
			}
			else if(current_field(tax_form)==field[3] ) {
					// Add standard taxes to next '3' structures here
					tax_rate=tax_rate + 16.7;
					count_tax++;
					td=fopen("tax.txt","a");
					fprintf(td ,"Standard tax,16.200000\n");
					fclose(td);
					wattron(tax_win , COLOR_PAIR(1));
					mvwprintw(tax_win, 17, STARTX, "Standard taxes successfully included.");	
					wattroff(tax_win, COLOR_PAIR(1));		
			}
			else if(current_field(tax_form)==field[4] ) {
					// Printing all current taxes in the bill
					print_in_middle(tax_win, 12, 75, 90, "CURRENT TAXES",COLOR_PAIR(1) );	
					tax_print(tax_win);
			}
			break;	
		}
	}
	unpost_form(tax_form);
	free_form(tax_form);
	for(i = 0; i < 5; i++)
		free_field(field[i]); 
	wmove(tax_win,0, 0); 
	wclear(tax_win);
	wrefresh(tax_win);
	delwin(tax_win);	
	return;
}
/*Tax Master Functions */
int tax_store(char **str) {		// returns 0 if successful else 1
	int i=0 , j ;
	FILE *td;
	tax *t;
	t=(tax *)malloc(sizeof(tax));
	
	while(i<2){
		if( i==1 ) {				// validating the value field of tax
    			for (j=0;j<strlen(str[1]); j++) {
        			if (!((isdigit(str[i][j])) || (str[i][j])=='.' || (str[i][j])==32))
        				return 1 ;	
        		}	
        	}
        	i++;
        }	
	// Writing to files if all fields properly validated
	td=fopen("tax.txt","a");
	
	t->name=(char*)malloc(sizeof(char)* 30);
	t->name=  str[0];
	t->value=atof(str[1]);
	
	fprintf(td , "%s,%lf\n", trim(t->name), t->value);
	
	free(t->name);
	free(t);
	fclose(td);
	return 0;
}
/* It will print all the current taxes on the screen */ 
void tax_print(WINDOW *w) {
	int i=1,count;
	FILE *td;
	char line[50], *name, *value;
	tax *t;
	t=(tax *)malloc(sizeof(tax));
	t->name=(char*)malloc(sizeof(char)* 30);
	mvwhline(w,37, 76,  ACS_HLINE, 86);
	mvwvline(w,1, 75,  ACS_VLINE, 38);	box(w,0,0);	//Reprinting
	td=fopen("tax.txt","r");
	
	count_tax=0;
	tax_rate=0;
	while(!feof(td)) {
		count = readline(td, line, 50);
		if(count == 0)
			break;
		name = strtok(line, ",");		
		value = strtok(NULL, ",");		
		strcpy(t->name,name);
		t->value=atof(value);
		wattron(w , COLOR_PAIR(5) | A_BOLD );
		mvwprintw(w ,12 + (2 * i), 110 , "%d)%-20s:%-5.2lf",i, t->name, t->value);
		wattroff(w , COLOR_PAIR(5) | A_BOLD);
		tax_rate = tax_rate + t->value;
		count_tax++;
		i++;
	}
	wattron(w , COLOR_PAIR(1) | A_BOLD |A_STANDOUT);
	mvwprintw(w ,38, 120 , "Current total tax rate:%-5.2lf %%",tax_rate);
	mvwprintw(w ,38, 80 , "Total number of taxes :%d",count_tax);
	wattroff(w , COLOR_PAIR(1) | A_BOLD |A_STANDOUT);
	free(t->name);
	free(t);
	fclose(td);
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Bill Display*/
void bill_display() {
	WINDOW  *menu_win;
	MENU *bill_menu;
	ITEM **my_items;
	int i, n_choices, ch;
	char *choices[] = {"Add/Modify items", "Cart", "Profile","Checkout" , "Exit"};
	
	init_screen();
	init_colorpair();
	menu_win =newwin(3, 112, 11, 33);	// Menu Window
	
	keypad(menu_win,TRUE);
	box(menu_win, 0 , 0);	
	wbkgd(menu_win, COLOR_PAIR(2));
	wrefresh(menu_win);
	
	n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices + 1 , sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]);
	my_items[i]=(ITEM *)NULL;	
	
	bill_menu = new_menu((ITEM **)my_items);
	menu_opts_off(bill_menu, O_SHOWDESC);
        set_menu_win(bill_menu, menu_win);
        set_menu_sub(bill_menu, derwin(menu_win, 1, 100, 1, 5));
	set_menu_format(bill_menu, 1, 5);
	set_menu_mark(bill_menu, " -> ");
	set_menu_fore(bill_menu,COLOR_PAIR(6) | A_REVERSE );
	set_menu_back(bill_menu,COLOR_PAIR(1) );
	post_menu(bill_menu);
	wrefresh(menu_win);
	
	attron( COLOR_PAIR(5) | A_STANDOUT);
	mvprintw( LINES - 2, 0, "Press <ENTER> to select an option");
	mvprintw( LINES - 1, 0, "Use Arrow Keys to navigate ");
	attroff(COLOR_PAIR(5) | A_STANDOUT);
	refresh();
	
	while((ch = wgetch(menu_win))) {
	        switch(ch) {
	  		case KEY_LEFT:
				menu_driver(bill_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(bill_menu, REQ_RIGHT_ITEM);
				break;
			case 10:				 
				pos_menu_cursor(bill_menu);
				
				if(strcmp(item_name(current_item(bill_menu)),"Add/Modify items") == 0 ) 
					bill_additem();
				else if(strcmp(item_name(current_item(bill_menu)),"Cart") == 0 ) 
					bill_view();
				else if(strcmp(item_name(current_item(bill_menu)),"Profile") == 0 ) 
					info_form();	
				else if(strcmp(item_name(current_item(bill_menu)),"Checkout") == 0 ) 
					bill_attributes();
				else if(strcmp(item_name(current_item(bill_menu)),"Exit") == 0 ) {
					unpost_menu(bill_menu);
        				free_menu(bill_menu);
       					for(i = 0; i < n_choices; ++i)
              					free_item(my_items[i]);
              				wbkgd(menu_win,COLOR_PAIR(1));
              				wclear(menu_win);
              				wrefresh(menu_win);	
					delwin(menu_win);
					return ;
				}
				break;
		}
                wrefresh(menu_win);
	}	
}
/*Bill functions*/
void bill_additem() {
	FIELD *field[6];
	FORM  *bill_form;
	FILE *fdr, *fdw;
	WINDOW *bill_win;
	int ch, rows, cols, i, count, newquantity;
	char *number,*name,*location,*brand,*quantity,*rate, *p,*q,*r,*s; 
	char *value[4];
	char line[200];
	char message[]="Item out of stock";
	
	fdr=fopen("item.csv","r+");
	init_screen();			
	init_colorpair();		
	bill_win = newwin(40, 164, 15, 10);
	keypad(bill_win, TRUE);

	field[0] = new_field(1, WIDTH, STARTY2, STARTX + 40, 0, 0);	//Number
	field[1] = new_field(1, WIDTH, STARTY2 +2, STARTX + 40, 0, 0);	//Name
	field[2] = new_field(1, WIDTH, STARTY2 + 4, STARTX + 40, 0, 0);	//Brand
	field[3] = new_field(1, WIDTH, STARTY2 +6, STARTX + 40, 0, 0);	//Quantity
	field[4] = new_field(1, 13, STARTY2 + 8 , STARTX + 35, 0, 0); 	//Add to cart
	field[5] = NULL;
	
	for(i = 0; i < 5; i++) {
		if(i==0 || i==1 || i==2 || i==3) {
			set_field_fore(field[i], COLOR_PAIR(2));
			set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
			field_opts_off(field[i], O_AUTOSKIP); 
		}
		else 	{	      
			set_field_fore(field[i], COLOR_PAIR(3));		
			set_field_back(field[i], COLOR_PAIR(3) | A_BOLD);
			field_opts_off(field[i], O_AUTOSKIP);
			field_opts_off(field[i], O_EDIT);
		}
	}
	set_field_buffer(field[4], 0, "ADD TO CART");
	
	bill_form = new_form(field);
	scale_form(bill_form , &rows, &cols);
	
        set_form_win(bill_form , bill_win);
        set_form_sub(bill_form , derwin(bill_win, rows, cols,2, 2));
	
        box(bill_win, 0, 0);
        wattron(bill_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
	mvwprintw(bill_win, 1  , 65 ,"ENTER ITEM DETAILS ");
	wattroff(bill_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
	
	post_form(bill_form );
	wrefresh(bill_win);
	
	wattron(bill_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(bill_win, STARTY2 + 2, STARTX + 20 ,"Item number:");
	mvwprintw(bill_win, STARTY2 + 4, STARTX + 20 ,"Item name:");
	mvwprintw(bill_win, STARTY2 + 6, STARTX + 20 ,"Brand:");
	mvwprintw(bill_win, STARTY2 + 8, STARTX + 20 ,"Quantity:");
	wattroff(bill_win, COLOR_PAIR(1) | A_BOLD);
	
	//mvwvline(bill_win,1, 75,  ACS_VLINE, 38);
	mvwhline(bill_win,13,1,  ACS_HLINE, 162);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );	
	mvprintw(LINES -3 , 0, "Find item no. from inventory search");					
	mvprintw(LINES -2 , 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1 , 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	refresh();

	while((ch = wgetch(bill_win)) !=27 ) {
		switch(ch)
		{	case KEY_DOWN:
				form_driver(bill_form , REQ_NEXT_FIELD);
				form_driver(bill_form , REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(bill_form , REQ_PREV_FIELD);
				form_driver(bill_form , REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(bill_form , REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT: case 32:
				form_driver(bill_form , REQ_RIGHT_CHAR);
				break;
			default:
				form_driver(bill_form , ch);
				if(current_field(bill_form)==field[0] || current_field(bill_form)==field[1]) {
				form_driver(bill_form, REQ_END_LINE);
				print_in_middle(bill_win, 14, 0, 164, "SUGGESTIONS", COLOR_PAIR(1));
				wattron(bill_win , COLOR_PAIR(1) | A_BOLD );
				mvwprintw(bill_win, 15, 96 ,"Avl. Quantity");
				wattroff(bill_win , COLOR_PAIR(1) | A_BOLD );
				for(i=0;i<39;i++) {
					wmove(bill_win,16+i,10);
					wclrtoeol(bill_win);
				}
				box(bill_win, 0, 0);
				i=0;
				fseek(fdr,0,SEEK_SET);
				while(!feof(fdr)) {
					count = readline(fdr, line, 200);
					if(count == 0)
						break;
					number = strtok(line, ",");		
					name = strtok(NULL, ",");		
					location = strtok(NULL, ",");
					brand = strtok(NULL, ",");
					quantity = strtok(NULL, ",");
					rate = strtok(NULL, ",");
					
					if(current_field(bill_form)==field[0]) {
						r=number;	
						s=trim(field_buffer(field[0], 0));
					}
					else {
						r=name;	
						s=trim(field_buffer(field[1], 0));
					}
					/*If criteria is satisfied and stock of item is present.*/
					if((strncasecmp(r,s,strlen(s))) == 0 && atoi(quantity)!=0) {
					wattron(bill_win,COLOR_PAIR(5));
      mvwprintw(bill_win,16+i,40,"%-10d %-30s %-20s %-8d %-8lf", atoi(number), name, brand,atoi(quantity),atof(rate));
					wattroff(bill_win,COLOR_PAIR(5));	
						i++;								
					}
					/*If criteria is satisfied and item isout of stock.*/
					else if((strncasecmp(r,s,strlen(s))) == 0 && atoi(quantity)==0) {
					wattron(bill_win,COLOR_PAIR(5));
      mvwprintw(bill_win,16+i,40,"%-10d %-30s %-20s %-16s", atoi(number), name, brand,message);
					wattroff(bill_win,COLOR_PAIR(5));	
						i++;								
					}
					else 
						continue;
				}
				}
				break;
			case KEY_BACKSPACE :
				form_driver(bill_form ,REQ_DEL_PREV);
				break;
			case 10 :	
			if(current_field(bill_form)==field[4]) {
				for(i = 0; i < 4; i++) {
					value[i]=field_buffer(field[i], 0);
				}
				/*Modifying Quantity of current item in item.csv file */
				fseek(fdr,0,SEEK_SET);	
				while(!feof(fdr)) {
					count = readline(fdr, line, 200);
					if(count == 0)
						break;
					number = strtok(line, ",");		
					name = strtok(NULL, ",");		
					location = strtok(NULL, ",");
					brand = strtok(NULL, ",");
					quantity = strtok(NULL, ",");
					rate = strtok(NULL, ",");
					
					if(!number || !name || !location||!brand || !quantity || !rate)
						break;
					r=trim(name);	
					s=trim(value[1]);
					p=trim(brand);	
					q=trim(value[2]);
					/* New quantity is the remaining items in the inventory*/
					newquantity=atoi(quantity) - atoi(trim(value[3]));
					
					if(((strcasecmp(r,s))==0) &&((strcasecmp(p,q))==0) ) {
						fseek(fdr,-(count+1) ,SEEK_CUR);
				fprintf(fdr,"%s,%s,%s,%s,%d,%s",number,name,location,
								brand,newquantity,rate);
						break;		
					}				
				}
				fclose(fdr);
				/*Adding the item to the file if stock is there*/		 
				fdw=fopen("bill.csv","a");
				fprintf(fdw,"%s,%s,%d,%lf\n",trim(brand),trim(name),atoi(value[3]),atof(rate));
				fclose(fdw);
				fdr=fopen("item.csv","r+");
				for(i = 0; i < 4; i++) {
					form_driver(bill_form ,REQ_NEXT_FIELD);
					form_driver(bill_form ,REQ_END_LINE);
					form_driver(bill_form ,REQ_CLR_FIELD);
				}
			}
			break;	
		}
	}
	fclose(fdr);	
	unpost_form(bill_form );
	free_form(bill_form );
	for(i = 0; i < 5; i++)
		free_field(field[i]);
	wmove(bill_win,0, 0); 
	wclear(bill_win);
	wrefresh(bill_win);
	delwin(bill_win);
	return;		
}
/* It will display all the current items in the cart */
void bill_view() {
	WINDOW *display_win;
	int count, i=0, j=0, ch, start=0, height=30;
	char *title[] = {"Sr no.","Brand","Name","Quantity","Rate(in Rs)","Amount"};
	char line[200];
	char *brand,*name,*quantity,*rate;   
	FILE *fd;
	bill *b[300] ;
	
	for(i=0;i<4;i++) {		//Clearing background screen first
		move(LINES-i,0);
		clrtoeol();	
		refresh();
	}	
	/*Display part*/
	display_win=newwin(40, 164, 15, 10);
	box(display_win, 0 , 0);	
	wrefresh(display_win);
	wattron(display_win,COLOR_PAIR(1) | A_BOLD );
	mvwprintw(display_win,2,25,"%-10s %-30s %-30s %-10s %-12s %-12s",title[0],title[1],title[2],title[3],title[4],title[5]);
	wattroff(display_win,COLOR_PAIR(1) | A_BOLD );
	mvwhline(display_win,3, 0,  ACS_HLINE, 164);
	mvwhline(display_win,35, 0,  ACS_HLINE, 164);
	keypad(display_win,TRUE);
	
	wattron(display_win,COLOR_PAIR(5) | A_STANDOUT);
	mvwprintw(display_win,37, 1, "Press PAGE UP AND PAGE DOWN TO SCROLL");
	mvwprintw(display_win,38, 1, "Press ESC to exit to menu bar ");
	wattroff(display_win,COLOR_PAIR(5) | A_STANDOUT);
		
	/*Reading part*/
	fd=fopen("bill.csv","r");	/*reading from a csv file */
	if(fd==NULL)
		return;
	box(display_win, 0 , 0);	//making borders again
	// Reading csv file and tokenizing the lines of files.
	subtotal=0;
	while(!feof(fd)) {
		count = readline(fd, line, 200);
		if(count == 0)
			break;
		brand = strtok(line, ",");		
		name = strtok(NULL, ",");		
		quantity = strtok(NULL, ",");
		rate = strtok(NULL, ",");
		
		b[j]=(bill *)malloc(sizeof(bill));
		b[j]->number=j+1;
		b[j]->name=(char*)malloc(sizeof(char)* 30);
		b[j]->brand=(char*)malloc(sizeof(char)* 30);
		strcpy(b[j]->name,name);
		strcpy(b[j]->brand,brand);
		b[j]->quantity=atoi(quantity);
		b[j]->rate=atof(rate);
		b[j]->amount=(atoi(quantity) * atof(rate));
		subtotal = subtotal + (b[j]->amount);
		j++;	
	}
	wattron(display_win,COLOR_PAIR(5) | A_BOLD | A_STANDOUT);
	mvwprintw(display_win,20, 72, "Press ENTER key to continue.");
	mvwprintw(display_win,36, 120, "No. of items in Cart :%d ",j);
	wattroff(display_win,COLOR_PAIR(5)| A_BOLD | A_STANDOUT);
	wattron(display_win,COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
	mvwprintw(display_win,38, 120, "Subtotal :Rs %-7.2lf /- ",subtotal);
	wattroff(display_win,COLOR_PAIR(1) | A_BOLD | A_STANDOUT);
		
	while((ch = wgetch(display_win)) != 27){	// Scrolling in bill display
		wmove(display_win,20, 72);
		wclrtoeol(display_win);		
		switch(ch) {
			case KEY_DOWN:
				if((start+height) < j)
					start++;
				break;
			case KEY_UP:
				if(start > 0)
					start--;
				break;	
		}
		for(i=start; i<(start+height) && i < j; i++) {	//j->item count
		wattron(display_win,COLOR_PAIR(5));
		mvwprintw(display_win,4 + (i-start),25 ,"%-10d %-30s %-30s %-10d %-12.2lf %-12.2lf\n", b[i]->number, b[i]->brand,
										 b[i]->name,b[i]->quantity,b[i]->rate,b[i]->amount);
		wattroff(display_win,COLOR_PAIR(5));
		box(display_win, 0 , 0);
		wrefresh(display_win);
		}		
	}
	for(i=0;i<j;i++) {
		free(b[i]->name);
		free(b[i]->brand);
		free(b[i]);
	}
	fclose(fd);
	wclear(display_win);
	wrefresh(display_win);
	delwin(display_win);
	return;
}
/* It contains form containing customer and owner information.*/ 
void info_form() {
	FIELD *field[8];
	FILE *fd;
	FORM  *item_form;
	WINDOW *item_win;
	int ch, rows, cols, i;
	char *info_title[] = {"Company Name:","Address:","Contact no.:","Name:","Address:","Contact no:",};
	char *owninfo[3],*custinfo[3];
	
	init_screen();		 	
	init_colorpair();		
	
	for(i = 0; i < 3; i++) {
		field[i] = new_field(1, WIDTH, STARTY + i * 3, STARTX, 0, 0);	//(0-2)
		field[i+3]=new_field(1, WIDTH, STARTY + (i+4) * 3, STARTX, 0, 0);// (3-5)
	}	
	field[6] = new_field(1, 10, STARTY + 7 * 3, STARTX + 10, 0, 0); // Button for enter 
	field[7] = NULL;

	for(i = 0; i < 6; i++) {
		
		set_field_fore(field[i], COLOR_PAIR(2));
		set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
		field_opts_off(field[i], O_AUTOSKIP); 
	
	}		      
	set_field_fore(field[ 6], COLOR_PAIR(3));		// Button
	set_field_back(field[ 6], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[ 6], O_AUTOSKIP);
	field_opts_off(field[ 6], O_EDIT);
	set_field_buffer(field[6], 0, "  ENTER");
	item_form = new_form(field);
	scale_form(item_form, &rows, &cols);
        item_win = newwin(40, 164, 15, 10);
        keypad(item_win, TRUE);
        set_form_win(item_form, item_win);
        set_form_sub(item_form, derwin(item_win, rows, cols, 2, 2));
        box(item_win, 0, 0);
        
        print_in_middle(item_win, 1, 0, 75, "ENTER DETAILS", COLOR_PAIR(1));
        wattron(item_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT );
	mvwprintw(item_win, STARTY +5, STARTX +33,  "OWNER INFO ");		// Printing side-titles
	mvwprintw(item_win, STARTY + 17, STARTX +33, "CUSTOMER INFO");
	wattroff(item_win , COLOR_PAIR(1) |A_BOLD | A_STANDOUT);
	
	post_form(item_form);
	wrefresh(item_win);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );						// Instructions-bill
	mvprintw(LINES -2 , 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1 , 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	refresh();
	for(i = 0; i < 3; i++) {
		wattron(item_win , COLOR_PAIR(1) | A_BOLD);
		mvwprintw(item_win, STARTY + 2 + (i * 3), STARTX - 20 ,"%s", info_title[i]);
		mvwprintw(item_win, STARTY + 2 + ((i+4) * 3), STARTX - 20 ,"%s", info_title[i+3]);
		wattroff(item_win , COLOR_PAIR(1) | A_BOLD);
	}	
	while((ch = wgetch(item_win))!=27)
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(item_form, REQ_NEXT_FIELD);
				form_driver(item_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(item_form, REQ_PREV_FIELD);
				form_driver(item_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(item_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(item_form, REQ_RIGHT_CHAR);
				break;
			default:
				form_driver(item_form, ch);
				break;
			case KEY_BACKSPACE :
				form_driver(item_form,REQ_DEL_PREV);
				break;
			case 10 :	
			if(current_field(item_form)==field[6] ) {
				for(i = 0; i < 3; i++) {
					owninfo[i]=field_buffer(field[i], 0);
					custinfo[i]=field_buffer(field[i+3], 0);	
				}
				fd=fopen("info.txt","w");
			fprintf(fd , "%s;%s;%s;%s;%s;%s\n", trim(owninfo[0]),trim(owninfo[1]),trim(owninfo[2]),
							     trim(custinfo[0]),trim(custinfo[1]),trim(custinfo[2]));
				fclose(fd);
				wattron(item_win , COLOR_PAIR(1));
				mvwprintw(item_win, STARTY + 25, STARTX + 3, "Data saved successfully.");	
				wattroff(item_win , COLOR_PAIR(1));	
				for(i = 0; i < 6; i++) {
					form_driver(item_form,REQ_NEXT_FIELD);
					form_driver(item_form,REQ_END_LINE);
					form_driver(item_form,REQ_CLR_FIELD);
				}			
			}
			break;	
		}
	}
	unpost_form(item_form);
	free_form(item_form);
	for(i = 0; i < 7; i++)
		free_field(field[i]);
	wmove(item_win,0, 0); 
	wclear(item_win);
	wrefresh(item_win);
	delwin(item_win);
	return;
}
/* It will show the final bill attributes and also contains button to print the bill in .txt file */ 
void bill_attributes() {
	FIELD *field[6];
	FORM  *item_form;
	FILE *fd;
	WINDOW *item_win;
	int ch, rows, cols, i;
	char *item_title[] = {"Subtotal(in Rs.):","Total taxes(%):","Discount(%):","Grand total(in Rs.)"};
	char sub[30],tax[30],grand[30]; // Strings corresponding to subtotal and total tax
	char *bill[4];
	
	init_screen();		
	init_colorpair();		
	
	for(i = 0; i <4; i++)
		field[i] = new_field(1, WIDTH, STARTY + i * 3, STARTX, 0, 0);
	field[4] = new_field(1, 15, STARTY + 4 * 3, STARTX + 5, 0, 0); // Button
	field[5] = NULL;

	for(i = 0; i < 4; i++) {
		
		set_field_fore(field[i], COLOR_PAIR(2));
		set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
		field_opts_off(field[i], O_AUTOSKIP); 
	
	}
	//str = convert subtotal(double) into string "sub" and setting respective field buffer
	sprintf(sub, "%lf", subtotal);		
	set_field_buffer(field[0], 0, sub);
	field_opts_off(field[0], O_EDIT);
		
	//str = convert tax_rate(double) into string "tax" and setting respective field buffer
	sprintf(tax, "%lf", tax_rate);	
	set_field_buffer(field[1], 0, tax);
	field_opts_off(field[1], O_EDIT);
			      
	set_field_fore(field[4], COLOR_PAIR(3));		// Setting attributes of button
	set_field_back(field[4], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[4], O_AUTOSKIP);
	field_opts_off(field[4], O_EDIT);
	set_field_buffer(field[4], 0, "PRINT BILL");
	item_form = new_form(field);
	scale_form(item_form, &rows, &cols);
        item_win = newwin(40, 164, 15, 10);
        keypad(item_win, TRUE);
        set_form_win(item_form, item_win);
        set_form_sub(item_form, derwin(item_win, rows, cols, 2, 2));
        box(item_win, 0, 0);
	print_in_middle(item_win, 1, 14, cols + 4, "BILL SUMMARY", COLOR_PAIR(1));
	
	post_form(item_form);
	wrefresh(item_win);
	
	attron(COLOR_PAIR(5) | A_STANDOUT );						// Instructions-bill
	mvprintw(LINES -2 , 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1 , 0, "Press ESC key to exit to menu bar.");
	attroff(COLOR_PAIR(5) | A_STANDOUT );	
	refresh();
	/*Printing the field titles on the screen */
	for(i = 0; i < 4; i++) {
		wattron(item_win , COLOR_PAIR(1) | A_BOLD);
		mvwprintw(item_win, STARTY + 2 + (i * 3), STARTX - 20 ,"%s", item_title[i]);
		wattroff(item_win , COLOR_PAIR(1) | A_BOLD);
	}
	wattron(item_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT );
	mvwprintw(item_win, STARTY + 2, STARTX +33,  "PLS VIEW CART BEFORE CHECKOUT");
	mvwprintw(item_win, STARTY + 8, STARTX +33,  "PLS PRESS ""DOWN KEY"" AFTER DISCOUNT VALUE");
	wattroff(item_win , COLOR_PAIR(1) | A_BOLD | A_STANDOUT );	
	while((ch = wgetch(item_win)) !=27)
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(item_form, REQ_END_LINE);
				if(current_field(item_form)==field[2] ) {	//If current field is discount 	
					discount=atof(trim(field_buffer(field[2], 0)));	//Getting discount value
					/* Calculating the grand total */ 
					grand_total = subtotal -(subtotal*discount)/100 + (subtotal*tax_rate)/100;  
					sprintf(grand, "%lf", grand_total);	
					set_field_buffer(field[3], 0, grand);	// Setting the grand total field buffer
				}
				form_driver(item_form, REQ_NEXT_FIELD);
				break;
			case KEY_UP:
				form_driver(item_form, REQ_PREV_FIELD);
				form_driver(item_form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(item_form, REQ_LEFT_CHAR);
				break;	
			case KEY_RIGHT:
				form_driver(item_form, REQ_RIGHT_CHAR);
				break;
			default:
				form_driver(item_form, ch);
				break;
			case KEY_BACKSPACE :
				form_driver(item_form,REQ_DEL_PREV);
				break;
				
			case 10 :
			if(current_field(item_form)==field[4] ) {
				grand_total = subtotal -(subtotal*discount)/100 + (subtotal*tax_rate)/100; 
				total_transaction= total_transaction + grand_total;
				for(i = 0; i < 4; i++) {
					form_driver(item_form,REQ_NEXT_FIELD);
					form_driver(item_form,REQ_END_LINE);
					form_driver(item_form,REQ_CLR_FIELD);
				}
				// Function to update bill text file
				bill_print();		
				wattron(item_win , COLOR_PAIR(1));
				mvwprintw(item_win, STARTY + 20, STARTX, "Printing.......");
				mvwprintw(item_win, STARTY + 21, STARTX, "Bill file generated is:bill%d.txt ",invoice_number-1);	
				wattroff(item_win , COLOR_PAIR(1));
				// Open the bill.csv in "w" mode so that it gets deleted.
				fd=fopen("bill.csv","w");
				fclose(fd);
			}
			break;	
		}
	}
	unpost_form(item_form);
	free_form(item_form);
	for(i = 0; i < 5; i++)
		free_field(field[i]);
	wmove(item_win,0, 0); 
	wclear(item_win);
	wrefresh(item_win);
	delwin(item_win);
	return;
}
/* This function will print all the required details in .txt file as properly formatted text.*/  
void bill_print() {
	int count, i ,ch, j=0, total_quantity;
	FILE *fd, *fd_info, *fd_user, *fd_bill;
	char *own_address,*own_number,*cust_name,*cust_address,*cust_number;
	char *title[] = {"Sr no.","Brand","Name","Quantity","Rate(in Rs)","Amount"};
	char *brand,*name,*quantity,*rate;
	char line[150],cashier_name[30],own_name[30];
	char inv[4], filename[10];
	char pdfcommand[50];
	bill *b[300] ;
	
	/*Naming the bill .txt file */
	// Eg. First bill will have name : bill1.txt
	sprintf(inv, "%d", invoice_number);
	strcat(filename,"");
	strcat(filename,"bill");
	strcat(filename,inv);
	strcat(filename,".txt");
	
	fd_info=fopen("info.txt","r");
	while(!feof(fd_info)) {
		count = readline(fd_info, line, 150);
		if(count == 0)
			break;
		strcpy(own_name,strtok(line, ";"));		
		own_address = strtok(NULL, ";");
		own_number = strtok(NULL, ";");
		cust_name = strtok(NULL, ";");
		cust_address = strtok(NULL, ";");
		cust_number = strtok(NULL, ";");
	}	
	fclose(fd_info);
	
	fd_user=fopen("username.txt","r");
	fscanf(fd_user,"%s",cashier_name);
	fclose(fd_user);
	
	fd=fopen(filename,"w");	// Deleting all the contents of bill text file if already exists
	fclose(fd);
	
	fd=fopen(filename,"a");		// Bill(Invoice) Text file 
	fprintf(fd , "\n\t\t%s\n\t\t%s\n", own_name, own_address);
	fprintf(fd , "\t\tContact No:%s\n\n", own_number);
	
	for( i=0 ; i < 95; i++ ) {
   		//ch='-'; ASCII value:45
      		fputc(45, fd);
   	}
   	print_time();
   	fprintf(fd , "\n\tInvoice#:%d%02d%02d%003d\t\tDate: %02d-%02d-%d %02d:%02d \n", year, month, day, invoice_number,
   									day,month,year,current_hour,current_minute);
   	invoice_number++;
   	for( i=0 ; i < 95; i++ ) {
   		ch='-';
      		fputc(ch, fd);
   	}
	fclose(fd);
	
	fd_bill=fopen("bill.csv","r");	
	while(!feof(fd_bill)) {
		count = readline(fd_bill, line, 150);
		if(count == 0)
			break;
		brand = strtok(line, ",");		
		name = strtok(NULL, ",");		
		quantity = strtok(NULL, ",");
		rate = strtok(NULL, ",");
		
		b[j]=(bill *)malloc(sizeof(bill));
		b[j]->number=j+1;
		b[j]->name=(char*)malloc(sizeof(char)* 30);
		b[j]->brand=(char*)malloc(sizeof(char)* 30);
		strcpy(b[j]->name,name);
		strcpy(b[j]->brand,brand);
		b[j]->quantity=atoi(quantity);
		b[j]->rate=atof(rate);
		b[j]->amount=(atoi(quantity) * atof(rate));
		total_quantity=total_quantity + b[j]->quantity;
		j++;	
	}
	fd=fopen(filename,"a");		// Bill(Invoice) Text file 	
	fprintf(fd ,"\n");	
	fprintf(fd ,"\t%-10s %-20s %-20s %-10s %-12s %-12s\n",title[0],title[1],title[2],title[3],title[4],title[5]);
	for( i=0 ; i < 95; i++ ) {
   		ch='-';
      		fputc(ch, fd);
   	}	
   	fprintf(fd ,"\n");			   	       
	for(i=0; i < j; i++) {			//j->item count
		
	fprintf(fd ,"\t%-10d %-20s %-20s %-10d %-12.2lf %-12.2lf\n", b[i]->number, b[i]->brand, b[i]->name,
							      b[i]->quantity,b[i]->rate,b[i]->amount);
							   	 
	}		
	for( i=0 ; i < 95; i++ ) {
   		ch='-';
      		fputc(ch, fd);
   	}
   	fprintf(fd ,"\n\t\t\t\t\tTotal Quantity  :     %-10d\n",total_quantity);
   	fprintf(fd ,"\n\t\t\t\t\tSub Total       :Rs %-12.2lf\n",subtotal);
   	fprintf(fd ,"\t\t\t\t\t  Total Taxes  	 :%-12.2lf %%\n",tax_rate);
   	fprintf(fd ,"\t\t\t\t\t  Discount        :%-12.2lf %%\n",discount);		
   	fprintf(fd ,"\t\t\t\t\t  Grand Total     :Rs %-12.2lf\n",grand_total);
   	fprintf(fd ,"\n\tPAYMENT TYPE:Cash\n");
   	fprintf(fd ,"\tCASHIER:%s\n",cashier_name);
   	
   	fprintf(fd,"\n\tCUSTOMER INFO:\n\n\tName:%s\n\tAddress:%s\n\tContact number:%s\n",cust_name,cust_address,cust_number);
   	fprintf(fd, "\n\t\t\t!!!!THANK YOU.PLS VISIT US AGAIN!!!!\n");
   	
   	/*Printing the mandatory govt. guidelines in the bill */  
 fprintf(fd, "\n\tWe hereby certify that our registration certificate under\n\tthe Maharashtra VAT Act 2002 is in force on date on which\n\tthe sale of goods specified in the Bill/Invoice is made\n\tby us and that the transaction of sale covered by this \n\tbill has been effected by us and it shall be accounted \n\tfor in the turnover of sales while filing out return\n\n\tKindly exchange goods within 48 hrs.\n\n");
   	fprintf(fd,"\tFor any complaints,contact:%s\n",own_number);
   	for( i=0 ; i < 95; i++ ) {
   		ch='-';
      		fputc(ch, fd);
   	} 
   	fprintf(fd, "\n\tFREE HOME DELIVERY\n\tCall :%s\n",own_number);
   	for( i=0 ; i < 95; i++ ) {
   		ch='-';
      		fputc(ch, fd);
   	}   						     
	fclose(fd);
	
	/*strcpy(pdfcommand, "libreoffice --convert-to pdf filename" );	//Copying command to pdfcommand
   	system(pdfcommand);						//Making system call to convert txt file to pdf.*/

	for(i=0;i<j;i++) {
		free(b[i]->name);
		free(b[i]->brand);
		free(b[i]);
	}	
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








