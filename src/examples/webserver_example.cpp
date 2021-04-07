#include "ld_webserver.h"

int main() {
    ld_webserver::Webserver webserver{5000};

    webserver.StartServer();

    return 0;
}