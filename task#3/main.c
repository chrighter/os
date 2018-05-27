#include <glib.h>
#include <errno.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sort_func(gconstpointer a, gconstpointer b) {
    int *int_a = (int*) a;
    int *int_b = (int*) b;

    return (*int_a) - (*int_b);
}

void print_error(char *info, bool exit_flag) {
    printf("%s: %s\n", strerror(errno), info);

    if (exit_flag) {
        exit(1);
    }
}

FILE *open_file(char *file_name) {
    FILE *fp = fopen(file_name, "r");

    if (errno != 0) {
        print_error(file_name, false);
        exit(1);
    } else {

        return fp;
    }
}

void get_numbers(char *line, GArray *array) {
    regex_t re;
    regmatch_t match;

    regcomp(&re, "-?[0-9]+", REG_EXTENDED);

    while (regexec(&re, line, 1, &match, 0) == 0) {
        size_t len_match = (size_t) (match.rm_eo - match.rm_so);

        if ((int) len_match < 10) {
            char *buffer = (char *) malloc(len_match);

            if (errno != 0) {
                print_error("Error!", true);
            }

            memcpy(buffer, line + match.rm_so, len_match);
            int num = atoi(buffer);
            g_array_append_val(array, num);
            free(buffer);
        }

        line += match.rm_eo;
    }
}

void parse_file(char *file_name, GArray *array) {
    FILE *fp = open_file(file_name);
    char *line = NULL;
    size_t len = 0;

    while ((getline(&line, &len, fp)) != -1) {
        get_numbers(line, array);
        line = NULL;
        len = 0;
    }

    fclose(fp);
}

GArray *get_all_sort_numbers(int argc, char *argv[]){
    GArray *array = g_array_new(FALSE, FALSE, sizeof(int));

    for (int i = 1; i < argc - 1; i++) {
        parse_file(argv[i], array);
    }

    g_array_sort(array, sort_func);

    return array;
}

void create_result_file(GArray *array, int argc, char *argv[]) {
    char *result_file_name = argv[argc - 1];

    FILE *result_fp = fopen(result_file_name, "wa");

    if (errno != 0) {
        print_error(result_file_name, true);
    }

    for (int i = 0; i < array->len; i++) {
        fprintf(result_fp, "%d ", g_array_index(array, int, i));
    }

    fclose(result_fp);
    g_array_free(array, TRUE);
    printf("%s\n", "Done!");
}


int main(int argc, char *argv[]) {
    GArray *array = get_all_sort_numbers(argc, argv);
    create_result_file(array, argc, argv);
}