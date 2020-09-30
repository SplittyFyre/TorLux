#include "Generate.h"

#include <string>

#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>

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
    fputs("\n# feel free to add additional tor configurations:\n", fout);
    fclose(fout);

    mkdir(hsdir.c_str(), S_IRWXU);
}