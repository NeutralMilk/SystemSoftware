#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/stat.h>

#include "backup.h"
#include "configuration.h"

int main(int argc, char *argv[]) {
    struct config_struct config;
    read_config_file(config);
}