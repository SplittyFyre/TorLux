#include "tool.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

static char discardbuf[4096];
void read_discard(int sockfd) {
    while (recv(sockfd, discardbuf, 4096, 0) == 4096);
}

int hcti(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool validb32(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '2' && c <= '7') || (c >= 'A' && c <= 'Z');
}

void die(const char *fmt, ...) {
    puts("Program died badly:");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf("\nerrno string: %s (%d)\n", strerror(errno), errno);

    exit(1);
}

void csrng(char *dest, size_t s) {
    FILE *fin = fopen("/dev/urandom", "rb");
    if (fin == NULL) {
        die("Failed to open /dev/urandom for csrng");
    }
    fread(dest, 1, s, fin);
    fclose(fin);
}



void generateConfig() {
    struct passwd *pw = getpwuid(getuid());

    char dir[128], hsdir[128], torrcpath[128];
    strcpy(dir, pw->pw_dir);
    strcat(dir, "/.torlux");
    
    strcpy(hsdir, dir);
    strcat(hsdir, "/hs");

    struct stat st;
    if (stat(dir, &st) == 0) { // if dir already there
        char cmd[128];
        strcpy(cmd, "rm -r ");
        strcat(cmd, dir);
        system(cmd);
    }

    mkdir(dir, S_IRWXU);

    strcpy(torrcpath, dir);
    strcat(torrcpath, "/torrc");
    FILE *fout = fopen(torrcpath, "w");
    if (fout == NULL) {
        puts("Failed to create torrc file");
        exit(1);
    }
    fputs("SocksPort 9350\n\n", fout);
    fprintf(fout, "HiddenServiceDir %s\n", hsdir);
    fputs("HiddenServicePort 80 127.0.0.1:42069\n", fout);
    fputs("\nHiddenServiceVersion 3\n", fout);
    fputs("\n# feel free to add additional tor configurations:\n", fout);
    fclose(fout);

    mkdir(hsdir, S_IRWXU);
}
