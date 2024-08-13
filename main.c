#include <stdio.h>
#include <string.h>
        
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

int main(int ac, char **av)
{
    if (ac > 1)
    {
        int flag_found = 0;
        for (int i = 1; i < ac; i++)
        {
            for (long unsigned int j = 0; j+1 < sizeof(flags) / sizeof(flags[0]); j++)
            {
                if (!strncmp(av[i], flags_help[j], strlen(flags_help[j])))
                {
                    flag_help();
                    return 0;
                }
                else if (!strncmp(av[i], "-", 1))
                {
                    if (strstr(av[i], flags_help[j]))
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
    printf("Try 'ping --help' or 'ping --usage' for more information.\n");
    return 64;
}
