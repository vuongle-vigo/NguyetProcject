# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
#define RESPONSE_SIZE 4 //bits

enum ResponseCode { // 4 bit cuoi
    DONE = 0b0001, //truyen het du lieu
    FILEFDOWNLOAD = 0b1000
};

int checkResponse(char *data){
    char responseBuffer[RESPONSE_SIZE];
    if(strncmp(data, "$$", 2)!=0){
        return -1;
    }
    strcpy(responseBuffer, data + 2);
    return atoi(responseBuffer);
}

int responseHandler(char *data, int code){
    int responseCode = checkResponse(data);
    if(responseCode == -1 && ((code & 0b0001) == 0b0001)){//du lieu thuong
        printf("%s", data);
        return code;
    }
    if(responseCode == -1 && ((code & 0b0010) == 0b0010)){
        int sockfd = setSock(NULL, 0);
        struct sockaddr_in ca2download;
        char filepath[100];
        char filename[100];
        uint32_t ip;
        int port;
        char * token = strtok(data, " ");
        // Lấy ra toàn bộ token
        strcpy(filepath, token);
        token = strtok(NULL, " ");
        strcpy(filename, token);
        token = strtok(NULL, " ");
        ip = atoi(token);
        token = strtok(NULL, " ");
        port = atoi(token);
        ca2download.sin_family = AF_INET;
        ca2download.sin_addr.s_addr = ip;
        ca2download.sin_port = htons(port);
        int cfd = connect(sockfd, (struct sockaddr *)&ca2download, sizeof(ca2download));
        if(cfd<0){
            perror("connect");
            exit(1);
        }
        char buf[1024] = {0};
        strcat(buf, filepath);
        strcat(buf, filename);
        sendData(sockfd, buf, strlen(buf));
        printf("buf: %s\n", buf);
        printf("Downloading\n"); 
        struct pollfd fds;
        fds.fd = sockfd;
        fds.events = POLLIN;
        fds.revents = POLLIN;
        memset(buf, 0, sizeof(buf));
        int recv1 = recvData(sockfd, buf, sizeof(buf));
        int codeRecvFile = checkResponse(buf); 
        printf("recv data: %s\n", buf);
        FILE *f = fopen("test1.jpeg", "wb");
        int count = 0;
        while(1){
            int ret = poll(&fds, 1, 5*1000);
            if(-1 == ret){
                perror("poll");
                return -1;
            }
            if(ret == 0){
                printf("poll() timed out: %d\n", TIMEOUT);
                break;
            }
            if(fds.revents & POLLIN){
                char ch;
                int recvc = recv(sockfd, &ch, 1, 0);
                fwrite(&ch, 1, 1, f);
            }
        }
        printf("counters: %d\n", count);
        fclose(f);
        printf("download complete\n");
    }
    return code;
}