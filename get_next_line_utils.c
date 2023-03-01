#include "get_next_line.h"

void    free_stash(t_elem *I)                                               // Dealloca ricorsivamente gli elementi della stringa
{       
    if (!I)
        return;
    free_stash(I->next);
    free(I->cont);
    free(I);
}

int     ft_strlen(char *s)
{
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

int     fnd_new_line(t_lst stash)
{
    int i;

    if (stash->head == NULL)                                                // Se la lista è vuota return non c'é uno \n
        return (0);
    i = 0;
    while (stash->tail->cont[i] && stash->tail->cont[i] != '\n')            // Scorri l'ultima stringa finchè non trovi uno \n
        i++;
    if (!stash->tail->cont[i])                                              // Se sei uscito dal while perché la stringa è finita
        return (0);                                                         // Non c'era uno \n
    return (1);                                                             // Altrimenti c'era
}