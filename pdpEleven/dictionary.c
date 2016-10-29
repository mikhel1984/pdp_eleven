
#include "dictionary.h"
#include "common.h"

dict_t dictNew(void)
{
    dict_s proto = {0, 10, malloc(10 * sizeof(dict_entry_s))};
    dict_t d = malloc(sizeof(dict_s));
    *d = proto;
    return d;
}

void dictFree(dict_t dict)
{
    int i;
    for (i = 0; i < dict->len; i++)
    {
        free(dict->entry[i].key);
    }

    free(dict->entry);
    free(dict);
}

void dictAdd(dict_t dict, const char *key, int value)
{
   int idx = dictFindIndex(dict, key);
   if (idx != -1)
   {
       dict->entry[idx].value = value;
       return;
   }

   if (dict->len == dict->cap)
   {
       dict->cap *= 2;
       dict->entry = realloc(dict->entry, dict->cap * sizeof(dict_entry_s));
   }

   dict->entry[dict->len].key = strdup(key);
   dict->entry[dict->len].value = value;
   dict->len++;
}

int dictFind(dict_t dict, const char *key, int def)
{
    int idx = dictFindIndex(dict, key);
    return idx == -1 ? def : dict->entry[idx].value;
}

int dictFindIndex(dict_t dict, const char *key)
{
    for (int i = 0; i < dict->len; i++)
    {
        if (!strcmp(dict->entry[i].key, key))
        {
            return i;
        }
    }
    return -1;
}
