/*
 * client.c: RPC Client Implementation for FTP Proxy
 */
#include <rpc/rpc.h>
#include "rpc_ftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void upload_file(FTPPROXY_PROG_client *client, const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_data = malloc(file_size);
    fread(file_data, 1, file_size, file);
    fclose(file);

    int *result = upload_1(file_name, file_data, client);
    if (result && *result == 0) {
        printf("File uploaded successfully!\n");
    } else {
        printf("Failed to upload file.\n");
    }
    free(file_data);
}

void download_file(FTPPROXY_PROG_client *client, const char *file_name) {
    opaque *file_data = download_1(file_name, client);
    if (file_data == NULL) {
        printf("Failed to download file.\n");
        return;
    }

    FILE *file = fopen(file_name, "wb");
    if (!file) {
        perror("Failed to create file locally");
        return;
    }

    fwrite(file_data, 1, strlen(file_data), file);
    fclose(file);
    printf("File downloaded successfully!\n");
}

void list_files(FTPPROXY_PROG_client *client) {
    char **file_list = list_files_1(NULL, client);
    if (file_list) {
        printf("Files on server:\n%s", *file_list);
    } else {
        printf("Failed to list files.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <server_host>\n", argv[0]);
        exit(1);
    }

    FTPPROXY_PROG_client *client = clnt_create(argv[1], FTPPROXY_PROG, FTPPROXY_VERS, "tcp");
    if (!client) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    // Example CLI
    upload_file(client, "example.txt");
    download_file(client, "example.txt");
    list_files(client);

    clnt_destroy(client);
    return 0;
}