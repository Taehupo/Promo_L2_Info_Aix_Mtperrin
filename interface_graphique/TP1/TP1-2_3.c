#include <gtk/gtk.h>

#ifndef MYDATA_MAGIC
#define MYDATA_MAGIC 0x46EA7E05 // IT'S DA MAGIC NUMBAAAAAAAAAAAAAAAAAAAAH
#endif

#include "ex1.h"

typedef unsigned int uint;

void init_mydata(Mydata * my)
{
	my->title="TP1 en GTK+ 3.0";	// Initialise le titre;
	my->win_width = 400;			// Initialise la largeur par defaut
	my->win_height = 300;			// Initialise la hauteur par defaut
	my->magic = MYDATA_MAGIC;		// Initialise le MagicNumber
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

void on_button1_clicked (GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);							// Récupération des données
	printf ("Button clicked in window ’%s’\n", my->title);	// Affichage de debug
}

void on_b_quit_clicked (GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);			// récupération des données
	printf ("Closing window and app\n");	// affichage de debug
	gtk_widget_destroy(my->window);			// Kill la fenètre ET l'application
}

void on_app_activate (GtkApplication * app, gpointer user_data)
{
	Mydata * my = get_mydata(user_data);													// Récupération des données
	GtkWidget * button_box, * button1, * b_quit;											// Création de la buttonbox & boutons

	my->window = gtk_application_window_new (app); 											// crée la fenètre de l'appli

	gtk_window_set_title (GTK_WINDOW (my->window), my->title); 								// set le titre
	gtk_window_set_default_size (GTK_WINDOW (my->window), my->win_width, my->win_height);	// set la taille de la fenètre

	button_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);								// Orientation de la buttonbox + création de la buttonbox
	gtk_container_add (GTK_CONTAINER (my->window), button_box);								// Ajout  de la buttonbox à la fenetre

	button1 = gtk_button_new_with_label ("Click me");										// Instancie puis donne son label au button1
	gtk_box_pack_start (GTK_BOX (button_box), button1, TRUE, TRUE, 0);								// Ajoute le button1 dans la buttonbox
	g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), my);				// Connection du signal de button1 quand cliqué à la fonction correspondante

	b_quit = gtk_button_new_with_label ("Quit");											// Instancie puis donne son label au b_quit
	gtk_box_pack_start (GTK_BOX (button_box), b_quit, TRUE, TRUE, 0);									// Ajoute le b_quit dans la buttonbox
	g_signal_connect (b_quit, "clicked", G_CALLBACK(on_b_quit_clicked), my);				// Connection du signal de b_quit quand cliqué à la fonction correspondante

	gtk_widget_show_all (my->window);
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