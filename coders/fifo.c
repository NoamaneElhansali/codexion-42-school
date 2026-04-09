#include "codexion.h"


void push(t_queue *q, int id)
{
    q->data[q->r++] = id;
}


int pop(t_queue *q)
{
    return q->data[q->f++];
}

void init_queue(t_queue *q)
{
    q->f = 0;
    q->r = 0;
}

int take_dongles_fifo(t_coder *coder)
{
    t_table *t = coder->table;
    pthread_mutex_lock(&t->queue_lock);
    push(&t->queue, coder->id);
    while (!is_in_first_fifo(&t->queue, coder->id, t->nb_coders / 2) && !get_stop(t))
        pthread_cond_wait(&t->cond, &t->queue_lock);
    pthread_mutex_unlock(&t->queue_lock);
    if (get_stop(t))
        return 0;
    take_dongles(coder);
    return 1;
}

void release_dongles_fifo(t_coder *coder)
{
    give_dongles(coder);
    pthread_mutex_lock(&coder->table->queue_lock);
    pop(&coder->table->queue);
    pthread_cond_broadcast(&coder->table->cond);
    pthread_mutex_unlock(&coder->table->queue_lock);
}

int is_in_first_fifo(t_queue *q, int id, int n)
{
    int i = q->f;
    int count = 0;

    while (i < q->r && count < n)
    {
        if (q->data[i] == id)
            return (1);
        count++;
        i++;
    }
    return (0);
}