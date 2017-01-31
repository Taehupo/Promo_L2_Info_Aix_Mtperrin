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

void on_item_load_activate (GtkWidget * widget, gpointer data)
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

void on_item_quit_activate (GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);			// récupération des données
	printf ("Closing window and app\n");	// affichage de debug
	gtk_widget_destroy(my->window);			// Kill la fenètre ET l'application
}

void on_item_rotate_activate (GtkWidget * widget, gpointer data)
{
	//Mydata * my = get_mydata(data);
	//Dere should be moar things here, juste la flemme de finir la dernière question du TP1
	printf("On fait tourner les servietteuh !\n");
}

void on_item_about_activate(GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);

	char * auteurs[] = {"Gabriel Aubert <uzumakigip@gmail.com>", "Lucas Moragues <lucas.moragues@Wanadoo.fr>", NULL};

	gtk_show_about_dialog (NULL, "program-name", my->title, "version", "2.4",
		"website", "http://j.mp/optigra", "authors", auteurs, "logo-icon-name", "face-angel", NULL);
}

void on_item_bg_color_activate(GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);

	GtkWidget * colorChooser;
	GdkRGBA bg_color;

	gint res;

	char msg[200];

	colorChooser = gtk_color_chooser_dialog_new ("Background Color", NULL);

	res = gtk_dialog_run (GTK_DIALOG(colorChooser));

	if (res == GTK_RESPONSE_OK)
	{
		gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(colorChooser), &bg_color);
		g_sprintf(msg, "Selected bg color : %s", gdk_rgba_to_string(&bg_color));
		set_status(my, msg);
		gtk_widget_override_background_color(my->image, GTK_STATE_FLAG_NORMAL, &bg_color);
	}
	g_free(msg);
	gtk_widget_destroy (colorChooser);
}

void on_item_scale_activate(GtkWidget * widget, gpointer data)
{
	Mydata * my = get_mydata(data);

	gtk_window_present(GTK_WINDOW(my->win_scale));
}

void window_init(GtkApplication * app, gpointer data)
{
	Mydata * my = get_mydata(data);

	my->window = gtk_application_window_new (app); 											// crée la fenètre de l'appli

	gtk_window_set_title (GTK_WINDOW (my->window), my->title); 								// set le titre
	gtk_window_set_default_size (GTK_WINDOW (my->window), my->win_width, my->win_height);	// set la taille de la fenètre
}

void layout_init(gpointer data)
{
	Mydata * my = get_mydata(data);

	my->vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);			// Orientation de la box + Instantiation de la vbox
	gtk_container_add (GTK_CONTAINER (my->window), my->vbox1);		// Ajout  de la box à la fenetre
}

void menus_init(gpointer data)
{
	Mydata * my = get_mydata(data);
	GtkWidget * menu_bar, * item_file, * item_tool, * item_help, * sub_file, * sub_tools, * sub_help;
	GtkWidget * item_load, * item_quit, * item_rotate, * item_bgColor, * item_about, * item_scale;

	menu_bar = gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(my->vbox1), menu_bar, FALSE, TRUE, 0);

		item_file = gtk_menu_item_new_with_label("File");
		gtk_menu_shell_append(GTK_MENU_SHELL (menu_bar), item_file);

			sub_file = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM (item_file), sub_file);

				item_load = gtk_menu_item_new_with_label("Load");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_file), item_load);
				g_signal_connect(item_load, "activate", G_CALLBACK(on_item_load_activate), my);

				item_quit = gtk_menu_item_new_with_label("Quit");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_file), item_quit);
				g_signal_connect(item_quit, "activate", G_CALLBACK(on_item_quit_activate), my);

		item_tool = gtk_menu_item_new_with_label("Tools");
		gtk_menu_shell_append(GTK_MENU_SHELL (menu_bar), item_tool);

			sub_tools = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM (item_tool), sub_tools);

				item_rotate = gtk_menu_item_new_with_label("Rotate");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools), item_rotate);
				g_signal_connect(item_rotate, "activate", G_CALLBACK(on_item_rotate_activate), my);

				item_bgColor = gtk_menu_item_new_with_label("Bg Color");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools), item_bgColor);
				g_signal_connect(item_bgColor, "activate", G_CALLBACK(on_item_bg_color_activate), my);

				item_scale = gtk_menu_item_new_with_label("Scale");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_tools), item_scale);
				g_signal_connect(item_scale, "activate", G_CALLBACK(on_item_scale_activate), my);

		item_help = gtk_menu_item_new_with_label("Help");
		gtk_menu_shell_append(GTK_MENU_SHELL (menu_bar), item_help);

			sub_help = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM (item_help), sub_help);

				item_about = gtk_menu_item_new_with_label("About");
				gtk_menu_shell_append(GTK_MENU_SHELL (sub_help), item_about);
				g_signal_connect(item_about,"activate", G_CALLBACK(on_item_about_activate), my);

	
	g_object_set (gtk_settings_get_default(), "gtk-shell-shows-menubar", FALSE, NULL);					//Affiche la menubar pas comme un attardé
}

void image_init(gpointer data)
{
	Mydata * my = get_mydata(data);

	GtkWidget * scroll;

	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start(GTK_BOX(my->vbox1), scroll, TRUE, TRUE, 0);

		my->image = gtk_image_new ();									// Instancie l'image
		gtk_container_add (GTK_CONTAINER (scroll), my->image);			// Ajoute l'image au scroll
}

void status_init(gpointer data)
{
	Mydata * my = get_mydata(data);

	my->status = gtk_statusbar_new ();										// Instancie la status bar
	gtk_box_pack_start (GTK_BOX (my->vbox1), my->status, FALSE, FALSE, 0);	// Ajoute la status bar à la vbox
	set_status (my, "Welcome in TP1");										// Défini le status
}

void win_scale_init(gpointer data)
{
	Mydata * my = get_mydata(data);

	my->win_scale = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW (my->win_scale), "Image Scale");

	gtk_widget_show_all(my->win_scale);

	gtk_widget_hide(my->win_scale);
}

void on_app_activate (GtkApplication * app, gpointer user_data)
{
	Mydata * my = get_mydata(user_data);	// Récupération des données

	window_init(app, my);					// Paramétrage de la fenètre

	layout_init(my);						// Paramétrage de l'affichage dans la fenètre

	menus_init(my);							// Paramétrage des menus

	image_init(my);							// Paramétrage de l'image
	
	status_init(my);						// Paramétrage du status

	win_scale_init(my);						// Paramétrage de la fenètre de mise à l'échelle
	
	gtk_widget_show_all (my->window);		// Affiche tout les widget instancié
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