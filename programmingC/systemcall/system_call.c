#include <stdio.h>
#include <stdlib.h>


int filename_exist(const char *filename){

    char command[256];
    snprintf(command, sizeof(command), "test -f %s", filename);
    return system(command);
}

int main(){
    if (filename_exist("../dockercommand/docker_container.c") == 0) {
        printf("File exists.\n");
    } else {
        printf("File does not exist.\n");
    }
    return 0;
}