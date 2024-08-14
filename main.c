#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const char *flags[] = {"--help", "-?", "-v"};
const char *flags_help[] = {"--help", "-?"};

void flag_help()
{
    printf("Usage: ping [OPTION...] HOST ...\n"
           "Send ICMP ECHO_REQUEST packets to network hosts.\n"
           "\n"
           " Options valid for all request types:\n"
           "\n"
           "  -v, --verbose              verbose output\n"
           "\n"
           "\n"
           "  -?, --help                 give this help list\n"
           "\n"
           "Mandatory or optional arguments to long options are also mandatory or optional\n"
           "for any corresponding short options.\n"
           "\n"
           "Options marked with (root only) are available only to superuser.\n"
           "\n"
           "Report bugs to <bug-inetutils@gnu.org>.\n");
}

int my_ping(int ac, char **av)
{
    int uniq = 0;
    if (ac > 1)
    {
        int flag_found = 0;
        for (int i = 1; i < ac; i++)
        {
            for (long unsigned int j = 0; j + 1 < sizeof(flags) / sizeof(flags[0]); j++)
            {
                if (!strncmp(av[i], flags_help[j], strlen(flags_help[j])))
                {
                    flag_help();
                    return 0;
                }
                else if (!strncmp(av[i], "-", 1))
                {
                    if (!strcmp(av[i], "--") && i == 1)
                    {
                        flag_found++;
                        uniq++;
                        break;
                    }
                    if (strstr(av[i], flags_help[j]) || !strncmp(av[i], "---", 3))
                    {
                        printf("ping: unrecognized option '%s'\n", av[i]);
                        flag_found++;
                        break;
                    }
                }
            }
            if (flag_found)
                break;
        }
        if (!flag_found)
        {
            printf("ping: unknown host\n");
            return 1;
        }
    }
    if (ac == 1 || uniq)
        printf("ping: missing host operand\n");
    printf("Try 'ping --help' or 'ping --usage' for more information.\n");
    return 64;

}

int ext_ping(char **av)
{
    char *ping = "/usr/local/bin/ping"; // Path to the ping executable
    av[0] = "ping"; // Set the first argument to "ping"

    // Execute ping with the provided arguments
    int ret = execve(ping, av, NULL);
    if (ret == -1)
    {
        perror("execve");
        return 1;
    }
    return 0;
}

int main(int ac, char **av)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        ext_ping(av);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int ret = WEXITSTATUS(status);
            printf("\033[0;32m\nRetour: %d\n\033[0m", ret);
        }
        printf("-----------------------------\n");
        printf("\033[0;32m\nRetour: %d\n\033[0m", my_ping(ac, av)); 
    }
    return 0;
}