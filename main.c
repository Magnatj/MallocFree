#include <stdio.h>
#include <stdlib.h>

char memory[8192];

typedef struct _memory_node
{
    struct _memory_node *next;
    size_t memsize;
}memory_node;

static memory_node *root = (memory_node*)(&memory[0]);

void *my_malloc(size_t requestedBytes)
{
    void *returnedPointer = NULL;

    if(root->memsize==0 && root->next==NULL && ((size_t)&memory + sizeof(memory)-((size_t)root+sizeof(memory_node)))>= requestedBytes)
    {
        returnedPointer = (void*)((size_t)root+sizeof(memory_node));
        root->memsize = requestedBytes;
    }

    else if(root->memsize==0 && root->next!=NULL && ((size_t)root->next - ((size_t)root+sizeof(memory_node))) >= requestedBytes)
    {
        returnedPointer = (void*)((size_t)root+sizeof(memory_node));
        root->memsize = requestedBytes;
    }

    else
    {
        memory_node *tmp = root;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
            if(tmp->memsize==0 && tmp->next == NULL && ((size_t)&memory + sizeof(memory)-((size_t)root+sizeof(memory_node)))>= requestedBytes)
            {
                returnedPointer = (void*)((size_t)tmp + sizeof(memory_node));
                tmp->memsize = requestedBytes;
            }

            else if(tmp->memsize==0 && tmp->next != NULL && ((size_t)tmp->next - ((size_t)tmp+sizeof(memory_node))) >= requestedBytes)
            {
                returnedPointer = (void *)((size_t)tmp + sizeof(memory_node));
                tmp->memsize = requestedBytes;
            }
        }

        if(returnedPointer==NULL && ((((size_t)&memory+sizeof(memory))- ((size_t)tmp+sizeof(memory_node)+tmp->memsize))-sizeof(memory_node)) >= requestedBytes)
        {
            returnedPointer = (void*)((size_t)tmp + (sizeof(memory_node)*2) + tmp->memsize);
            tmp->next = (memory_node*)((size_t)(returnedPointer)-sizeof(memory_node));
        }
    }
    return returnedPointer;
}

void my_free(void *exterminatePointer)
{
    memory_node * tmp = root;

    while(tmp->next != NULL)
    {
            if(((size_t)tmp+sizeof(memory_node)) == (size_t)exterminatePointer)
            {
                break;
            }
            else
            {
                tmp = tmp->next;
            }
    }

    if(((size_t)tmp+sizeof(memory_node)) == (size_t)exterminatePointer)
    {
        printf("Eliminando: %d\n", (int)((size_t)tmp+sizeof(memory_node)));
        root->memsize = 0;
    }
}

int main (void)
{
    int *DiosMeGuarde=my_malloc(sizeof(int));
    *DiosMeGuarde=100;

    int *AngelDeMiGuarda = my_malloc((sizeof(int)));
    *AngelDeMiGuarda=100;

    printf("%d\n%d\n",(int)root, (int)DiosMeGuarde);
    printf("%d\n%d\n",(int)root->next, (int)AngelDeMiGuarda);

    my_free(DiosMeGuarde);

    int *DiosMeGuardeReloaded = my_malloc(sizeof(int));
    *DiosMeGuardeReloaded=100;
    printf("%d\n%d\n",(int)root, (int)DiosMeGuardeReloaded);

    return 0;
}
