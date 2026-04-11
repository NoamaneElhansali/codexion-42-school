#include "codexion.h"


int main(int argc, char *argv[])
{
    int *param;
    t_table *table;
    int i = 0;
    // t_coder *coders;
    if (!parser_parameter(argv, argc))
        return 1;
    param = parser(argv, argc);
    table = init_parameter(param, argv[8]);
    start_monitoring(table);

    pthread_mutex_destroy(&table->print_lock);
    pthread_mutex_destroy(&table->stop_lock);
    pthread_mutex_destroy(&table->queue_lock);
    pthread_cond_destroy(&table->cond);
    while (i < table->nb_coders)
    {
        pthread_mutex_destroy(&table->dongles[i]);
        pthread_mutex_destroy(&table->coders[i].compile_count_lock);
        pthread_mutex_destroy(&table->coders[i].last_compile_lock);
        i++;
    }
    
    free(param);
    free(table->coders);
    free(table->dongles);
    free(table);
    return 0;
}