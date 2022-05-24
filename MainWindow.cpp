//
// Created by matisiekpl on 23.05.22.
//

#include "MainWindow.h"

void MainWindow::setup_window() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Kreator memów");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), false);
    gtk_widget_set_size_request(GTK_WIDGET(window), 400, 570);

    container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);
}

void MainWindow::create_widgets() {
    first_text_entry = gtk_entry_new();
    gtk_widget_set_size_request(first_text_entry, 380, 34);
    gtk_fixed_put(GTK_FIXED(container), first_text_entry, 10, 10);
    g_signal_connect(G_OBJECT(first_text_entry), "changed", G_CALLBACK(rerender), (gpointer) window);

    choose_image_button = gtk_button_new_with_label("Wybierz obrazek");
    gtk_widget_set_size_request(choose_image_button, 380, 380);
    gtk_fixed_put(GTK_FIXED(container), choose_image_button, 10, 54);
    g_signal_connect(G_OBJECT(choose_image_button), "clicked", G_CALLBACK(choose_file), (gpointer) window);

    second_text_entry = gtk_entry_new();
    gtk_widget_set_size_request(second_text_entry, 380, 34);
    gtk_fixed_put(GTK_FIXED(container), second_text_entry, 10, 444);
    g_signal_connect(G_OBJECT(second_text_entry), "changed", G_CALLBACK(rerender), (gpointer) window);

    save_button = gtk_button_new_with_label("Zapisz do pliku");
    gtk_widget_set_size_request(save_button, 380, 70);
    gtk_fixed_put(GTK_FIXED(container), save_button, 10, 488);
    g_signal_connect(G_OBJECT(save_button), "clicked", G_CALLBACK(open_save_image_dialog), (gpointer) window);
}

void MainWindow::load_image(std::string new_filename) {
    filename = new_filename;
    std::cout << "Loading file: " << filename << std::endl;
    gtk_widget_destroy(choose_image_button);

    cv::Mat loaded_image = cv::imread(filename);

    int width = loaded_image.rows;
    int height = loaded_image.cols;
    std::cout << "aa" << width;
    int max_size = width < height ? width : height;
    std::cout << "aa" << loaded_image.cols;
    meme_size = max_size - 10;

    int offset_width = (loaded_image.cols - max_size) / 2;
    int offset_height = (loaded_image.rows - max_size) / 2;
    cv::Rect roi(offset_width, offset_height, max_size, max_size);
    image = loaded_image(roi).clone();

    is_image_loaded = true;
    render_image();
    render_image();
}

void MainWindow::save_meme(std::string filename) {
    cv::imwrite(filename + ".png", render_image());
}

cv::Mat MainWindow::render_image() {
    if (!MainWindow::instance().is_image_loaded) return {};
    auto temp_image = image.clone();

    int text_size = meme_size / 400;

    std::string first_text = gtk_entry_get_text(GTK_ENTRY(first_text_entry));
    double first_text_scale = (double) first_text.length() / 70.0;
    cv::putText(temp_image, first_text, cv::Point((meme_size / 2) - meme_size * first_text_scale, text_size * 50),
                cv::FONT_HERSHEY_TRIPLEX, text_size,
                cv::Scalar(0, 0, 0), 2, false);

    std::string second_text = gtk_entry_get_text(GTK_ENTRY(second_text_entry));
    double second_text_scale = (double) second_text.length() / 70.0;
    cv::putText(temp_image, second_text,
                cv::Point((meme_size / 2) - meme_size * second_text_scale, meme_size - text_size * 50),
                cv::FONT_HERSHEY_TRIPLEX, text_size,
                cv::Scalar(0, 0, 0), 2, false);

    cv::Mat scaled_image;
    cv::resize(temp_image, scaled_image, cv::Size(MEME_SIZE, MEME_SIZE), cv::INTER_AREA);

    auto pixbuf = gdk_pixbuf_new_from_data(scaled_image.data, GDK_COLORSPACE_RGB, FALSE, 8, scaled_image.cols,
                                           scaled_image.rows,
                                           scaled_image.step,
                                           NULL, NULL);
    meme_preview = gtk_image_new_from_pixbuf(pixbuf);
    gtk_widget_set_size_request(meme_preview, 380, 380);
    gtk_fixed_put(GTK_FIXED(container), meme_preview, 10, 54);

    gtk_widget_show_all(window);
    return temp_image;
}

void MainWindow::run() {
    gtk_widget_show_all(window);
    gtk_main();
}