//
// Created by Mateusz Woźniak for Programming 2 course (ISI AGH)
//

#ifndef MEME_MAKER_MAINWINDOW_H
#define MEME_MAKER_MAINWINDOW_H

#include <gtk/gtk.h>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>


class MainWindow {
private:
    GtkWidget *window;
    GtkWidget *container;
    GtkWidget *first_text_entry;
    GtkWidget *second_text_entry;
    GtkWidget *meme_preview;
    GtkWidget *choose_image_button;
    GtkWidget *save_button;
    std::string filename;
    cv::Mat image;
    bool is_image_loaded = false;
    int meme_size;

private:
    void setup_window();

    void create_widgets();

    void load_image(std::string new_filename);

    void save_meme(std::string filename);

    static void choose_file(GtkWidget *widget, gpointer window) {
        (void) widget; // Eliminates "error: unused parameter ‘widget’"
        auto dialog = gtk_file_chooser_dialog_new("Otwórz", GTK_WINDOW(window),
                                                  GTK_FILE_CHOOSER_ACTION_OPEN,
                                                  GTK_STOCK_CANCEL,
                                                  GTK_RESPONSE_CANCEL,
                                                  GTK_STOCK_OPEN,
                                                  GTK_RESPONSE_ACCEPT, NULL);
        if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
            std::string filename;
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
            MainWindow::instance().load_image(filename);
        }
        gtk_widget_destroy(dialog);
    }

    static void open_save_image_dialog(GtkWidget *widget, gpointer window) {
        (void) widget; // Eliminates "error: unused parameter ‘widget’"
        if (!MainWindow::instance().is_image_loaded) return;
        auto dialog = gtk_file_chooser_dialog_new("Zapisz", GTK_WINDOW(window),
                                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                                  GTK_STOCK_CANCEL,
                                                  GTK_RESPONSE_CANCEL,
                                                  GTK_STOCK_SAVE,
                                                  GTK_RESPONSE_ACCEPT, NULL);
        if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
            std::string filename;
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
            MainWindow::instance().save_meme(filename);
        }
        gtk_widget_destroy(dialog);
    }

    static void rerender() {
        MainWindow::instance().render_image();
    }


public:
    cv::Mat render_image();

    void run();

    static MainWindow &instance() {
        static MainWindow INSTANCE;
        return INSTANCE;
    }

    MainWindow() {
        setup_window();
        create_widgets();
    }

    static const int MEME_SIZE = 380;
};


#endif //MEME_MAKER_MAINWINDOW_H
