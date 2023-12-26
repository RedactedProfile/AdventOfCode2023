#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef struct
{
    uint32_t dst;
    uint32_t src;
    uint32_t range;
}Tuple;

typedef struct
{
    size_t count;
    Tuple* data;
#ifdef DEBUG
    char id[64];
#endif
}Map;

#define LEX_EOL     -2
#define LEX_EOF     -1
#define LEX_TOKEN   1

int
lex(char* out, size_t token, char** script)
{
    int code = LEX_EOF;
    char* p = *script;
    /*skip white space*/
    while (*p <= ' ')
    {
        if (*p == '\n')
        {
            code = LEX_EOL;
            *out++ = *p++;
            goto Exit;
        }
        if (*p == '\0')
        {
            code = LEX_EOF;
            *out++ = *p++;
            goto Exit;
        }
        p++;
    }
    /*token*/
    while (1)
    {
        char c = *p;
        if (c <= ' ')
        {
            break;
        }
        if (token != 1)
        {
            *out++ = c;
            token--;
        }
        p++;
    }
    code = LEX_TOKEN;
Exit:
    *out = '\0';
    *script = p;
    return code;
}

char*
read(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (!fp)
    {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* data = malloc(size + 1);
    fread(data, size, 1, fp);
    data[size] = '\0';
    fclose(fp);
    return data;
}

unsigned
solve(unsigned x, Map* maps, int count_maps)
{
    Map* map = maps;
    for (int j = 0; j < 7; j++) /*only ever seven it seems, so just unroll*/
    {
        Tuple* tuple = map->data;
        size_t count = map->count;

        while (count >= 2)
        {
            int delta = x - tuple->src;
            if ((delta >= 0) & (delta < tuple->range))
            {
                x = tuple->dst + delta;
                goto Next;
            }
            tuple++;

            delta = x - tuple->src;
            if ((delta >= 0) & (delta < tuple->range))
            {
                x = tuple->dst + delta;
                goto Next;
            }
            tuple++;
            count -= 2;
        }

        while (count--)
        {
            int delta = x - tuple->src;
            if ((delta >= 0) & (delta < tuple->range))
            {
                x = tuple->dst + delta;
                goto Next;
            }
            tuple++;
        }
    Next:
        map++;
    }
    return x;
}

int
main(int argc, char** argv)
{
    if (argc <= 1)
    {
        return EXIT_FAILURE;
    }
    char* script = read(argv[1]);
    if (!script)
    {
        return EXIT_FAILURE;
    }

    int count = 0;
    unsigned* seeds = NULL;

    int count_maps = 0;
    Map* maps = NULL;

    char* p = script;
    char token[64];
    char prevtoken[64];
    while (lex(token, sizeof token, &p) != LEX_EOF)
    {
        /*Parse seeds*/
        if (!strcmp("seeds:", token))
        {
            while (lex(token, sizeof token, &p) != LEX_EOL)
            {
                seeds = realloc(seeds, ++count * sizeof(seeds[0]));
                seeds[count - 1] = atoi(token);
            }
#ifdef DEBUG
            for (int i = 0; i < count; i++)
            {
                printf("seed %i: %u\n", i, seeds[i]);
            }
#endif
        }

        /*Parse map*/
        if (!strcmp("map:", token))
        {
            maps = realloc(maps, ++count_maps * sizeof(maps[0]));
            Map* map = &maps[count_maps - 1];
            map->count = 0;
            map->data = NULL;
#ifdef DEBUG
            strcpy(map->id, prevtoken);
#endif
            if (lex(token, sizeof token, &p) != LEX_EOL);
#ifdef DEBUG
            printf("%s\n", map->id);
#endif            
            while (lex(token, sizeof token, &p) != LEX_EOF)
            {
                unsigned
                    a = 0,
                    b = 0,
                    c = 0;

                /*End of map?*/
                if (token[0] == '\n')
                {
                    break;
                }
                /*Parse tuple*/
                a = atoi(token);
                if (lex(token, sizeof token, &p) != LEX_TOKEN);
                b = atoi(token);
                if (lex(token, sizeof token, &p) != LEX_TOKEN);
                c = atoi(token);
                if (lex(token, sizeof token, &p) != LEX_EOL);

                map->data = realloc(map->data, ++map->count * sizeof(map->data[0]));
                Tuple* tuple = &map->data[map->count - 1];
                tuple->dst = a;
                tuple->src = b;
                tuple->range = c;
            }
#ifdef DEBUG
            for (int i = 0; i < map->count; i++)
            {
                Tuple* tuple = map->data + i;
                printf("%i: %u %u %u\n", i, tuple->dst, tuple->src, tuple->range);
            }
#endif
        }

        /*Copy old token*/
        strcpy(prevtoken, token);
    }


    {/*single*/
        unsigned lowest = ~0; /*Max*/
        for (int i = 0; i < count; i++)
        {
            unsigned x = solve(seeds[i], maps, count_maps);
            if (x < lowest)
            {
                lowest = x;
            }
        }

        printf("lowest is %u\n", lowest);
    }

    {/*stupid*/
        clock_t start = clock();
        unsigned lowest = ~0; /*Max*/
        for (int i = 0; i < count; i += 2)
        {
            unsigned start = seeds[i];
            unsigned end = seeds[i + 1];
            while (end >= 1)
            {
                unsigned x = solve(start++, maps, count_maps);
                if (x < lowest)
                {
                    lowest = x;
                }
                end--;
            }
        }
        clock_t end = clock();
        printf("lowest is %u\n", lowest);
        printf("time taken: %g seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    }

    return EXIT_SUCCESS;
}