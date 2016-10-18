#include<stdio.h>
#include<menu.h>
#include<form.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

#define STARTX 30
#define STARTY 5
#define WIDTH 30

#define N_FIELDS 8

#define SIZE 150
int count_item=0; // Global variable for no. of items
int count_tax=0; // Global variable for no. of taxes
char *line[SIZE]; // It stores ptr to all lines in item.csv

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

void print_time(){

	time_t rawtime;
  	struct tm * timeinfo;

  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	attron(COLOR_PAIR(2) );
  	//mvprintw ( 1,120, "Current local time and date: %s", asctime (timeinfo) );
  	mvprintw ( 1,130, "[%d %d %d %d:%d:%d]",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, 							timeinfo->tm_min, timeinfo->tm_sec);
  	attron(COLOR_PAIR(2) );

}
int item_store(char **str) { 		//returns -1 if successful else "i"->field number
	int i=0 , j, len ;
	FILE *fd;
	item *q;
	q=(item *)malloc(sizeof(item));
	
	while(i < 6) {			// Validation
	
		if( i==0 || i== 4 || i== 5 ) {
			len = strlen(str[i]);
    			for (j=0;j<len; j++) {
        			if (!((isdigit(str[i][j])) || (str[i][j])=='.' || (str[i][j])==32))
        				return i;	
        		}	
        	}
        	i++;			
	}
	// Writing to file if all fields properly validated
	
	fd=fopen("item.csv","a+");
	
	q->number=atoi(str[0]);
		q->name=(char*)malloc(sizeof(char)* 30);
	q->name=  str[1];
		q->location=(char*)malloc(sizeof(char)* 30);
	q->location=  str[2];
		q->brand=(char*)malloc(sizeof(char)* 30);
	q->brand=str[3];
	q->quantity=atoi(str[4]);
	q->rate=atof(str[5]);
				/*Making a csv file */
	fprintf(fd , "%d,%s,%s,%s,%d,%lf\n", q->number, q->name, q->location,q->brand,q->quantity,q->rate);
	
	count_item++; 
	free(q);
	fclose(fd);
	return -1;
}
void item_read() {
	FILE *fd;
	fd=fopen("item.csv","r");	/*reading from a csv file */

	// Reading csv file and tokenizing the lines of files.

	fclose(fd);
	return;
}
int tax_store(char **str) {		// returns 0 if successful else 1
	int i=0 , j, len ;
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
	
	td=fopen("tax.txt","a+");
	
	t->name=(char*)malloc(sizeof(char)* 30);
	t->name=  str[0];
	t->value=atof(str[1]);
	
	fprintf(td , "%s%lf\n", t->name, t->value);
	
	count_tax++; 
	free(t);
	fclose(td);
	return 0;
}
void tax_print(WINDOW *w) {
	int i , j, len ;
	FILE *td;
	tax *t;
	t=(tax *)malloc(sizeof(tax));
	t->name=(char*)malloc(sizeof(char)* 30);

	td=fopen("tax.txt","r");
	
	i=1;
	while(!feof(td)) {
		fscanf(td , "%s%lf\n", t->name, &t->value);
		wattron(w , COLOR_PAIR(5) );
		mvwprintw(w ,13 + (2 * i), 110 , "%d)%s       		:%lf",i, t->name, t->value);
		wattroff(w , COLOR_PAIR(5));
		i++;
	}
	free(t);
	fclose(td);
	return;
}
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void item_menu();
void tax_menu() ;

//WINDOW *create_newwin(int height, int width, int starty, int startx);

char *choices[] = {
                        "Home", "Item master", "Inventory" , "Tax master", "Bill", "Exit"
                  };
