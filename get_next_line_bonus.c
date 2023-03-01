#include "get_next_line_bonus.h"

void    extract_line(t_lst stash, char **line)
{
    t_elem  *I;                                                     // Iteratore per scorrere la lista
    int     i;
    int     ln;

    ln = 0;
    I = stash->head;                                                // Comincio dalla testa della lista
    *line = (char *) malloc(sizeof(char) * (stash->chrs + 1));      // Alloco una riga di output che abbia il giusto numero di celle
    if (!*line)
        return;
    while (I)                                                       // Scorro la lista finché non termina
    {
        i = 0;
        while (I->cont[i] && I->cont[i] != '\n')                    // Scorro il content finché o non finisce o trovo uno \n
        {
            line[0][ln] = I->cont[i];                               // Copio la stringa del content nella riga di output
            i++;
            ln++;
        }
        if (I->cont[i] == '\n')                                         // Se il while è terminato a causa di uno \n
        {
            line[0][ln] = '\n';                                         // Posiziono uno \n nella riga di output
            ln++;
            stash->nwln = i + 1;
        }
        I = I->next;                                                    // Caratteri della lista - caratteri copiati
    }
    stash->buff = stash->chrs - ln;                                 // Il numero di caratteri post \n è uguale a
    line[0][ln] = '\0';                                                 // Null termino la riga di output
}

void    clean_stash(t_lst stash)
{
    int     i;
    t_elem  *nw;                                                    

    i = 0;
    if (stash->nwln)                                                    // Se è presente uno \n e quindi dell'overflow
    {
        nw = (t_elem *) malloc(sizeof(t_elem));                         // Creo un nuovo elemento di lista
        if (!nw)
            return;
        nw->cont = (char *) malloc(sizeof(char) * (stash->buff + 1));   // Alloco un content delle dimensioni corrette
        if (!nw->cont)
            return;
        nw->next = NULL;                                                
        nw->prev = NULL;
        stash->chrs = stash->buff;                                      // Aggiorno il numero di caratteri nella lista
        while (stash->tail->cont[stash->nwln + i])                      // Scorro il content dell'ultimo elemento
        {
            nw->cont[i] = stash->tail->cont[stash->nwln + i];            // Copio il post \n nel nuovo content
            i++;                                                        
        }
        nw->cont[i] = '\0';                                             // Null termino il nuovo content
    }
    free_stash(stash->head);                                            // Dealloco l'intera lista
    if (stash->nwln)
    {
        stash->head = nw;                                                   // nw è ora l'unico elemento della lista
        stash->tail = nw;
    }
    else
    {
        stash->head = NULL;
        stash->tail = NULL;
        stash->chrs = 0;
    }
    stash->nwln = 0;                                                    // Azzero il flag new_line
}

void    add_stash(t_lst stash, char *buf)
{
    int i;
    t_elem  *nw;


    nw = (t_elem *) malloc(sizeof(t_elem));                           // Alloca un nuovo elemento della lista
    if (!nw)    
        return;
    nw->cont = (char *) malloc(sizeof(char) * (ft_strlen(buf) + 1));  // Alloca una stringa nel content della lista
    if (!nw->cont)
        return;
    i = 0;
    while (buf[i])                                                    // Scorri la stringa del buffer
    {   
        nw->cont[i] = buf[i];                                         // Copia il carattere nel content della lista
        i++;
        stash->chrs++;                                                // Incrementa il contatore di caratteri della lista
    }
    nw->cont[i] = '\0';                                               // Null termina il nuovo content
    nw->next = NULL;                                                  // Inizializza i valori dell'elemento di lista
    nw->prev = stash->tail;                                           // Il precedente di nw è il vecchio ultimo elemento
    if (stash->tail != NULL)                                          // Se la lista non è vuota
        stash->tail->next = nw;                                       // Il successivo del vecchio ultimo diventa nw
    stash->tail = nw;                                                 // Ora nw è l'ultimo
    if (stash->head == NULL)                                          // Se la lista è vuota
        stash->head = nw;                                             // Ora nw è anche il primo elemento della lista
}

void    read_stash(int fd, t_lst stash)                         
{
    char    *buf;
    int     byte;

    byte = 1;
    while (!fnd_new_line(stash) && byte > 0)                    // Scorri il file finché non hai trovato uno \n o un EOF
    {
        buf = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));
        if (!buf)                                               
            return;
        byte = (int) read(fd, buf, BUFFER_SIZE);                // Conta i bytes letti
        if (byte == -1)
        {
            free(buf);
            return;
        }
        buf[byte] = '\0';                                       // NUll termina la stringa appena inserita nel buffer
        add_stash(stash, buf);                                  // Aggiungi la stringa del buffer nella lista
        free(buf);                                              // Dealloca il buffer
    }
}

char    *get_next_line(int fd)
{
    static t_lst  files[1024];
    t_lst         stash;
    char          *line;                                        // Riga di output
    int           tmp;

    if (fd < 0 || BUFFER_SIZE < 1 || read(fd, &line, 0) < 0)    // Error managment
        return (NULL);

    tmp = (files[fd] != NULL);

    if (tmp)
        stash = files[fd];
    else
        stash = nw_t_lst(fd, files);

    read_stash(fd, stash);                                      // Leggi e posiziona nella linked list le stringhe di buffer
    extract_line(stash, &line);                                 // Estrai la riga di output
    clean_stash(stash);                                         // Dealloca la lista, eventualmente mantenendo un elemento di over
    if (line[0] == '\0')
    {
        free(line);
        return (NULL);
    }
    return (line);                                              // Restituisci la riga di output
}