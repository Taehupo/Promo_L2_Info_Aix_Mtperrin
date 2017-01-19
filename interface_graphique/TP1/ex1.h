#ifndef EX1_H
#define EX1_H

typedef unsigned int uint;

typedef struct
{
	GtkWidget * window;

	char * title;
	char * current_folder;

	gint win_width;
	gint win_height;

	GtkWidget * image;
	GtkWidget * status;

	GdkPixbuf * pixbuf1;

	uint magic;
	
} Mydata;

#endif