MENU *my_menu;
int main(int argc, char* argv[]) {
	
	WINDOW *my_win1, *my_win2;
	int startx, starty, width, height;
	int ch, x, y;
	tax *t;
	
	ITEM **my_items;
	int c;				
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices, i;
        
	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on
					 * every thing to me 		*/
	noecho();
	keypad(stdscr, TRUE);		/* We can use F1, keypad and all.	*/
	
	initscr();			/* initialize NCurses */
	start_color();		
	init_pair(2, COLOR_CYAN, COLOR_BLACK);	
	
	// TEXT DISPLAY 
	move(1,75);
	attron(COLOR_PAIR(2) | A_STANDOUT | A_BOLD);
	printw("WELCOME TO GROCERY BILLING SYSTEM");
	attroff(COLOR_PAIR(2) | A_STANDOUT | A_BOLD);
	print_time();           /* Print current time and date */
	refresh();		/* update the screen */
	
	height = 10;
	width = 10;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	
	
	init_pair(30, COLOR_RED, COLOR_BLACK);	// colors initialization
	init_pair(40, COLOR_CYAN, COLOR_BLACK);
	init_pair(50, COLOR_WHITE, COLOR_BLUE);
	init_pair(60, COLOR_WHITE, COLOR_BLACK);
	init_pair(70, COLOR_YELLOW, COLOR_BLACK);
	
	//bkgd(COLOR_PAIR(60)); //Changing stdscr's background color
	//refresh();
	my_win1 =newwin(5, 164, 5, 10);		// Window 1
	box(my_win1, 0 , 0);
	wbkgd(my_win1, COLOR_PAIR(50));
	wrefresh(my_win1);
		
	my_win2 =newwin(40, 164, 15, 10);	// Window 2
	box(my_win2, 0 , 0);	
	wbkgd(my_win2, COLOR_PAIR(1));
	wrefresh(my_win2);
	
	//getmaxyx(stdscr,y,x);
	//printw("Window size is %d rows, %d columns.\n",y,x);
	
	/* Create items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i) 
                my_items[i] = new_item(choices[i], choices[i]);
	my_items[i]=(ITEM *)NULL;	
	
	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set menu option not to show the description */
	menu_opts_off(my_menu, O_SHOWDESC);

	/* Create the window to be associated with the menu */
        keypad(my_win1, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_win1);
        set_menu_sub(my_menu, derwin(my_win1, 1, 100, 2, 30));
	set_menu_format(my_menu, 1, 6);
	set_menu_mark(my_menu, " -> ");

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu,COLOR_PAIR(30) | A_REVERSE | A_BOLD);
	set_menu_back(my_menu,COLOR_PAIR(40));
	set_menu_grey(my_menu,COLOR_PAIR(50));

	attron(COLOR_PAIR(70) | A_STANDOUT);
	mvprintw(LINES - 4, 0, "Press <ENTER> to select an option");
	mvprintw(LINES - 3, 0, "Use PageUp and PageDown to scroll");
	mvprintw(LINES - 2, 0, "Use Arrow Keys to navigate (F1 to Exit)");
	attroff(COLOR_PAIR(70) | A_STANDOUT);
	refresh();

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_win1);
	
	while((ch = wgetch(my_win1)) != KEY_F(1))
	{       switch(ch)
	        {	
			case KEY_LEFT:
				menu_driver(my_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(my_menu, REQ_RIGHT_ITEM);
				break;
			case 10:					//ENTER 
				//attron(COLOR_PAIR(4));
				//move(LINES - 5,0);
				//clrtoeol();
				//mvprintw(LINES - 5, 0, "Item selected is : %s",item_name(current_item(my_menu)));
				//attroff(COLOR_PAIR(4));
				pos_menu_cursor(my_menu);
				refresh();
				
				if(strcmp(item_name(current_item(my_menu)),"Item master") == 0 ) {
					for(i = 2;i < 5;i++) {
						move(LINES - i,0);
						clrtoeol();
					}
					refresh();
					item_menu();
				}
				else if(strcmp(item_name(current_item(my_menu)),"Inventory") == 0 ) {
				
				}
				else if(strcmp(item_name(current_item(my_menu)),"Tax master") == 0 ) {
					for(i = 2;i < 5;i++) {
						move(LINES - i,0);
						clrtoeol();
					}
					refresh();
					tax_menu();
				}
				else if(strcmp(item_name(current_item(my_menu)),"Bill") == 0 ) {
				
				}
				else if(strcmp(item_name(current_item(my_menu)),"Exit") == 0 ) {
					endwin();
					return 0;
				}
				break;
		}
                wrefresh(my_win1);
                wrefresh(my_win2);
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);

	endwin();			/* clean up NCurses */	
	return 0;
}	
void item_menu() {
	FIELD *field[N_FIELDS];
	FORM  *my_form;
	WINDOW *my_form_win;
	int ch, rows, cols, i, res, s = -1, t;
	char *value[7];
	
	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize few color pairs */
   	init_pair(1, COLOR_CYAN, COLOR_BLACK);	
	init_pair(2, COLOR_WHITE,COLOR_BLUE);
	init_pair(3, COLOR_BLUE, COLOR_RED);
	init_pair(15 ,COLOR_RED, COLOR_BLACK );
	/* Initialize the fields */
	for(i = 0; i < N_FIELDS - 2; i++)
		field[i] = new_field(1, WIDTH, STARTY + i * 3, STARTX, 0, 0);
	field[N_FIELDS - 2] = new_field(1, 10, STARTY + i * 3, STARTX + 10, 0, 0); // Button
	field[N_FIELDS - 1] = NULL;

	/* Set field options */
	for(i = 0; i < N_FIELDS - 2; i++) {
		
		set_field_fore(field[i], COLOR_PAIR(2));/* Put the field with blue background */
		set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
		field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this is filled up */
	
	}
	/* Setting the attributes of the button. */				      
	set_field_fore(field[ N_FIELDS - 2], COLOR_PAIR(3));		// Button
	set_field_back(field[ N_FIELDS - 2], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[ N_FIELDS - 2], O_AUTOSKIP);
	field_opts_off(field[ N_FIELDS - 2], O_EDIT);
	set_field_buffer(field[N_FIELDS - 2], 0, "  ADD");/* Initialize the field */

	/* Create the form and post it */
	my_form = new_form(field);
	
	/* Calculate the area required for the form */
	scale_form(my_form, &rows, &cols);

	/* Create the window to be associated with the form */
        my_form_win = newwin(40, 164, 15, 10);
        keypad(my_form_win, TRUE);

	/* Set main window and sub window */
        set_form_win(my_form, my_form_win);
        set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

	/* Print a border around the main window and print a title */
        box(my_form_win, 0, 0);
	print_in_middle(my_form_win, 1, 14, cols + 4, "ITEM DESCRIPTION", COLOR_PAIR(1));
	
	post_form(my_form);
	wrefresh(my_form_win);
	
	attron(COLOR_PAIR(70) | A_STANDOUT );						// Instructions-item
	mvprintw(LINES -2 , 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1 , 0, "Press F1 key to exit");
	attroff(COLOR_PAIR(70) | A_STANDOUT );	
	refresh();
	
	/* Printing the labels of the fields */
	wattron(my_form_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(my_form_win, STARTY + 2, STARTX - 20 , "Item no.:");
	mvwprintw(my_form_win, STARTY + 5, STARTX - 20 , "Item name:");
	mvwprintw(my_form_win, STARTY + 8, STARTX - 20 , "Location:");
	mvwprintw(my_form_win, STARTY + 11, STARTX - 20 , "Brand:");
	mvwprintw(my_form_win, STARTY + 14, STARTX - 20 , "Quantity:");
	mvwprintw(my_form_win, STARTY + 17, STARTX - 20 , "Rate (in Rs) :");
	wattroff(my_form_win , COLOR_PAIR(1) | A_BOLD);
	
	
	/* Loop through to get user requests */
	while((ch = wgetch(my_form_win)) != KEY_F(1))
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);/* Go to the end of the present buffer */
				form_driver(my_form, REQ_END_LINE);/* Leaves nicely at the last character */
				break;
			case KEY_UP:
				form_driver(my_form, REQ_PREV_FIELD);/* Go to previous field */
				form_driver(my_form, REQ_END_LINE);
				break;
			default:
				if(s!=-1) {
					wmove(my_form_win,STARTY + (s * 3) + 3 , STARTX + 2);
					wclrtoeol(my_form_win);
				}
				form_driver(my_form, ch);/* If this is a normal character, it gets printed*/
				break;
			case KEY_BACKSPACE :
				form_driver(my_form,REQ_DEL_PREV);
				break;
			case 10 :	
			if(current_field(my_form)==field[N_FIELDS - 2] ) {
				for(i = 0; i < N_FIELDS - 2; i++) {
					value[i]=field_buffer(field[i], 0);
					//mvwprintw(my_form_win, 29, 0 , "%s", value[i]);
				}
				
				/*for(i=0; i<30 ; i++ )
					mvwprintw(my_form_win, i, 0 , "%d", value[0][i]);*/
				s=item_store(value);
				if( s == -1) {
					wattron(my_form_win , COLOR_PAIR(1));
					mvwprintw(my_form_win, STARTY + 22, STARTX, "Data saved successfully.");	
					wattroff(my_form_win , COLOR_PAIR(1));	
					for(i = 0; i < N_FIELDS - 2; i++) {
						form_driver(my_form,REQ_NEXT_FIELD);
						form_driver(my_form,REQ_END_LINE);
						form_driver(my_form,REQ_CLR_FIELD);
					}	
				
				}
				else {	
					wattron(my_form_win , COLOR_PAIR(15) | A_BOLD);			
					mvwprintw(my_form_win,STARTY + (s * 3) + 3 , STARTX + 2, "Error in the above field");
					wattroff(my_form_win , COLOR_PAIR(15) | A_BOLD);
				}		
			}
			break;	
		}
	}

	/* Un post form and free the memory */
	unpost_form(my_form);
	free_form(my_form);
	for(i = 0; i < N_FIELDS - 1; i++)
		free_field(field[i]);
	 
	endwin();
	return;
}
void tax_menu() {
	FIELD *field[6];
	FORM  *my_form;
	WINDOW *my_form_win;
	int ch, rows, cols, i, t;
	char *tvalue[2];
	
	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	/* Initialize few color pairs */
   	init_pair(1, COLOR_CYAN, COLOR_BLACK);	
	init_pair(2, COLOR_WHITE,COLOR_BLUE);
	init_pair(3, COLOR_BLUE, COLOR_RED);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(15 ,COLOR_RED, COLOR_BLACK );	
	/* Initialize the fields */
	
	field[0] = new_field(1, WIDTH, STARTY, STARTX, 0, 0);
	field[1] = new_field(1, WIDTH, STARTY + 3, STARTX, 0, 0);
	field[2] = new_field(1, 10, STARTY + 6, STARTX, 0, 0); // Button for "SAVE"
	field[3] = new_field(1, 10, STARTY + 6, STARTX + 15, 0, 0); // Button for "UPDATE"
	field[4] = new_field(1, 25, STARTY + 8, STARTX , 0, 0); // Button for "INCLUDE STANDARD TAXES"
	field[5] = NULL;

	
	for(i = 0; i < 5;i++) {
		if(i==0 || i==1) {
		
	set_field_fore(field[i], COLOR_PAIR(2));/* Put the field with blue background */
	set_field_back(field[i], COLOR_PAIR(2) | A_UNDERLINE);
	field_opts_off(field[i], O_AUTOSKIP); /* Don't go to next field when this is filled up */
		}
		else if (i==2 || i==3 || i==4) {     
	set_field_fore(field[i], COLOR_PAIR(3));		// Button
	set_field_back(field[i], COLOR_PAIR(3) | A_BOLD);
	field_opts_off(field[i], O_AUTOSKIP);
	field_opts_off(field[i], O_EDIT);
		}
	}
	
	set_field_buffer(field[2], 0, "  SAVE");	/* Initialize the field */
	set_field_buffer(field[3], 0, "  UPDATE");
	set_field_buffer(field[4], 0, "  INCLUDE STANDARD TAXES");
	
	/* Creating window + posting form*/
	my_form = new_form(field);
	scale_form(my_form, &rows, &cols);
	my_form_win = newwin(40, 164, 15, 10);
        keypad(my_form_win, TRUE);
	set_form_win(my_form, my_form_win);
        set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));
	box(my_form_win, 0, 0);
	print_in_middle(my_form_win, 1, 0, 75, "TAX DESCRIPTION", COLOR_PAIR(1));
	post_form(my_form);
	wrefresh(my_form_win);
	
	for(i=1;i< 39 ;i++)
		mvwprintw(my_form_win, i , 75 , "|"); 					// Vertical Partition
	
	attron(COLOR_PAIR(70) | A_STANDOUT );						// Instructions-tax
	mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields");
	mvprintw(LINES -1, 0, "Press F1 key to exit");
	attroff(COLOR_PAIR(70) | A_STANDOUT );	
	
	print_in_middle(my_form_win, 1, 75, 90, "STANDARD TAXES", COLOR_PAIR(1));	//standard tax box
	wattron(my_form_win , COLOR_PAIR(5) );
	mvwprintw(my_form_win ,	3, 110, "1) Service Tax : 14.2 %%");
	mvwprintw(my_form_win , 5, 110, "2) VAT         : 2    %%");
	mvwprintw(my_form_win , 7, 110, "3) Other taxes : 0.5  %%");
	wattroff(my_form_win , COLOR_PAIR(5) );
	
	// Showing all current taxes in the bill
	print_in_middle(my_form_win, 13, 75, 90, "CURRENT TAXES",COLOR_PAIR(1) );
			
				
	for(i=76;i< 163 ;i++)
		mvwprintw(my_form_win, 11 , i , "_"); 					// Horizontal Partition
	refresh();
	
	/* Printing the labels of the fields */
	wattron(my_form_win , COLOR_PAIR(1) | A_BOLD);
	mvwprintw(my_form_win, STARTY + 2, STARTX - 20 , "Tax Name(1 word):");
	mvwprintw(my_form_win, STARTY + 5, STARTX - 20 , "Tax Rate (in %) :");
	wattroff(my_form_win , COLOR_PAIR(1) | A_BOLD);
	/* Loop through to get user requests */
	while((ch = wgetch(my_form_win)) != KEY_F(1))
	{	switch(ch)
		{	case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			case KEY_UP:
				form_driver(my_form, REQ_PREV_FIELD);
				form_driver(my_form, REQ_END_LINE);
				break;
			default:
				form_driver(my_form, ch);
				break;
			case KEY_BACKSPACE :
				form_driver(my_form,REQ_DEL_PREV);
				break;
			case 10 :
			wmove(my_form_win, 17,STARTX);
			wclrtoeol(my_form_win);	
			if(current_field(my_form)==field[2] ) {
				
					tvalue[0]=field_buffer(field[0], 0);
					tvalue[1]=field_buffer(field[1], 0);
					
					t=tax_store(tvalue);
					
					if (t == 0) {
						wattron(my_form_win , COLOR_PAIR(1));
						wmove(my_form_win,STARTY + 6, STARTX + 2 );	//Clear the error message	
						wclrtoeol(my_form_win);
						mvwprintw(my_form_win, 17, STARTX, "Data saved successfully.");
						wattroff(my_form_win , COLOR_PAIR(1));
						tax_print(my_form_win);				//Printing current taxes
					}
					else {
						wattron(my_form_win , COLOR_PAIR(15));
						mvwprintw(my_form_win,STARTY + 6, STARTX + 2, "Error in the above field");
						wattroff(my_form_win , COLOR_PAIR(15));	
					}
						
			}
			else if(current_field(my_form)==field[3] ) {
				
					wattron(my_form_win , COLOR_PAIR(1));
					mvwprintw(my_form_win, 17, STARTX, "Data updated successfully.");	
					wattroff(my_form_win , COLOR_PAIR(1));	
					for(i = 0; i < 6; i++) {		// clearing fields once submitted
						form_driver(my_form,REQ_NEXT_FIELD);
						form_driver(my_form,REQ_END_LINE);
						form_driver(my_form,REQ_CLR_FIELD);
					}
			}
			else if(current_field(my_form)==field[4] ) {
				
					// Add standard taxes to next 'n' structures here
					wattron(my_form_win , COLOR_PAIR(1));
					mvwprintw(my_form_win, 17, STARTX, "Taxes successfully included.");	
					wattroff(my_form_win , COLOR_PAIR(1));	
					
			}
			
			
			break;	
		}
	}
	unpost_form(my_form);
	free_form(my_form);
	for(i = 0; i < 5; i++)
		free_field(field[i]); 
	endwin();
	return;
}
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
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


