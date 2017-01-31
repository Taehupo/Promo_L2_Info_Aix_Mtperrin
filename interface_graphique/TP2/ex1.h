#ifndef EX1_H
#define EX1_H

typedef unsigned int uint;

typedef struct
{
	GtkWidget * window;
	GtkWidget * win_scale;

	char * title;
	char * current_folder;

	gint win_width;
	gint win_height;

	GtkWidget * vbox1;

	GtkWidget * image;
	GtkWidget * status;

	GdkPixbuf * pixbuf1;

	uint magic;
	
} Mydata;

#endif