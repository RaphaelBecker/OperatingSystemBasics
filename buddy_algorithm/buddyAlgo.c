#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

// define error macro
#define err(msg)            \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

struct node
{
    long data;
    int numAlloc;
    int buddy;
    // long occupiedBlocks;
    bool usedflag, splitflag;
    struct node *left;
    struct node *right;
    char PFarray[];
};
// framework
bool debugmode = 0;
char inputBuf[10];
int splitFlagDone = 0, usedFlagDone = 0;
long BLOCKSIZE = 4;   // -b
long NUM_BLOCKS = 32; // -n
long mReserved = 0;   // m
long fReserved = 0;   // f
int init = 0;
int wasFreed = 0;
int numAllocated = 0;
int buddyCounter = 0;
int tempDeleteBuddy[2] = {-1, -99999};
int deleteKey = -1;
int deletionCounter = 0;
int deletionCounterDeletionNode = 0;
int deleteFlag = 0;
// tree
bool isNumber = true;
int reserveBuffer[10];
int reserveCounter = 1;
long mostLittleData = 100000;

void print_help(char *executable_name)
{
    printf("Usage: %s [-b BLOCKSIZE] [-n NUM_BLOCKS]\n", executable_name);
    puts("BLOCKSIZE: integer, interpreted as kiB (i.e. -b 4 => 4096 Bytes). Has to be power of 2!");
    puts("NUM_BLOCKS: integer, number of blocks to be used. Has to be power of 2!");
}

const char *MENU = "Type your command, then [enter]\n"
                   "Available commands:\n"
                   "m SIZE\t\tallocate SIZE Bytes of memory. SIZE has to be > 0.\n"
                   "f POINTER\tfree the memory referenced by POINTER. If it is invalid, an error is shown and nothing else happens.\n"
                   "l\t\tprint current memory layout.\n"
                   "q\t\tquit the program.";

void print_menu()
{
    puts(MENU);
}

void clearBuffer(void)
{
    for (int i = 0; i < (int)sizeof(inputBuf); i++)
    {
        inputBuf[i] = '\n';
    }
}

