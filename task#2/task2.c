#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

const int BUFFER_SIZE = 100;

int get_sparce_file(char *file_name) {
    int file_desc = open(file_name, O_WRONLY);

    if (file_desc == -1) {
        printf("%s\n", "Error!");
        return file_desc;
    }

    char write_buffer[BUFFER_SIZE];
    char read_buffer[BUFFER_SIZE];

    int offset = 0;
    int len = 0;

    int readed;
    int i = 0;

    while (1) {
        readed = read(STDIN_FILENO, read_buffer, BUFFER_SIZE);

        if (readed) {
            while (i < readed) {
                while (i < readed && read_buffer[i] == 0) {
                    offset++;
                    i++;
                }

                while (i < readed && read_buffer[i] != 0) {
                    write_buffer[len] = read_buffer[i];
                    len++;
                    i++;
                }

                if (len > 0) {
                    write(file_desc, write_buffer, len);
                    len = 0;
                }

                if (offset > 0) {
                    lseek(file_desc,  offset, SEEK_CUR);
                    offset = 0;
                }
            }
            printf("%s\n", "Done!");
        } else {
            break;
        }

    }
    return close(file_desc);
}

int main(int argc, char *argv[]) {
    if (get_sparce_file(argv[1]) < 0) {
        printf("%s\n", "Error!");
    }
    return 0;
}