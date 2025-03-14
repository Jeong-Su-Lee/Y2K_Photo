#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <jpeglib.h>
#include "define.h"



#define SERVER_PORT 25000
#define BUF_SIZE 1024
#define MAX_CLIENTS 10


typedef struct {
    struct sockaddr_in addr;
    int active;
} Client;

struct ThreadArgs {
    int sockfd;
    Client clients[MAX_CLIENTS];
};

void broadcast_message(int sockfd, Client clients[], const char *msg);

void* time_sender_thread(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int sockfd = args->sockfd;
    Client *clients = args->clients; 
    int count = 8;

    char time_buf[100];
    while (1) {
        snprintf(time_buf, sizeof(time_buf), "%d", count);
        printf("Count 전송 : %d\n", count);
        broadcast_message(sockfd, clients, time_buf);
               
        if (count == 0) {
                broadcast_message(sockfd, clients, "capture");
                count = 9;
            }
        sleep(1); // 1초마다 전송
        count -= 1;
    }
    return NULL;
}

// void send_capture_message (int sockfd, struct sockaddr_in client_addr)
// {
//     printf("클라이언트에 'capture' 명령 전송\n");
//     printf("전송하는 클라이언트 정보 : %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
//     const char *capture_msg = "capture";
//     sendto(sockfd, capture_msg, strlen(capture_msg), 0,
//            (struct sockaddr*)&client_addr, sizeof(client_addr));
// }

void send_message (int sockfd, struct sockaddr_in client_addr, const char *msg)
{
    printf("전송하는 클라이언트 정보 : %s:%d msg : %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), msg);
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
}

void broadcast_message(int sockfd, Client clients[], const char *msg) {
    for (int i = 0; i < 2; i++) {
        if (clients[i].active) {
            sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
}
int find_client(Client clients[], struct sockaddr_in *client_addr)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active &&
            clients[i].addr.sin_addr.s_addr == client_addr->sin_addr.s_addr &&
            clients[i].addr.sin_port == client_addr->sin_port) {
            return i; // 클라이언트 인덱스 반환
        }
    }
    return -1; // 클라이언트를 찾지 못함
}
int add_client(Client clients[], struct sockaddr_in *client_addr)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) { // 비활성 클라이언트 슬롯 사용
            clients[i].addr = *client_addr;
            clients[i].active = 1;
            printf("새 클라이언트 추가: %s[%d]:%d\n",
                   inet_ntoa(client_addr->sin_addr),i, ntohs(client_addr->sin_port));
            return i; // 새 클라이언트 인덱스 반환
        }
    }
    printf("클라이언트 추가 실패: 최대 클라이언트 수 초과\n");
    return -1;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    char filename[100];
    FILE *fp = NULL;
    int receiving_image = 0;
    Client clients[MAX_CLIENTS] = {0,};
    int flag = -1; // DEBUG 용 추후 수정
    int count = 0; // DEBUG 용 추후 수정
    char msg[MAX_MSG_LEN] = {0,};

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

    int client_index = -1;
    while (1) {
        ssize_t recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0,
                                    (struct sockaddr*)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom 실패");
            break;
        }
        buffer[recv_len] = '\0'; //메시지 끝 처리 
        client_index = find_client(clients, &client_addr);
        // "hello" 수신 시 클라이언트 정보 저장 후 capture 명령 전송
        if (recv_len >= 4 && strncmp(buffer, "CONN", 4) == 0) {
            if(client_index == -1){
                add_client(clients,&client_addr); //클라이언트 추가   
                client_index = find_client(clients, &client_addr);
                sprintf(msg, "CLI%d", client_index + 1);
                clients[client_index].addr.sin_port = htons(SERVER_PORT);
                sleep(1); // 충분한 설정시간 줌
                send_message(sockfd, clients[client_index].addr, msg);

                printf("클라이언트 추가됨: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
            // client_found = 1;
            
            pthread_t tid;
            struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
            args->sockfd = sockfd;
            memcpy(args->clients, clients, sizeof(Client) * MAX_CLIENTS);
            // sleep(1);  // 약간의 지연 후
            if(client_index == 1){ //연결이 둘다 됐다면 둘다 전송 
                flag = 1; // DEBUG 용 추후 수정
                if (pthread_create(&tid, NULL, time_sender_thread, args) != 0)
                {
                    perror("타임 스레드 생성 실패");
                    free(args);
                } 
                else
                {
                    pthread_detach(tid); // 메모리 자동 회수
                }
                // broadcast_message(sockfd, clients, "capture");
            }
            continue;
        }
        // "EOF" 수신 시 이미지 저장 종료
        if (recv_len >= 3 && strncmp(buffer, "EOF", 3) == 0) {
            printf("이미지 수신 완료!\n");
            count ++; // DEBUG 용 추후 수정
            if (fp) {
                fclose(fp);
                fp = NULL;
            }
            receiving_image = 0;
            // sleep(3);  // 약간 텀을 주고
            // broadcast_message(sockfd, clients, "capture");
            continue;
        }

        // 이미지 데이터 수신
        // client_index = find_client(clients, &client_addr); DEBUG
        // printf("[DEBUG] :  %d\n", client_index);
        if (flag == 1) {    // 추후 수정
            if (!fp && !receiving_image) {
                sprintf(filename, "image_%d.jpg", count); // DEBUG 용 추후 수정
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
