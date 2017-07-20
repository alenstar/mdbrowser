#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <io.h>
#include <fcntl.h>
#endif

#ifdef __cplusplus 
extern "C" {
#endif
// free memory by user
char *load_file(const char *path, size_t *data_size)
{
    FILE *fp = NULL;
    struct stat sb;
    char *buffer = NULL;
    size_t size = 0;
    size_t n = 0;

    if (stat(path, &sb)) {
        perror(path);
        //exit(EXIT_FAILURE);
        return NULL;
    }
    size = sb.st_size;

    fp = fopen(path, "rb");
    if (!fp) {
        perror(path);
        //exit(EXIT_FAILURE);
        return NULL;
    }

    buffer = (char*)malloc(size);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate %lld bytes\n",
                (long long)size);
        //exit(EXIT_FAILURE);
        fclose(fp);
        return NULL;
    }

    n = fread(buffer, size, 1, fp);
    if (n != size) {
        perror(path);
        //exit(EXIT_FAILURE);
        fclose(fp);
        return NULL;
    }

    fclose(fp);

    *data_size = size;
    return buffer;
}

int save_file(const char *path, char* data, size_t size)
{
    FILE *fp = NULL;
    fp = fopen(path, "wb");
    if(fp == NULL) {
        fprintf(stderr, "Unable to open file %s\n",path);
        return -1;
    }

    size_t n = fwrite(data, size, 1, fp);
    if (n != size) {
        perror(path);
        fclose(fp);
        return -2;
    }
    fclose(fp);
    return 0;
}
#ifdef __cplusplus 
}
#endif