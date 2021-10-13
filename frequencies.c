#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "frequencies.h"

bool calc_frequencies(Frequencies freqs, const char* path, const char** a_error){

    FILE* fileOpen;
    fileOpen = fopen(path, "r");

    if (fileOpen == NULL){
        *a_error = strerror(errno);
        return false; 
    }
    else {
        for(char character = fgetc(fileOpen); !feof(fileOpen); character = fgetc(fileOpen)) {   
            freqs[(int)character]++;
      }
    }

    fclose(fileOpen);
    return true; 

}
