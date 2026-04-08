#include "codexion.h"

long get_deadline(t_coder *coder)
{
    return coder->last_compile + coder->table->time_to_burnout;
}

void add_to_heap(t_heap *heap, t_coder *coder)
{
    int i = heap->size;
    t_coder *tmp;

    heap->data[i] = coder;
    heap->size++;

    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (get_deadline(heap->data[p]) <= get_deadline(heap->data[i]))
            break;
        
        tmp = heap->data[p];
        heap->data[p] = heap->data[i];
        heap->data[i] = tmp;
        
        i = p;
    }
}

void remove_min(t_heap *heap)
{
    int i = 0;
    int j;
    int z;
    int smallest;
    t_coder *tmp;
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    while (1)
    {
        j = i * 2 + 1;
        z = i * 2 + 2;
        smallest = i;

        if (j < heap->size && get_deadline(heap->data[j]) < get_deadline(heap->data[smallest]))
            smallest = j;
        if (z < heap->size && get_deadline(heap->data[z]) < get_deadline(heap->data[smallest]))
            smallest = z;
        if (smallest == i)
            break;
        
        tmp = heap->data[i];
        heap->data[i] = heap->data[smallest];
        heap->data[smallest] = tmp; 
        i = smallest;
    }
}

t_coder *get_min_heap(t_heap *heap)
{
    if (heap->size == 0)
        return NULL;
    return heap->data[0];
}


int take_dongles_edf(t_coder *coder)
{
    t_table *table = coder->table;
    pthread_mutex_lock(&table->queue_lock);
    add_to_heap(&table->heap, coder);
    while (!is_in_first_edf(&table->heap, coder->id, table->nb_coders) && !get_stop(table))
        pthread_cond_wait(&table->cond, &table->queue_lock);
    
    pthread_mutex_unlock(&table->queue_lock);
    if (get_stop(table))
        return 0;
    take_dongles(coder);
    return 1;
}

void release_dongles_edf(t_coder *coder)
{
    t_table *table = coder->table;
    give_dongles(coder);
    pthread_mutex_lock(&table->queue_lock);
    remove_min(&table->heap);
    pthread_cond_broadcast(&table->cond);
    pthread_mutex_unlock(&table->queue_lock);
}

int is_in_first_edf(t_heap *h, int id, int n)
{
    int i = 0;
    while (i < h->size && i < n)
    {
        if (h->data[i]->id == id)
            return (1);
        i++;
    }
    return (0);
}