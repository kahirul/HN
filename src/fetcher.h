#include "hn.h"

struct fetch_item_arg
{
    char *id;
    int position;
};

struct item *fetch_item(const char *id, int position);

void *fetch_item_async(void *arg);

void fetch_top_stories(struct item **top_stories, size_t topN);
