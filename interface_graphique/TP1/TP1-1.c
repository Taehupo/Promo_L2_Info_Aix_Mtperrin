#include <gtk/gtk.h>

typedef struct
{
	GtkWidget *window;
	char *title;
	gint win_width;
	gint win_height;
} Mydata;

void init_mydata(Mydata *my)
{
	my->title="TP1 en GTK+ 3.0";
	my->win_width = 400;
	my->win_height = 300;
}

void on_button1_clicked (GtkWidget *widget, gpointer data)
{
	Mydata *my = data;
	printf ("Button clicked in window ’%s’\n", my->title);
}

void on_b_quit_clicked (GtkWidget *widget, gpointer data)
{
	Mydata *my = data;
	printf ("Closing window and app\n");
	gtk_widget_destroy(my->window);
}

void on_app_activate (GtkApplication* app, gpointer user_data)
{
	Mydata *my = user_data;
	GtkWidget *button_box, *button1, *b_quit;

	my->window = gtk_application_window_new (app);

	gtk_window_set_title (GTK_WINDOW (my->window), my->title);
	gtk_window_set_default_size (GTK_WINDOW (my->window), my->win_width, my->win_height);

	button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add (GTK_CONTAINER (my->window), button_box);

	button1 = gtk_button_new_with_label ("Click me");
	gtk_container_add (GTK_CONTAINER (button_box), button1);
	g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), my);

	b_quit = gtk_button_new_with_label ("Quit");
	gtk_container_add (GTK_CONTAINER (button_box), b_quit);
	g_signal_connect (b_quit, "clicked", G_CALLBACK(on_b_quit_clicked), my);

	gtk_widget_show_all (my->window);
}

int main (int argc, char *argv[])
{
	Mydata my;
	init_mydata(&my);

	GtkApplication *app;
	int status;

	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK(on_app_activate), &my);

	status = g_application_run (G_APPLICATION(app), argc, argv);
	g_object_unref (app);

	return status;
}