#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;

    char *args[64];

    while (1)
    {
        // Display prompt
        printf("shellforge$ ");
        fflush(stdout);

        // Ctrl+D
        if (getline(&line, &len, stdin) == -1)
            break;

        // Remove newline
        line[strcspn(line, "\n")] = '\0';

        int i = 0;

        // Split line into words
        char *token = strtok(line, " \t");

        while (token != NULL && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }

        // NULL termination
        args[i] = NULL;

        // Empty input
        if (i == 0)
            continue;

        // Exit
        if (strcmp(args[0], "exit") == 0)
            break;

        // Create child process
        pid_t pid = fork();

        if (pid == 0)
        {
            // Child process
            execvp(args[0], args);

            // Runs only if execvp fails
            perror("Command execution error");
            exit(1);
        }
        else if (pid > 0)
        {
            // Parent process waits for child
            waitpid(pid, NULL, 0);
        }
        else
        {
            // fork failed
            perror("Fork creation error");
        }
    }

    free(line);

    return 0;
}
