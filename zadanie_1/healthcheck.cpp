#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

int main() {
    const char* portEnv = std::getenv("APP_PORT");
    int port = portEnv ? std::atoi(portEnv) : 8080;
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) return 1;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(static_cast<uint16_t>(port));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Próba połączenia z serwerem
    if (connect(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == 0) {
        close(s);
        return 0; // Sukces - serwer działa
    }

    close(s);
    return 1; // Błąd - serwer nie odpowiada
}