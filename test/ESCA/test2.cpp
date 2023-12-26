// тестирование сокетов и файлов
// всего 4 утечки
// cppcheck - 1 верная
// PVS - 1 верная
// ESCA - 4 верных, 0 ложных
// note: currently doesn't work
// RUN: %verif %s -defects-out=%t.txt
// RUN: cat %t.txt | %FileCheck %s
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <fcntl.h>

// CHECK-NOT: Found {{.*}} problems

void error( const char *msg ) {
    perror(msg);
    exit(1);
}

int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return sockfd;
}

void test_1() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr{}, cli_addr{};
    sockfd = create_socket();
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 1111;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    send(newsockfd, "Hello, world!\n", 13, 0);
    bzero(buffer, 256);
    read(newsockfd, buffer, 255);
    printf("Here is the message: %s\n", buffer);
//     Утечка сокетов
// cppcheck - | ESCA + | PVS -
//    close(newsockfd);
//    close(sockfd);
}

// cppcheck - | ESCA + | PVS +
void test_2() {
    std::string filename = "fn";
    int f;
    for( int i = 0; i < 1500; ++i ) {
        f = open((filename + std::to_string(i)).c_str(), O_RDWR);
        // работа с файлом
        write(f, "id", 2);
        // забываем закрыть
    }
}

FILE *createFile( const std::string &fn ) {
    auto f = fopen(fn.c_str(), "w");
    return f;
}

// cppcheck + | ESCA + | PVS -
void test_2_2() {
    auto f = createFile("some Name");
    if( f ) {
        fprintf(f, "%d", 10);
    }
}

int main() {
    test_1();
    test_2();
    test_2_2();
    return 0;
}
