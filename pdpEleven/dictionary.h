
#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct dict_entry_s
{
    const char *key;
    int value;
} dict_entry_s;

typedef struct dict_s
{
    int len;
    int cap;
    dict_entry_s *entry;
} dict_s, *dict_t;

extern dict_t dictNew(void);
extern void dictFree(dict_t dict);

extern void dictAdd(dict_t dict, const char *key, int value);

extern int dictFind(dict_t dict, const char *key, int def);
extern int dictFindIndex(dict_t dict, const char *key);

#endif // DICTIONARY_H