void checkForDigit(char **arg, int argD, int type)
{
    int i = 0;
    while (arg[argD][i] != '\0')
    {
        if (!isdigit(arg[argD][i]))
        {
            isNumber = false;
        }
        i++;
    }
    if (isNumber == false)
    {
        print_help(arg[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (type == 0)
            BLOCKSIZE = atoi(arg[argD]) * 1024;
        if (type == 1)
            NUM_BLOCKS = atoi(arg[argD]);
    }
}

// function converts decimal to binary and checks if number is 2^X
void ckeckPotOfTwo(char **arg, int n)
{
    // array to store binary number
    int binNum[32];
    n = atoi(arg[n]);
    // counter for binary array
    int i = 0;
    while (n > 0)
    {
        // storing remainder in binary array
        binNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    // checks if binary has more tha one "1". If so -> Not 2^X number
    int vCounter = 0;
    for (int j = i - 1; j >= 0; j--)
    {
        if (binNum[j] == 1)
        {
            vCounter++;
        }
        if (vCounter > 1)
        {
            print_help(arg[0]);
            exit(EXIT_FAILURE);
        }
    }
}

bool mfReserve(char *inputBuf)
{
    // Extract the first token
    char *token = strtok(inputBuf, " "); // saves 1sr token = "m", delimited by " "
    char *mf = token;
    token = strtok(NULL, " "); // saves 2nd token = "number of bytes", delimited by " "

    if (token != NULL) // if token 2 exists?
    {
        if (strcmp(mf, "m") == 0)
        {
            mReserved = atol(token);
            if (debugmode == 1)
                printf("functionmReserve mReserved %ld\n", mReserved);
        }
        else if (strcmp(mf, "f") == 0)
        {
            fReserved = atol(token);
        }

        token = strtok(NULL, " "); // exists 3rd token? -> error
        if (token != NULL)
            return false;
    }
    else
    {
        return false;
    }
    return true;
}

void convertmReserved(void)
{
    // how many blocks are gonna used for mReserved?
    long temp = 0;
    temp = mReserved / (BLOCKSIZE * 1024);
    if (debugmode == 1)
        printf("convertmReserved temp: %ld\n", temp);
    if (mReserved % (BLOCKSIZE * 1024) != 0 || mReserved == 0)
    {
        temp++;
    }
    if (debugmode == 1)
        printf("Modulo: %ld\n", (mReserved % (BLOCKSIZE * 1024)));
    mReserved = temp;
}

int numLoops(void)
{
    if (debugmode == 1)
        printf("num of used Bolcks == mREserved: %ld\n", mReserved);
    int pot = 0, temp = NUM_BLOCKS, potMax = 0;
    while (pot != 1)
    {
        pot = temp / 2;
        temp = pot;
        potMax++;
    }
    if (debugmode == 1)
        printf("potMax: %d\n", potMax);
    int loopCounter = 0, potCounter = potMax;
    while (mReserved <= (long)pow(2.0, potCounter))
    {
        //printf("pow: %ld\n", (long)pow(2.0, potCounter));
        loopCounter++;
        potCounter--;
    }
    if (debugmode == 1)
        printf("Number of Iterations: %d\n", loopCounter - 1);
    return loopCounter - 1;
}

/* newNode() allocates a new node with the given data and NULL left and 
right pointers. */
struct node *newNode(int dataa, bool use)
{
    // Allocate memory for new node
    struct node *node = (struct node *)malloc(sizeof(struct node) + (sizeof(char *) * (dataa / BLOCKSIZE) + (2 * sizeof(char *))));

    // Assign data to this node
    node->data = dataa;
    node->numAlloc = 0;
    node->usedflag = use;
    node->splitflag = false;
    node->buddy = buddyCounter;

    for (int i = 0; i < (node->data); i++)
    {
        node->PFarray[i] = 'F';
    }
    // Initialize left and right children as NULL
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void mostLitteDataToBeSplitted(struct node *root)
{

    // if node is null, return
    if (!root)
        return;

    // if node is leaf node:
    if (!root->left && !root->right)
    {

        if ((mReserved <= root->data / 2) && (root->usedflag == false) && splitFlagDone == 1) // TODO NUR KLEINST MÖGLICHE NODE MIT SPLITFLAG VERSEHEN root->data == mlpData
        {
            if (mostLittleData >= root->data)
                mostLittleData = root->data;
            if (debugmode == 1)
                printf("root->data: %ld, mostLitteData: %ld\n", root->data, mostLittleData);
            return;
        }
    }

    // printf(" strlen: %d. ", (int)strlen(root->PFarray)); // DEBUGGING

    // if left child exists, check for leaf
    // recursively
    if (root->left)
        mostLitteDataToBeSplitted(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        mostLitteDataToBeSplitted(root->right);
}

// function will set SPlitFlag if Leaf Node is to be splitted
void setSplitFlag(struct node *root)
{

    // if node is null, return
    if (!root)
        return;

    // if node is leaf node:
    if (!root->left && !root->right)
    {
        if (mReserved <= root->data / 2)
        {
            if ((mReserved <= root->data / 2) && (root->usedflag == false))
            {
                if ((mReserved <= root->data / 2) && (root->usedflag == false) && splitFlagDone == 1 && root->data == mostLittleData) // TODO NUR KLEINST MÖGLICHE NODE MIT SPLITFLAG VERSEHEN root->data == mlpData
                {
                    root->splitflag = true;
                    splitFlagDone = 0;
                    mostLittleData = 100000;
                    return;
                }
            }
        }
    }

    // printf(" strlen: %d. ", (int)strlen(root->PFarray)); // DEBUGGING

    // if left child exists, check for leaf
    // recursively
    if (root->left)
        setSplitFlag(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        setSplitFlag(root->right);
}

// function will set UsedFlag if Leaf Node is to be allocated
void setUsedFlag(struct node *root)
{

    // if node is null, return
    if (!root)
        return;

    // if node is leaf node:
    if (!root->left && !root->right)
    {
        if (((root->data / mReserved) == 1) && (root->usedflag == false) && usedFlagDone == 1)
        {
            root->usedflag = true;
            numAllocated = numAllocated + 1;
            root->numAlloc = numAllocated;
            usedFlagDone = 0;
            for (int i = 0; i < root->data; i++)
            {
                root->PFarray[i] = 'P';
            }
            return;
        }
    }

    // recursively
    if (root->left)
        setUsedFlag(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        setUsedFlag(root->right);
}

// function will split all leaf nodes with SplitFlag == true
void splitLeafNode(struct node *root)
{
    // if node is null, return
    if (!root)
        return;

    // if node is leaf node, create left and right node
    //    if ((root->data / 2) >= 1 && !root->left && !root->right && root->usedflag == false && root->splitflag == true)
    if ((root->data / 2) >= 1 && !root->left && !root->right && root->usedflag == false && root->splitflag == true)
    {
        long newdata = root->data / 2;
        //printf("newdata: %ld\n", newdata); // FOR DEBUGGING
        if (debugmode == 1)
            printf("buddy counter: %d \n", buddyCounter);
        root->left = newNode(newdata, false);
        root->right = newNode(newdata, false);
        buddyCounter = buddyCounter + 1;
        root->usedflag = false;
        root->splitflag = false;
        return;
    }

    // if left child exists, check for leaf
    // recursively
    if (root->left)
        splitLeafNode(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        splitLeafNode(root->right);
}

// prints all nodes from left to right
void printLeafNodes(struct node *root)
{
    // if node is null, return
    if (!root)
        return;

    // if node is leaf node, print its data
    if (!root->left && !root->right && root != NULL)
    {
        // printf("Node: data: %ld, ", root->data); // DEBUGGING

        // change pfarray from F (free) to P (allocated) based on usedflag and data

        // print
        if (debugmode == true)
        {
            printf("[%s] d%ld u%d sf%d nA%d b%d || ", root->PFarray, root->data, root->usedflag, root->splitflag, root->numAlloc, root->buddy);
        }
        else
        {
            printf("%s", root->PFarray);
        }
        // printf(" strlen: %d. ", (int)strlen(root->PFarray)); // DEBUGGING
        return;
    }

    // if left child exists, check for leaf
    // recursively
    if (root->left)
        printLeafNodes(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        printLeafNodes(root->right);
}

// changes P to F is possible by given f X, sets usedFlag = false
void freeMemoryByKey(struct node *root)
{
    // if node is null, return
    if (!root)
        return;

    // if node is leaf node
    if (!root->left && !root->right && root->numAlloc == fReserved)
    {
        // Assign data to this node
        root->numAlloc = 0;
        root->usedflag = false;
        root->splitflag = false;

        for (int i = 0; i < (root->data); i++)
        {
            root->PFarray[i] = 'F';
        }
        wasFreed = 1;
        return;
    }

    // if left child exists, check for leaf
    // recursively
    if (root->left)
        freeMemoryByKey(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        freeMemoryByKey(root->right);
}

// changes P to F is possible by given f X, sets usedFlag = false

struct node *deleteNodesByKey(struct node *root)
{
    if (root == NULL)
        return NULL;
    if (root->left == NULL && root->right == NULL && root->buddy == deleteKey && root->data != NUM_BLOCKS)
    {
        free(root);
        return NULL;
    }

    // Else recursively delete in left and right
    // subtrees.
    root->left = deleteNodesByKey(root->left);
    root->right = deleteNodesByKey(root->right);

    return root;
}

// determines if nodes can be deleted. deletion in function
void determineDeletionOfNodes(struct node *root)
{
    // if node is null, return
    if (!root)
        return;

    // if node is leaf node
    if (!root->left && !root->right && root->usedflag == false && root->buddy != 0)
    {
        tempDeleteBuddy[deletionCounter] = root->buddy;
        //printf("tempDeleteBuddy[deletionCounter]: %d\n", tempDeleteBuddy[deletionCounter]);
        deletionCounter++;
        if (deletionCounter > 1)
            deletionCounter = 0;
        if (tempDeleteBuddy[0] == tempDeleteBuddy[1])
        {
            // TODO: Delete Nodes with root->buddy == tempDeleteBuddy!
            deleteFlag = 1;
            deleteKey = tempDeleteBuddy[0];
            if (debugmode == 1)
                printf("delete Nodes by same key: %d, %d\n", tempDeleteBuddy[0], tempDeleteBuddy[1]);
        }
        return;
    }

    // if left child exists, check for leaf
    // recursively
    if (root->left)
        determineDeletionOfNodes(root->left);

    // if right child exists, check for leaf
    // recursively
    if (root->right)
        determineDeletionOfNodes(root->right);
}

// allocates Memory if possible by m X
void allocateMemory(struct node *root)
{
    convertmReserved();
    int maxLoops = numLoops();
    if (maxLoops == 0)
    {
        usedFlagDone = 1;
        setUsedFlag(root);
        if (usedFlagDone == 0)
        {
            fprintf(stdout, "success\n");
        }
        else
        {
            fprintf(stdout, "error\n");
        }
    }

    if (init == 0) //INIT: only first iteration
    {
        for (int y = 0; y < maxLoops; y++)
        {
            if (debugmode == 1)
                printf("\n %d ------- (%d) \n", y + 1, init);
            splitFlagDone = 1;
            usedFlagDone = 1;
            if (debugmode == 1)
                printf("\nsetUsedFlag init:\n");
            setUsedFlag(root);
            if (debugmode == 1)
                printLeafNodes(root);
            if (debugmode == 1)
                printf("\nsettedSplitFlag init:\n");
            mostLitteDataToBeSplitted(root);
            setSplitFlag(root);
            if (debugmode == 1)
                printLeafNodes(root);
            if (debugmode == 1)
                printf("\nsplitLeaveNodes init:\n");
            splitLeafNode(root);
            if (debugmode == 1)
                printLeafNodes(root);

            if (y == maxLoops - 1)
            {
                if (debugmode == 1)
                    printf("\nsetUsedFlag init:\n");
                //numAllocated++;
                setUsedFlag(root);
                if (debugmode == 1)
                    printLeafNodes(root);
                if (usedFlagDone == 0)
                {
                    fprintf(stdout, "success\n");
                }
                else
                {
                    fprintf(stdout, "error\n");
                }
            }
        }
        if (debugmode == 1)
            printf("\n End of memory allocation init");

        clearBuffer();
        init++;
    }
    else // all memory allocation after INIT
    {
        for (int y = 0; y < maxLoops; y++)
        {
            if (debugmode == 1)
                printf("\n %d ------- (%d) \n", y + 1, init);
            splitFlagDone = 1;
            usedFlagDone = 1;
            if (debugmode == 1)
                printf("\nsetUsedFlag:\n");
            setUsedFlag(root);
            if (usedFlagDone == 0)
            {
                fprintf(stdout, "success\n");
            }
            if (usedFlagDone == 1)
            {
                if (debugmode == 1)
                    printf("\nsettedSplitFlag:\n");
                mostLitteDataToBeSplitted(root);
                setSplitFlag(root);
                if (debugmode == 1)
                    printLeafNodes(root);
                if (debugmode == 1)
                    printf("\nsplitLeaveNodes:\n");
                splitLeafNode(root);
                if (debugmode == 1)
                    printLeafNodes(root);

                if (y == maxLoops - 1)
                {
                    if (debugmode == 1)
                        printf("\nsetUsedFlag:\n");
                    //numAllocated++;
                    setUsedFlag(root);
                    if (debugmode == 1)
                        printLeafNodes(root);
                    if (usedFlagDone == 0)
                    {
                        fprintf(stdout, "success\n");
                    }
                    else
                    {
                        fprintf(stdout, "error\n");
                    }
                }
            }
            else
            {
                y = maxLoops;
                if (debugmode == 1)
                    printLeafNodes(root);
                //numAllocated++;
            }
        }
        if (debugmode == 1)
            printf("\n End of memory allocation");

        clearBuffer();
    }
}

int main(int argc, char *argv[])
{
    char *arg[argc]; // save commandline arguemtns in char array
    struct node *root = newNode(NUM_BLOCKS, false);
    buddyCounter = buddyCounter + 1;

    for (int i = 0; i < argc; i++)
    {
        arg[i] = argv[i];
    }

    if (argc == 1 || argc == 3 || argc == 5)
    {
        if (argc == 1)
        {
            // Fall argc == 1: Initialwerte setzen
            BLOCKSIZE = 4;   // -b
            NUM_BLOCKS = 32; // -n
        }
        else if (((strcmp("-b", argv[1]) || (strcmp("-n", argv[1]) == 0)) == 0) && argc == 3)
        {
            // argv[2] soll auf Zahlen geprüft werden.
            // m BLOCKSIZE wird gesetzt, NUM_BLOCKS = 32 (Initialwert)
            checkForDigit(arg, 2, 0); // IO:  BLOCKSIZE = arg[2] * 1024
            ckeckPotOfTwo(arg, 2);    // NIO: print_help UND exit(EXIT_FAILURE)
        }

        else if (strcmp("-b", argv[1]) == 0 && strcmp("-n", argv[3]) == 0 && argc == 5)
        {
            // m = arg[2] und f = arg[4]
            // IO:  BLOCKSIZE = arg[2] * 1024; BLOCK_COUNT = arg[4];
            // NIO: print_help UND exit(1)
            checkForDigit(arg, 2, 0);
            ckeckPotOfTwo(arg, 2);
            checkForDigit(arg, 4, 1);
            ckeckPotOfTwo(arg, 4);
        }

        print_menu();
        clearBuffer();
        // Execute programms TODO
        while (1) // read and execute commands
        {
            if (fgets(inputBuf, sizeof(inputBuf), stdin) != NULL) // read input chars
            {
                if (strncmp("q", inputBuf, 1) == 0) // Exit command "q"
                {
                    return EXIT_SUCCESS;
                }
                else if (strncmp("l", inputBuf, 1) == 0) // print memory layout
                {
                    printLeafNodes(root);
                    printf("\n");
                }
                else if (strncmp("m", inputBuf, 1) == 0)
                {
                    // Allocate mReserved Bytes of memory
                    if (mfReserve(inputBuf) == true && mReserved > 0 && mReserved <= (NUM_BLOCKS * BLOCKSIZE * 1024))
                    {
                        allocateMemory(root);
                    }
                    else
                    {
                        fprintf(stdout, "error\n"); // Cok
                    }
                }
                else if (mfReserve(inputBuf) == true && strncmp("f", inputBuf, 1) == 0)
                {
                    // f POINTER       free the memory referenced by POINTER. If it is invalid, an error is shown and nothing else happens.
                    if (debugmode == 1)
                        printf("deletekey: %d\n", deleteKey);
                    freeMemoryByKey(root);
                    if (wasFreed == 0)
                    {
                        if (fReserved > 0)
                        {
                            fprintf(stdout, "Could not free %ld - number is invalid!\n", fReserved);
                        }
                    }
                    else // Key is valid
                    {
                        wasFreed = 0;
                        determineDeletionOfNodes(root); // deleteFlag = 1;
                    }
                    while (deleteFlag == 1)
                    {
                        determineDeletionOfNodes(root); //  deleteFlag = 1;
                        if (deleteFlag == 1)
                        {
                            if (debugmode == 1)
                                printf("deleteKey: %d\n", deleteKey);
                            if (fReserved > 0)
                            {
                                deleteNodesByKey(root); // if (root->left == NULL && root->right == NULL && root->buddy == deleteKey && root->data != NUM_BLOCKS)
                                deleteFlag = 0;
                                buddyCounter = buddyCounter - 1;
                            }
                        }
                        determineDeletionOfNodes(root); //  deleteFlag = 1;
                    }

                    // IN CASE OF ERROR PRINT: In function freeMemoryByKey -> If Key was not found in leaf nodes, print error message
                }
                else
                {
                    print_help(argv[0]);
                    return EXIT_FAILURE;
                }
            }
            else
            {
                err("scanf error\n");
            }
            clearBuffer();
        }
    }
    else if (argc == 2 && !strcmp("-h", argv[1]))
    {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }
    else
    {
        print_help(argv[0]);
        return EXIT_FAILURE;
    }
}
