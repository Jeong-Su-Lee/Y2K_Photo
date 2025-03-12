#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 25000
#define BUF_SIZE 1024

void send_capture_message(int sockfd, struct sockaddr_in client_addr)
{
    printf("클라이언트에 'capture' 명령 전송\n");
    const char *capture_msg = "capture";
    sendto(sockfd, capture_msg, strlen(capture_msg), 0,
           (struct sockaddr*)&client_addr, sizeof(client_addr));
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    char filename[100];
    FILE *fp = NULL;
    int receiving_image = 0;

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

    while (1) {
        ssize_t recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                                    (struct sockaddr*)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom 실패");
            break;
        }

        // "hello" 수신 시 클라이언트 정보 저장 후 capture 명령 전송
        if (recv_len >= 5 && strncmp(buffer, "hello", 5) == 0) {
            saved_client_addr = client_addr;
            client_found = 1;
            printf("클라이언트 연결됨: %s:%d\n",
                   inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port));
            saved_client_addr.sin_port = htons(SERVER_PORT);

            sleep(1);  // 약간의 지연 후
            send_capture_message(sockfd, saved_client_addr);
            continue;
        }

        // "EOF" 수신 시 이미지 저장 종료
        if (recv_len >= 3 && strncmp(buffer, "EOF", 3) == 0) {
            printf("이미지 수신 완료!\n");
            if (fp) {
                fclose(fp);
                fp = NULL;
            }
            receiving_image = 0;
            sleep(3);  // 약간 텀을 주고
            send_capture_message(sockfd, saved_client_addr);
            continue;
        }

        // 이미지 데이터 수신
        if (client_found) {
            if (!fp && !receiving_image) {
                sprintf(filename, "image_%ld.jpg", time(NULL));
                fp = fopen(filename, "wb");
                if (!fp) {
                    perror("파일 열기 실패");
                    break;
                }
                printf("이미지 저장 시작: %s\n", filename);
                receiving_image = 1;
            }

            if (fp) {
                fwrite(buffer, 1, recv_len, fp);
            }
        }
    }

    if (fp) fclose(fp);
    close(sockfd);
    return 0;
}
