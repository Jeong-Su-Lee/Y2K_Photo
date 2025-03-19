#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <jpeglib.h>
#include <errno.h>
#include "define.h"



#define SERVER_PORT 25000
#define BUF_SIZE 32768
#define MAX_CLIENTS 10
#define FINAL_HEADER "FINIMG"
#define FINAL_HEADER_LEN 6 
#define msleep(x) usleep((x) * 1000)

int shooting_count = 0;


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
    shooting_count = 0;

    char time_buf[100];
    while (1) {
        snprintf(time_buf, sizeof(time_buf), "CNT%d", count);
        printf("Count 전송 : %s\n", time_buf);
        broadcast_message(sockfd, clients, time_buf);
        
        if (count == 0)
        {
            broadcast_message(sockfd, clients, "CAPT");
            count = 9;
            shooting_count += 1;
        }
        if (shooting_count == 8)
        {
            break;
        }
        sleep(1); // 1초마다 전송
        count -= 1;
    }
    return NULL;
}

void broadcast_message_with_file(int sockfd, Client clients[], const char *msg, const char *file_path) 
{
    char buffer[BUF_SIZE];
    unsigned char send_buffer[BUF_SIZE];  // 헤더 + 실제 데이터

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            // 우선 메시지 전송
            size_t bytes_read;

            FILE *fp = fopen(file_path, "rb");
            if (!fp) {
                perror("파일 열기 실패");
                return;
            }
            
            while ((bytes_read = fread(buffer, 1, BUF_SIZE - FINAL_HEADER_LEN, fp)) > 0) {
                // 헤더를 먼저 넣고
                memcpy(send_buffer, FINAL_HEADER, FINAL_HEADER_LEN);
                // 그 뒤에 이미지 데이터 붙이기
                memcpy(send_buffer + FINAL_HEADER_LEN, buffer, bytes_read);
            
                // 총 전송할 길이 = 헤더 + 데이터
                size_t send_len = FINAL_HEADER_LEN + bytes_read;
            
                if(sendto(sockfd, send_buffer, send_len, 0,(struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr)) == -1){
                    printf("Connection Error: %d (%s)\n", errno, strerror(errno));
                }
                msleep(10); // 안정성 확보를 위한 delay
            }
            
            fclose(fp);
            // 이미지 전송 끝났으면 EOF 패킷 전송
            msleep(100);
            sendto(sockfd, "EOFFIN", 6, 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
            
        }
    }
}


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
void combine_images(const char* img1_path, const char* img2_path, const char* output_path, int combine_type) {
    struct jpeg_decompress_struct img1_info, img2_info;
    struct jpeg_error_mgr err;

    // 에러 핸들러 설정
    img1_info.err = jpeg_std_error(&err);
    img2_info.err = jpeg_std_error(&err);

    // 이미지 파일 열기
    FILE* img1_file = fopen(img1_path, "rb");
    FILE* img2_file = fopen(img2_path, "rb");

    if (!img1_file || !img2_file) {
        printf("Error: Unable to open image files.\n");
        return;
    }

    jpeg_create_decompress(&img1_info);
    jpeg_create_decompress(&img2_info);

    jpeg_stdio_src(&img1_info, img1_file);
    jpeg_stdio_src(&img2_info, img2_file);

    // 이미지 디코드
    jpeg_read_header(&img1_info, TRUE);
    jpeg_read_header(&img2_info, TRUE);

    jpeg_start_decompress(&img1_info);
    jpeg_start_decompress(&img2_info);

    int width, height;
    int channels = img1_info.output_components;

    if (combine_type == 0) {
        // 왼쪽-오른쪽 결합
        if (img1_info.output_height != img2_info.output_height) {
            printf("Error: Images must have the same height for horizontal combination.\n");
            return;
        }
        width = img1_info.output_width + img2_info.output_width;
        height = img1_info.output_height;
    } else {
        // 위-아래 결합
        if (img1_info.output_width != img2_info.output_width) {
            printf("Error: Images must have the same width for vertical combination.\n");
            return;
        }
        width = img1_info.output_width;
        height = img1_info.output_height + img2_info.output_height;
    }

    unsigned char* img1_row = (unsigned char*)malloc(img1_info.output_width * channels);
    unsigned char* img2_row = (unsigned char*)malloc(img2_info.output_width * channels);
    unsigned char* combined_row = (unsigned char*)malloc(width * channels);

    FILE* output_file = fopen(output_path, "wb");
    if (!output_file) {
        printf("Error: Unable to create output image file.\n");
        return;
    }

    struct jpeg_compress_struct out_info;
    out_info.err = jpeg_std_error(&err);
    jpeg_create_compress(&out_info);
    jpeg_stdio_dest(&out_info, output_file);

    // 출력 이미지 설정
    out_info.image_width = width;
    out_info.image_height = height;
    out_info.input_components = channels;
    out_info.in_color_space = img1_info.out_color_space;
    jpeg_set_defaults(&out_info);
    jpeg_start_compress(&out_info, TRUE);

    // 이미지 결합
    if (combine_type == 0) {
        // 왼쪽-오른쪽 결합
        while (img1_info.output_scanline < img1_info.output_height) {
            jpeg_read_scanlines(&img1_info, &img1_row, 1);
            jpeg_read_scanlines(&img2_info, &img2_row, 1);

            // 결합
            memcpy(combined_row, img1_row, img1_info.output_width * channels);
            memcpy(combined_row + img1_info.output_width * channels, img2_row, img2_info.output_width * channels);

            jpeg_write_scanlines(&out_info, &combined_row, 1);
        }
    } else {
        // 위-아래 결합
        while (img1_info.output_scanline < img1_info.output_height) {
            jpeg_read_scanlines(&img1_info, &img1_row, 1);
            jpeg_write_scanlines(&out_info, &img1_row, 1);
        }
        while (img2_info.output_scanline < img2_info.output_height) {
            jpeg_read_scanlines(&img2_info, &img2_row, 1);
            jpeg_write_scanlines(&out_info, &img2_row, 1);
        }
    }

    // 정리 및 리소스 해제
    jpeg_finish_decompress(&img1_info);
    jpeg_finish_decompress(&img2_info);
    jpeg_destroy_decompress(&img1_info);
    jpeg_destroy_decompress(&img2_info);

    jpeg_finish_compress(&out_info);
    jpeg_destroy_compress(&out_info);

    fclose(img1_file);
    fclose(img2_file);
    fclose(output_file);

    free(img1_row);
    free(img2_row);
    free(combined_row);

    printf("Images combined and saved as '%s'.\n", output_path);
}
int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    char filename[100];
    FILE *fp = NULL;
    int receiving_image = 0;
    char filename2[100];
    FILE *fp2 = NULL;
    int receiving_image2 = 0;
    char filename_final[100];
    FILE *fp_final = NULL;
    int receiving_image_final = 0;
    Client clients[MAX_CLIENTS] = {0,};
    char header[5] = {0};
    int count = 0;
    char msg[MAX_MSG_LEN] = {0,};
    int jpg_size = 0;
    unsigned char *jpg_data = NULL;
    char guide_name[7] = {0,};
    int guide_flag = 0;
    Client clients_temp[MAX_CLIENTS] = {0,};


    const char* img1 = "img_client1_1.jpg";
    const char* img2 = "img_client2_1.jpg";
    const char* temp[] = {"temp1.jpg","temp2.jpg"};
    const char* result[] = {"result_1.jpg","result_2.jpg","result_3.jpg","result_4.jpg"};
    const char* final = "final.jpg";
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
        ssize_t recv_len = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        
        if (recv_len < 0) {
            perror("recvfrom 실패");
            break;
        }
        buffer[recv_len] = '\0'; //메시지 끝 처리

        memcpy(header, buffer, 4);
        // "CONN" 수신 시 클라이언트 정보 저장 후 capture 명령 전송
        if (recv_len >= 4 && strncmp(header, "CONN", 4) == 0) {
            if(find_client(clients, &client_addr) == -1){
                add_client(clients,&client_addr); //클라이언트 추가   
                client_index = find_client(clients, &client_addr);
                // sprintf(msg, "CLI%d", client_index + 1);
                clients[client_index].addr.sin_port = htons(SERVER_PORT);
                msleep(200);  // 충분한 설정시간 줌
                if (client_index == 1){
                    // broadcast_message(sockfd, clients, "CONNCOMP");
                    broadcast_message(sockfd, clients, guide_name);
                    send_message(sockfd, clients[0].addr, "CLI1");
                    send_message(sockfd, clients[1].addr, "CLI2");
                    pthread_t tid;
                    struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
                    args->sockfd = sockfd;
                    memcpy(args->clients, clients, sizeof(Client) * MAX_CLIENTS);
                    if(client_index == 1){
                        if (pthread_create(&tid, NULL, time_sender_thread, args) != 0)
                        {
                            perror("타임 스레드 생성 실패");
                            free(args);
                        } 
                        else
                        {
                            pthread_detach(tid); // 메모리 자동 회수
                        }
                    }
                    continue;
                }
                printf("클라이언트 추가됨: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            }
            // client_found = 1;
        }
        // "EOF" 수신 시 이미지 저장 종료
        else if (recv_len >= 6 && strncmp(buffer, "EOFCAP", 6) == 0) {
            if (strncmp(buffer, "EOFCAP1", 7) == 0 && fp) {
                fclose(fp);
                fp = NULL;
                receiving_image = 0;
            }
            else if (strncmp(buffer, "EOFCAP2", 7) == 0 && fp2) {
                fclose(fp2);
                fp2 = NULL;
                receiving_image2 = 0;
            }
            count ++;
            printf("이미지 수신 완료! \n");
            if(count%2==0){// 왼쪽/오른쪽 수신
                if(count%4==0){
                    combine_images(img1, img2, temp[1],0);
                    combine_images(temp[0], temp[1],result[count/4-1],0);
                }else{
                    combine_images(img1, img2, temp[0],0);
                }
            }
            if(count==16){ //촬영 완료 
                combine_images(result[0], result[1], temp[0],1);
                combine_images(result[2], result[3], temp[1],1);
                combine_images(temp[0], temp[1],final,1);
                broadcast_message_with_file(sockfd, clients, "final", final);
            }
            continue;
        }

        else if (strncmp(header, "CAP1", 4) == 0)
        {
            // 저장: CAPn
            if (!fp && !receiving_image) {
                sprintf(filename, "img_client1_1.jpg"); 
                fp = fopen(filename, "wb");
                if (!fp) {
                    perror("파일 열기 실패");
                    break;
                }
                printf("이미지 저장 시작합니다: %s\n", filename);
            }
            receiving_image = 1;
            if (fp)
            {
                fwrite(buffer + 4, 1, recv_len - 4, fp);
            }
        }
        else if (strncmp(header, "CAP2", 4) == 0)
        {
            // 저장: CAPn
            if (!fp2 && !receiving_image2) {
                sprintf(filename2, "img_client2_1.jpg");
                fp2 = fopen(filename2, "wb");
                if (!fp2) {
                    perror("파일 열기 실패");
                    break;
                }
                printf("이미지 저장 시작합니다: %s\n", filename2);
            }
            receiving_image2 = 1;
            if (fp2)
            {
                fwrite(buffer + 4, 1, recv_len - 4, fp2);
            }
        }
        else if (strncmp(header, "IMG1", 4) == 0 || strncmp(buffer, "EOFIMG1", 7) == 0)
        {
            sendto(sockfd, buffer, recv_len, 0,(struct sockaddr*)&clients[1].addr, sizeof(clients[1].addr));
        }
        else if (strncmp(header, "IMG2", 4) == 0 || strncmp(buffer, "EOFIMG2", 7) == 0)
        {
            sendto(sockfd, buffer, recv_len, 0,(struct sockaddr*)&clients[0].addr, sizeof(clients[0].addr));
        }
        else if (recv_len >= 5 && strncmp(buffer, "GUIDE", 5) == 0 && guide_flag == 0)
        {
            guide_flag = 1;
            strncpy(guide_name, buffer, 6);
            msleep(200);
            broadcast_message(sockfd, clients_temp, "SELG");
            memset(&clients_temp, 0, sizeof(clients_temp));
        }
        else if (recv_len >= 4 && strncmp(buffer, "TEMP", 4) == 0 ){
            if(find_client(clients_temp, &client_addr) == -1)
            {
                add_client(clients_temp, &client_addr); //클라이언트 추가   
                client_index = find_client(clients_temp, &client_addr);
                // sprintf(msg, "CLI%d", client_index + 1);
                clients_temp[client_index].addr.sin_port = htons(SERVER_PORT);
                msleep(200); // 충분한 설정시간 줌
                if (client_index == 1)
                {
                    printf("broadcast\n");
                    broadcast_message(sockfd, clients_temp, "CONNCOMP");
                    msleep(100);
                    memset(&clients_temp, 0, sizeof(clients_temp));
                }
            }
        }
        else if (recv_len >= 4 && strncmp(buffer, "DECO", 4) == 0 ){
            if (!fp_final && !receiving_image_final) {
                // 현재 시간 기반 파일명 생성
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(filename_final, sizeof(filename_final), "image_%Y%m%d_%H%M%S.jpg", t);
        
                fp_final = fopen(filename_final, "wb");
                if (!fp_final) {
                    perror("파일 열기 실패");
                    break;
                }
                printf("이미지 저장 시작합니다: %s\n", filename);
            }
            receiving_image_final = 1;
            if (fp_final) {
                fwrite(buffer + 4, 1, recv_len - 4, fp_final);
            }
        }
        else if (strncmp(buffer, "EOFDECO", 7) == 0) {
            // 마지막 파일 닫은 뒤 변수 초기화(안해줘도 되는 변수도 있지만 다 초기화)
            if (fp_final)
            {
                fclose(fp_final);
                fp_final = NULL;
            }

            receiving_image = 0;
            receiving_image2 = 0;
            receiving_image_final = 0;
            count = 0;
            jpg_size = 0;

            jpg_data = NULL;
            guide_flag = 0;
            receiving_image_final = 0;
            memset(&clients, 0, sizeof(clients));
            // memset(&clients_temp, 0, sizeof(clients_temp));

            memset(guide_name, 0, sizeof(guide_name));
            memset(buffer, 0, sizeof(buffer));
            memset(filename, 0,sizeof(filename));
            memset(filename2, 0, sizeof(filename2));
            memset(filename_final, 0, sizeof(filename_final));
            memset(header, 0, sizeof(header));
            memset(msg, 0, sizeof(msg));

            if (fp)
            {
                fclose(fp);
                fp = NULL;
            }
            if (fp2)
            {
                fclose(fp2);
                fp2 = NULL;
            }
        }
        else if (recv_len >= 4 && strncmp(buffer, "SELG", 4) == 0 ){
            if(find_client(clients_temp, &client_addr) == -1)
            {
                add_client(clients_temp, &client_addr); //클라이언트 추가   
                client_index = find_client(clients_temp, &client_addr);
                // sprintf(msg, "CLI%d", client_index + 1);
                clients_temp[client_index].addr.sin_port = htons(SERVER_PORT);
                msleep(100);
            }
        }

    }

    if (fp) fclose(fp);
    close(sockfd);
    return 0;
}
