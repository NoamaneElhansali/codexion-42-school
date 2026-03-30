#include "codexion.h"

int parser_parameter(char **av, int ac)
{
    int i = 1;
    if (ac != 10)
        return 0;
    while (i < ac - 1)
        if (ft_atoi(av[i++]) < 0)
            return 0;
    if (!ft_strcmp(av[9],"fifo") && !ft_strcmp(av[9],"edf"))
        return (0);
    init_parameter();
    return (1);
}