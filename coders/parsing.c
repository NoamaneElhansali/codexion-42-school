#include "codexion.h"

int parser_parameter(char **av, int ac)
{
    int i = 1;
    long numbre;
    // int *param;
    // t_scheduler shu;
    if (ac != 9)
        return 0;
    while (i < ac - 1)
    {
        numbre = ft_atoi(av[i++]);
        if (numbre < 0 || numbre > INT_MAX)
            return 0;
    }
    if (!ft_strcmp_adv(av[8],"fifo") && !ft_strcmp_adv(av[8],"edf"))
        return (0);
    // param = parser(av, ac);
    // shu = FIFO;
    // if (ft_strcmp_adv(av[8],"edf"))
    //     shu = EDF;
    // init_parameter(param, shu);
    return (1);
}