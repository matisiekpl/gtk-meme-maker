#include <iostream>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    auto main = MainWindow::instance();
    main.run();
    return 0;
}
