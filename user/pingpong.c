#include "kernel/types.h"
#include "user/user.h"


const char* CHILD_MESSAGE = "sent by child";
const char* PARENT_MESSAGE = "sent by parent";
const int BUF_SIZE = sizeof(char) * 100;

int main(int argc, char** argv) {
    int pipe_write_to_child[2];
    int pipe_write_to_parent[2];

    pipe(pipe_write_to_parent);
    pipe(pipe_write_to_child);

    if (fork() == 0) {
        close(pipe_write_to_parent[0]);
        close(pipe_write_to_child[1]);

        char* buf = malloc(BUF_SIZE);

        int size = 0;
        for (;read(pipe_write_to_child[0], buf + size, 1) > 0; size++);
        buf[size] = 0;

        close(pipe_write_to_child[0]);

        printf("%d: got %s\n", getpid(), buf);
        free(buf);
        
        write(pipe_write_to_parent[1], CHILD_MESSAGE, strlen(CHILD_MESSAGE));
        close(pipe_write_to_parent[1]);
    } else {
        close(pipe_write_to_parent[1]);
        close(pipe_write_to_child[0]);

        write(pipe_write_to_child[1], PARENT_MESSAGE, strlen(PARENT_MESSAGE));
        close(pipe_write_to_child[1]);
        
        char* buf = malloc(BUF_SIZE);
        
        int size = 0;
        for (;read(pipe_write_to_parent[0], buf + size, 1) > 0; size++);
        buf[size] = 0;

        close(pipe_write_to_parent[0]);

        printf("%d: got %s\n", getpid(), buf);
        free(buf);
    }
    
    exit(0);
}