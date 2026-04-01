#include "codexion.h"


int main(int argc, char *argv[])
{
    int *param;
    t_table *table;
    // t_coder *coders;
    if (!parser_parameter(argv, argc))
        return 1;
    param = parser(argv, argc);
    table = init_parameter(param, argv[8]);
    free(param);
    free(table->coders);
    free(table->dongles);
    free(table);
    return 0;
}