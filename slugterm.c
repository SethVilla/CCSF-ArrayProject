#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char input[4096];
    int last_status = 0;

    while (1) {
        printf("slugterm> ");
        fflush(stdout);

        // Read input - exit ONLY on EOF
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        // Check for empty command
        if (strlen(input) == 0) {
            fprintf(stderr, "command malformed\n");
            last_status = 1;
            continue;
        }

        // Check for leading/trailing pipe
        if (input[0] == '|' || input[strlen(input) - 1] == '|') {
            fprintf(stderr, "command malformed\n");
            last_status = 1;
            continue;
        }

        // Handle errcode - just print and continue
        if (strcmp(input, "errcode") == 0) {
            printf("%d\n", last_status);
            last_status = 0;
            continue;
        }

        // Parse pipeline commands - store in persistent buffer
        char *commands[16];
        int num_commands = 0;
        
        // Use input directly for parsing (it persists)
        char *saveptr1;
        char *token = strtok_r(input, "|", &saveptr1);
        
        while (token != NULL && num_commands < 16) {
            // Trim leading spaces
            while (*token == ' ') token++;
            
            // Trim trailing spaces
            char *end = token + strlen(token) - 1;
            while (end > token && *end == ' ') {
                *end = '\0';
                end--;
            }

            if (strlen(token) > 0) {
                commands[num_commands++] = token;
            }
            token = strtok_r(NULL, "|", &saveptr1);
        }

        if (num_commands == 0) {
            fprintf(stderr, "command malformed\n");
            last_status = 1;
            continue;
        }

        // Single command
        if (num_commands == 1) {
            char *args[128];
            int arg_count = 0;

            char *saveptr2;
            char *arg = strtok_r(commands[0], " ", &saveptr2);
            while (arg != NULL && arg_count < 127) {
                args[arg_count++] = arg;
                arg = strtok_r(NULL, " ", &saveptr2);
            }
            args[arg_count] = NULL;

            if (arg_count == 0) {
                fprintf(stderr, "command malformed\n");
                last_status = 1;
                continue;
            }

            pid_t pid = fork();
            if (pid == 0) {
                execv(args[0], args);
                perror("execv failed");
                exit(1);
            } else if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
                last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
            } else {
                perror("fork failed");
                last_status = 1;
            }
            continue;
        }

        // Multi-stage pipeline
        int pipes[15][2];
        pid_t pids[16];
        int pipes_created = 0;
        int fork_failed = 0;

        // Create all pipes first
        for (int i = 0; i < num_commands - 1; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe failed");
                last_status = 1;
                fork_failed = 1;
                break;
            }
            pipes_created++;
        }

        // If pipe creation failed, clean up and continue
        if (fork_failed) {
            for (int j = 0; j < pipes_created; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            continue;
        }

        // Fork all children
        int children_forked = 0;
        for (int i = 0; i < num_commands; i++) {
            pids[i] = fork();
            if (pids[i] == 0) {
                // Child process - set up I/O redirection
                if (i > 0) {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }
                if (i < num_commands - 1) {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }

                // Close ALL pipe file descriptors in child
                for (int j = 0; j < num_commands - 1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // Parse and execute command
                char *args[128];
                int arg_count = 0;
                
                char *saveptr3;
                char *arg = strtok_r(commands[i], " ", &saveptr3);
                while (arg != NULL && arg_count < 127) {
                    args[arg_count++] = arg;
                    arg = strtok_r(NULL, " ", &saveptr3);
                }
                args[arg_count] = NULL;

                if (arg_count == 0) {
                    exit(1);
                }

                execv(args[0], args);
                perror("execv failed");
                exit(1);
            } else if (pids[i] < 0) {
                perror("fork failed");
                fork_failed = 1;
                break;
            }
            children_forked++;
        }

        // Parent: Close ALL pipe ends
        for (int i = 0; i < num_commands - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        // Wait for all children that were actually forked
        for (int i = 0; i < children_forked; i++) {
            int status;
            waitpid(pids[i], &status, 0);
            if (i == children_forked - 1) {
                last_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
            }
        }

        if (fork_failed) {
            last_status = 1;
        }
    }

    return 0;
}
