#include "Generate.h"

#include <cstdio>
#include <string>

#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include "blunder.h"

void Generate::generate() {
    passwd *pw = getpwuid(getuid());

    std::string dir = pw->pw_dir;
    dir += "/.torlux";
    std::string hsdir = dir + "/hs";

    struct stat st;
    if (stat(dir.c_str(), &st) == 0) {
        std::string cmd = "rm -r " + dir;
        system(cmd.c_str());
    }

    mkdir(dir.c_str(), S_IRWXU);

    FILE *fout = fopen((dir + "/torrc").c_str(), "w");
    if (fout == NULL) {
        puts("Failed to create torrc file");
        exit(1);
    }
    fputs("SocksPort 9350\n\n", fout);
    fprintf(fout, "HiddenServiceDir %s\n", hsdir.c_str());
    fputs("HiddenServicePort 80 127.0.0.1:42069\n", fout);
    fputs("\nHiddenServiceVersion 3\n", fout);
    fputs("\n# feel free to add additional tor configurations:\n", fout);
    fclose(fout);

    mkdir(hsdir.c_str(), S_IRWXU);
}




static FILE *fin = NULL;

void Generate::initRandom() {
    fin = fopen("/dev/urandom", "rb");
    if (fin == NULL) {
        die("Failed to open /dev/urandom");
    }
}

void Generate::randBytes(char *dest, size_t n) {
    fread(dest, 1, n, fin);
}

void Generate::cleanupRandom() {
    if (fin != NULL) {
        fclose(fin);
    }
}