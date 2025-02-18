/****************************************************************************************************
 * Include
 ****************************************************************************************************/

#include <errno.h>
#include "obj.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************************************
 * Function Definition (Public)
 ****************************************************************************************************/

/*** Destroy ***/
void obj_destroy(obj_object* const object)
{
    free(object->face);
    free(object->vertex);
}

/*** Parse ***/
bool obj_parse(const char* const Path, obj_object* const object)
{
    FILE* file;
    int a, b, c, dummy, iFace, iVertex;
    char line[1024];
    float x, y, z;

    /*** Parse ***/
    /* Open */
    errno = 0;
    if((file = fopen(Path, "r")) == NULL)
    {
        (void)printf("fopen Failed: %d (%s)\n", errno, strerror(errno));
        return false;
    }

    /* Set Up */
    iFace = 0;
    object->nFace = 0;
    object->face = NULL;
    iVertex = 0;
    object->nVertex = 0;
    object->vertex = NULL;

    /* Count */
    while(fgets(line, sizeof(line), file))
    {
        /* Count */
        if(strncmp(line, "f ", 2) == 0)
        {
            /* Face */
            object->nFace++;
        }
        else if(strncmp(line, "v ", 2) == 0)
        {
            /* Vertex */
            object->nVertex++;
        }
    }
    rewind(file);

    /* Memory */
    object->face = malloc(object->nFace * sizeof(*(object->face)));
    object->vertex = malloc(object->nVertex * sizeof(*(object->vertex)));

    /* Data */
    while(fgets(line, sizeof(line), file))
    {
        /* Data */
        if(strncmp(line, "f ", 2) == 0)
        {
            /* Face */
            if(sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &dummy, &dummy, &b, &dummy, &dummy, &c, &dummy, &dummy) == 9)
            {
                /* Success */
                object->face[iFace].a = a - 1; // Indices Start At 1 In Object File
                object->face[iFace].b = b - 1;
                object->face[iFace].c = c - 1;
                iFace++;
            }
            else
            {
                /* Error */
                free(object->face);
                free(object->vertex);
                return false;
            }
        }
        else if(strncmp(line, "v ", 2) == 0)
        {
            /* Vertex */
            if(sscanf(line, "v %f %f %f", &x, &y, &z) == 3)
            {
                /* Success */
                object->vertex[iVertex].x = x;
                object->vertex[iVertex].y = y;
                object->vertex[iVertex].z = z;
                iVertex++;
            }
            else
            {
                /* Error */
                free(object->face);
                free(object->vertex);
                return false;
            }
        }
    }
    
    /* Close */
    (void)fclose(file);

    return true;
}
