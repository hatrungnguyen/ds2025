/*
*
    * server.c: RPC Server Implementation for FTP Proxy
    */
   #include <rpc/rpc.h>
   #include "rpc_ftp.h"
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <dirent.h>
   #include <unistd.h>

   #define FILE_DIRECTORY "./files/"

   int *upload_1_svc(char **file_name, struct svc_req *req) {
    static int result = 0;
    FILE *file = fopen(*file_name, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        result = -1;
        return &result;
    }

    fwrite((*file_name), sizeof(char), strlen(*file_name), file);
    fclose(file);
    result = 0;
    return &result;
}

opaque *download_1_svc(char **file_name, struct svc_req *req) {
    static char file_data[4096];
    memset(file_data, 0, sizeof(file_data));

    FILE *file = fopen(*file_name, "rb");
    if (!file) {
        perror("File not found");
        return NULL;
    }

    fread(file_data, 1, sizeof(file_data), file);
    fclose(file);
    return file_data;
}

char **list_files_1_svc(void *argp, struct svc_req *req) {
    static char result[4096];
    memset(result, 0, sizeof(result));

    DIR *dir = opendir(FILE_DIRECTORY);
    if (!dir) {
        strcpy(result, "Error opening directory.");
        return &result;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            strcat(result, entry->d_name);
            strcat(result, "\n");
        }
    }
    closedir(dir);
    return &result;
}