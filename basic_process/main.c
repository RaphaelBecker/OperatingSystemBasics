#include <stdio.h>
// EXIT_FAILURE
#include <stdlib.h>
// noreturn
#include <stdnoreturn.h>
// wait()
#include <sys/wait.h>
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

noreturn void child(void)
{
    if (printf("Ich bin dein Kind PID: %i, PPID: %i\n", getpid(), getppid()) < 0)
    {
        err("printf");
    }
    readEndFromUserImput();

    if (printf("Kind mit PID: %i beendet\n", getpid()) < 0)
    {
        err("printf\n");
    }

    exit(EXIT_SUCCESS); // noreturn Funktion
}

void forknTimes(unsigned n)
{
    for (unsigned i = 0; i < n; ++i)
    {
        switch (fork()) // Rückgabewerte von fork(): Fehler = -1; Eltern = PID vom Kind (default im switch - case); Kind = 0;
        {
        case -1:
            err("fork\n");
        case 0:
            child();
        }
    }
}

void waitnTimes(unsigned n)
{
    for (unsigned i = 0; i < n; ++i)
    {
    retry:                  // label
        if (wait(NULL) < 0) /* wait() kann einen pointer uebergeben werden, der den Rückgabewert des Kindes überprüft. Rückgabewert PID des Kinds auf das gewartet wurde */
        {
            if (errno == EINTR) /* wait setzt errno bei Fehlerkondition: WNOHANG wurde nicht gesetzt und ein unblockiertes Signal oder eine SIGCHLD wurde abgefangen */
            {
                goto retry; // wiederhole bis wait() erfolgreich
            }
            err("wait");
        }
    }
}
int main(void) // C-Standard: Wenn in main kein Rückgabewert definiert wird 0 zurückgegeben
{
    // von stdin lesen
    unsigned n = promt_userinp("Enter process count: ");
    // n neue Prozesse starten
    printf("Hello from %i\n", getpid());   // Gibt Process Id vom Elternprozess zurück
    forknTimes(n);                         // Klont den Eltern Prozess; Kindprozess entsteht
    waitnTimes(n);                         /* wait() wartet auf Zustandsänderungen in einem Kind des aufrufenden Prozesses um Informationen über das Kind zu erhalten, dessen Zustand sich geändert hat. */
    printf("Goodbye from %i\n", getpid()); // Kind und elternprozess geben Ihre PID aus

    //Wartet auf die Terminierung der Kindsprozesse (waitn())
    if (printf("Elternprozess mit PID: %i beendet\n", getpid()) < 0)
    {
        err("printf\n");
    }
    return EXIT_SUCCESS;
}