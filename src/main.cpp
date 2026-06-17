#include "server.h"

int main() {
    Server server(6379);
    return server.start() ? 0 : 1;
}
