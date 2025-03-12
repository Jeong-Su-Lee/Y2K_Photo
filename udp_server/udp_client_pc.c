// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <time.h>

// #include "define.h"

// #define MAX_BUF 256

// pid_t pid;

// void start_timer(int seconds, void (*callback)(int, struct sockaddr_in*, socklen_t));

// void send_delayed_message(int sfd, struct sockaddr_in *client_addr, socklen_t addr_len) {
//     const char *msg = "capture"; // 클라이언트가 받아서 저장할 trigger
//     sendto(sfd, msg, strlen(msg), 0, (struct sockaddr *)client_addr, addr_len);
//     printf("[%d] timer expired, message sent: %s\n", pid, msg);
// }

// int main(int argc, char **argv)
// {
//     int ret, sfd;
//     struct sockaddr_in addr_server, addr_client;
//     socklen_t addr_client_len;
//     // char buf[MAX_BUF];

//     if(argc != 1) {
//         printf("usage: %s\n", argv[0]);
//         return EXIT_FAILURE;
//     }
//     printf("[%d] running %s\n", pid = getpid(), argv[0]);

//     sfd = socket(AF_INET, SOCK_DGRAM, 0);
//     if(sfd == -1) {
//         printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
//         return EXIT_FAILURE;
//     }

//     memset(&addr_server, 0, sizeof(addr_server));
//     addr_server.sin_family = AF_INET;
//     addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
//     addr_server.sin_port = htons(SERVER_PORT);
//     ret = bind(sfd, (struct sockaddr *)&addr_server, sizeof(addr_server));
//     if(ret == -1) {
//         printf("[%d] error: %s (%d)\n", pid, strerror(errno), __LINE__);
//         return EXIT_FAILURE;
//     }
// 	memset(&addr_client, 0, sizeof(addr_client));
// 	addr_client.sin_family = AF_INET;
// 	addr_client.sin_addr.s_addr = inet_addr(CLIENT_IP);
// 	addr_client.sin_port = htons(SERVER_PORT);
//     while (1) {
//         addr_client_len = sizeof(addr_client);
//         // len = recvfrom(sfd, buf, MAX_BUF-1, 0, (struct sockaddr *)&addr_client, &addr_client_len);
// 		sleep(3);
// 		send_delayed_message(sfd, &addr_client, addr_client_len);
//         // if(len > 0) {
//         //     buf[len] = 0;
//         //     printf("[%d] received: %s\n", pid, buf);

//         //     // echo back
//         //     sendto(sfd, buf, len, 0, (struct sockaddr *)&addr_client, sizeof(addr_client));

//         //     // 타이머 시작: 3초 후 클라이언트에 메시지 전송
//         //     printf("[%d] starting 3-second timer to send capture signal...\n", pid);
//         //     sleep(seconds);
//         //     send_delayed_message(sfd, &addr_client, addr_client_len);
//         // }
//     }

//     close(sfd);
//     printf("[%d] closed\n", pid);
//     printf("[%d] terminated\n", pid);
//     return EXIT_SUCCESS;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 25000
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // UDP 소켓 생성
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("소켓 생성 실패");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // 바인드
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("바인드 실패");
        close(sockfd);
        exit(1);
    }

    printf("서버 실행 중... 클라이언트 메시지 대기 중\n");

    // 클라이언트 정보 저장용
    struct sockaddr_in saved_client_addr;
    int client_found = 0;

    // 이미지 저장용
    FILE *fp = NULL;

    while (1) {
        ssize_t recv_len = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                                    (struct sockaddr*)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom 실패");
            break;
        }

        buffer[recv_len] = '\0';

        // "hello" 메시지를 받으면 클라이언트 정보 저장
        if (strcmp(buffer, "hello") == 0) {
            saved_client_addr = client_addr;
            client_found = 1;

            printf("클라이언트 연결됨: %s:%d\n",
                   inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port));
			saved_client_addr.sin_port = htons(SERVER_PORT);

            // 약간의 지연 후 capture 메시지 전송
            sleep(1);
            const char *capture_msg = "capture";
            sendto(sockfd, capture_msg, strlen(capture_msg), 0,
                   (struct sockaddr*)&saved_client_addr, sizeof(saved_client_addr));
            printf("클라이언트에 'capture' 명령 전송\n");

            // 새 파일 열기 (기존 내용 덮어쓰기)
            fp = fopen("received.jpg", "wb");
            if (!fp) {
                perror("파일 열기 실패");
                break;
            }

        } else {
            // 이미지 데이터 수신 중이라면 저장
            if (fp && client_found) {
                fwrite(buffer, 1, recv_len, fp);

                // 예시로, 1초 이상 무응답이면 저장 종료하거나
                // 또는 패킷 끝에 "EOF" 전송하는 방식도 고려 가능
            }
        }
    }

    if (fp) fclose(fp);
    close(sockfd);
    return 0;
}
