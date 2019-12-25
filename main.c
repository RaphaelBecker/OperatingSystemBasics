#include <stdio.h>
// EXIT_FAILURE
#include <stdlib.h>
// noreturn
#include <stdnoreturn.h>
#include <pthread.h>
//errno
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define err(msg)            \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

#define err2(e, msg)        \
    do                      \
    {                       \
        errno = e;          \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

void postProcessStdIn(void) // Fehlerbehandlung technischer Natur
{
    do // bereinigt stdin indem es stdin leer konsuliert: getchar() ließt solange einzelne Zeichen ein bis EoL erreicht ist
    {
        if (feof(stdin)) // Fehlerkondition: Auf stdin liegt End of File
        {
            if (fprintf(stderr, "stdin: end of file\n") < 1)
            {
                err("error executing fprintf\n");
            }
            exit(EXIT_FAILURE);
        }
        if (ferror(stdin)) // Fehlerkondition: Auf stdin liegt ein Fehler an
        {
            err("stdin error\n");
        }
    } while (getchar() != 0x0a); //0x0a : \n (End of Line)
}
unsigned promt_userinp(const char *input) // Ließt den User input ein und prueft auf Inputfehler
{
    unsigned i;
    // handle errors?
    int done = 0;
    while (!done) // bis scanf erfolgreich ist
    {
        puts(input);        //puts gibt input mit Zeilenumbruch aus
        if (ferror(stdout)) // liegt Fehler auf stdout vor?
        {
            err("stdout error\n");
        }
        if (scanf("%u", &i) == 1) //scanf kann Eingabe in gewünschte Ausgabedatentypen parsen %u, pendant zu printf, &i Adresse zur Spzelle i, return 1 : ohne Fehler
        {
            done = 1; // scanf war erfolgreich
        }
        postProcessStdIn(); // StdIn nach scanf(..) fuer die spaetere Verarbeitung wieder aufbereiten
    }
    return i;
}

void readEndFromUserImput(void)
{
    int done = 0;
    char input[5];
    while (!done) // bis scanf erfolgreich ist
    {
        if (scanf("%3s", input) == 1 && !strcmp(input, "END")) // Erwarten 3 Zeichen + 1 Zeichen + EoL = 5; Wenn kein Uneterschied auf "END" rauskommt
        {
            done = 1; // scanf war erfolgreich
        }
        postProcessStdIn(); // StdIn nach scanf(..) fuer die spaetere Verarbeitung wieder aufbereiten
    }
}

void *start_routine(void *arg)
{
    (void)arg;                                              // cast um Compilerwarnung zu unterdruecken
    if (printf("Thread %lu started\n", pthread_self()) < 0) // Gibt Thread ID aus
    {
        err("printf");
    }
    readEndFromUserImput();

    if (printf("Thread %lu terminated\n", pthread_self()) < 0) // Gibt Thread ID aus
    {
        err("printf\n");
    }

    return NULL;
}

void pThreadCreate_nTimes(unsigned n, pthread_t *threads)
{
    for (unsigned i = 0; i < n; ++i)
    {
        int er;
        if ((er = pthread_create(&threads[i], NULL, start_routine, NULL))) /*  Die Funktion pthread_create() startet einen neuen Thread im aufrufenden Prozess.  Der neue Thread startet die Ausführung durch den Aufruf von start_routine(); arg ist Argument von start_routine(). */
        {
            err2(er, "ptherad_create");
        }
    }
}

void pThreadjoin_nTimes(unsigned n, const pthread_t *threads)
{
    for (unsigned i = 0; i < n; ++i)
    {
        int er;
        if ((er = pthread_join(threads[i], NULL))) /* Die Funktion pthread_join() wartet auf die Beendigung des von thread spezifizierten Threads.  Wenn dieser Thread bereits beendet ist, kehrt pthread_join() sofort zurück. (Zweites Argument waere ein Pointer um den Exitstatus des Threads zu speichern, hier NULL)*/
        {
            err2(er, "pthread_join");
        }
    }
}
int main(void) // C-Standard: Wenn in main kein Rückgabewert definiert wird 0 zurückgegeben
{
    // read n from stdin
    unsigned n = promt_userinp("Enter thread count: ");
    // start n new processes
    pthread_t *threads = calloc(n, sizeof(*threads));
    if (!threads)
    {
        err("calloc");
    }
    pThreadCreate_nTimes(n, threads); // Erstellt n Threads (Entspricht semantisch der Prozesserstellung)
    pThreadjoin_nTimes(n, threads);   // Die Funktion pthread_join() wartet auf die Beendigung des von thread spezifizierten Threads

    // Alle Threads sind terminiert
    puts("All threads joined\n");
    if (ferror(stdout))
    {
        err("stdout");
    }

    return EXIT_FAILURE;
}