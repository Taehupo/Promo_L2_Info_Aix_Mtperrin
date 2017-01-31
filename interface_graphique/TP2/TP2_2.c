#include <gtk/gtk.h>
#include <glib/gprintf.h>

#ifndef MYDATA_MAGIC
#define MYDATA_MAGIC 0x46EA7E05 // IT'S DA MAGIC NUMBAAAAAAAAAAAAAAAAAAAAH
#endif

#include "ex1.h"

typedef unsigned int uint;

void init_mydata(Mydata * my)
{
	my->title = "TP1 en GTK+ 3.0";	// Initialise le titre; 
	my->current_folder = NULL;
	my->win_width = 400;			// Initialise la largeur par defaut
	my->win_height = 300;			// Initialise la hauteur par defaut
	my->magic = MYDATA_MAGIC;		// Initialise le MagicNumber
	my->pixbuf1 = NULL;
}

// Cette fonction permet de tester si le data que l’on a recuperé dans
// une callback contient bien my ; sinon, cela veut dire que:
Mydata * get_mydata (gpointer data)
{
	if (data == NULL) 										// - soit on a oublié de transmettre my dans g_signal_connect,
	{
		fprintf (stderr, "get_mydata: NULL data\n");		// Message d'erreur envoyé sur la sortie d'erreur
		return NULL;
	}
	if (((Mydata *)data)->magic != MYDATA_MAGIC) 			// - soit on s’est trompé dans le nombre de paramètres de la callback.
	{
		fprintf (stderr, "get_mydata: bad magic number\n"); // message d'erreur envoyé sur la sortie d'erreur
		return NULL;
	}
	return data;
}

void set_status (Mydata * my, const char * msg)
{
	gtk_statusbar_pop ( (GtkStatusbar *) my->status, 0);
	gtk_statusbar_push ( (GtkStatusbar *) my->status, 0, msg);
}

void on_button1_clicked (GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);																							// Récupération des données
	//début du sélecteur de fichier
	GtkWidget * dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Choisissez un fichier",(GtkWindow *) my->window, 
		action, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);

	if (my->current_folder != NULL)
	{
		gtk_file_chooser_set_current_folder ( (GtkFileChooser *)dialog, my->current_folder);
	}

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if(res == GTK_RESPONSE_ACCEPT)
	{
		char * filename;
		GtkFileChooser * chooser = GTK_FILE_CHOOSER (dialog);
		filename = gtk_file_chooser_get_filename (chooser);
		set_status(my, "Loading Image...");
		g_clear_object(&my->pixbuf1);
		my->pixbuf1 = gdk_pixbuf_new_from_file (filename, NULL);														// Chargement de l'image
		if(my->pixbuf1 == NULL)
		{
			set_status(my, "Loading Failed : not an image");																// Actualisation du status
			gtk_image_set_from_icon_name ( (GtkImage *)my->image,"image-missing", GTK_ICON_SIZE_DIALOG);
		}
		else
		{
			char msg[200];			
			g_sprintf(msg, "Loading successful : %dx%d", gdk_pixbuf_get_width(my->pixbuf1), gdk_pixbuf_get_height(my->pixbuf1));		// création du message de status
			set_status(my, msg);																							// Actualisation du status
			gtk_image_set_from_pixbuf ( (GtkImage *)my->image, my->pixbuf1);												// Chargement de l'image & Affichage																							// Destruction de Pixbuf
		}
		g_free(my->current_folder);
		my->current_folder = gtk_file_chooser_get_current_folder (chooser);
		g_free (filename);
	}
	gtk_widget_destroy (dialog);
	//Fin du sélecteur de fichier
	printf ("Loading image in ’%s’\n",my->title);																			// Affichage de debug
}

void on_b_quit_clicked (GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);			// récupération des données
	printf ("Closing window and app\n");	// affichage de debug
	gtk_widget_destroy(my->window);			// Kill la fenètre ET l'application
}

void on_b_rotate_clicked(GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);
	//Dere should be moar things here, juste la flemme de finir la dernière question du TP1
}

void on_app_activate (GtkApplication * app, gpointer user_data)
{
	Mydata * my = get_mydata(user_data);													// Récupération des données
	GtkWidget * vbox1, * hbox1, * button1, * b_quit, * b_rotate, * scroll;					// Création de la box & boutons

	my->window = gtk_application_window_new (app); 											// crée la fenètre de l'appli

	gtk_window_set_title (GTK_WINDOW (my->window), my->title); 								// set le titre
	gtk_window_set_default_size (GTK_WINDOW (my->window), my->win_width, my->win_height);	// set la taille de la fenètre

	vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);										// Orientation de la box + Instantiation de la vbox
	gtk_container_add (GTK_CONTAINER (my->window), vbox1);									// Ajout  de la box à la fenetre

	hbox1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);									// Orientation de la box + Instantiation de la box
	gtk_box_pack_start(GTK_BOX (vbox1), hbox1, FALSE, TRUE, 0);								// Ajout de la box à la fenetre

		button1 = gtk_button_new_with_label ("Load me Senpai !");							// Instancie puis donne son label au button1
		gtk_box_pack_start (GTK_BOX (hbox1), button1, TRUE, TRUE, 0);						// Ajoute le button1 dans la hbox
		g_signal_connect (button1, "clicked", G_CALLBACK(on_button1_clicked), my);			// Connection du signal de button1 quand cliqué à la fonction correspondante

		b_rotate = gtk_button_new_with_label("Rotate");										// Pareil que les autres la, c'est bon vous devriez comprendre depuis que je le marque
		gtk_box_pack_start (GTK_BOX (hbox1), b_rotate, TRUE, TRUE, 0);
		g_signal_connect (b_rotate, "clicked", G_CALLBACK(on_b_rotate_clicked), my);

		b_quit = gtk_button_new_with_label ("Quit");										// Instancie puis donne son label au b_quit
		gtk_box_pack_start (GTK_BOX (hbox1), b_quit, FALSE, FALSE, 0);						// Ajoute le b_quit dans la hbox
		g_signal_connect (b_quit, "clicked", G_CALLBACK(on_b_quit_clicked), my);			// Connection du signal de b_quit quand cliqué à la fonction correspondante

	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start(GTK_BOX(vbox1), scroll, TRUE, TRUE, 0);

	my->image = gtk_image_new ();															// Instancie l'image
	gtk_container_add (GTK_CONTAINER (scroll), my->image);									// Ajoute l'image à la vbox

	my->status = gtk_statusbar_new ();														// Instancie la status bar
	gtk_box_pack_start (GTK_BOX (vbox1), my->status, FALSE, FALSE, 0);						// Ajoute la status bar à la vbox
	set_status (my, "Welcome in TP1");														// Défini le status

	gtk_widget_show_all (my->window);														// Affiche tout les widget instancié
}

int main (int argc, char * argv[])
{
	Mydata my;																// Création de la struct my
	init_mydata(&my);														// Initialisation de my

	GtkApplication * app;													// Création de l'application
	int status;																// création de status

	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);				// Instantiation de l'application
	g_signal_connect (app, "activate", G_CALLBACK(on_app_activate), &my);	// Connection du signal au startup de l'appli

	status = g_application_run (G_APPLICATION(app), argc, argv);			// Démarrage de l'appli
	g_object_unref (app);													// Un fois applis fermer, on la détruit

	return status;															// On renvois le statu de sortie de l'appli
}