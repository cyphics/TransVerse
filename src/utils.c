#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

char buff[PATH_MAX];

void cut_string_after_pattern(char *string_to_cut, char *pattern){
    int original_idx = 0;
    int comparison_idx = 0;

    while(string_to_cut[original_idx] != '\0'){
        if(string_to_cut[original_idx] == pattern[comparison_idx]) {
            comparison_idx++;
            if(pattern[comparison_idx] == '\0'){
                string_to_cut[original_idx + 1] = '\0';
                return;
            }
        }
        else comparison_idx = 0;
        original_idx++;
    }
}

const char* get_project_root_dir() {
    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len != -1) {
      buff[len] = '\0';
    }
    cut_string_after_pattern(buff, "transverse");
    return buff;
}
