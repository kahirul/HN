#include <pthread.h>
#include <string.h>
#include "fetcher.h"
#include "client.h"

struct item *fetch_item(const char *id, int position)
{
    printf("Fetching story #%d\n", position);
    char url[100];
    snprintf(url, 100, "https://hacker-news.firebaseio.com/v0/item/%s.json", id);

    struct json_object *json;
    int result = get_json(url, &json);

    struct item *item = NULL;
    if (result == 0)
    {
        item = parse_item(json);
        json_object_put(json);
    }

    return item;
}

void *fetch_item_async(void *arg)
{
    struct fetch_item_arg *item_idx = (struct fetch_item_arg *)arg;
    struct item *item = fetch_item(item_idx->id, item_idx->position);

    return (void *)item;
}

void fetch_top_stories(struct item **top_stories, size_t topN)
{
    char *url = "https://hacker-news.firebaseio.com/v0/topstories.json";
    struct json_object *json;
    int result_code = get_json(url, &json);

    if (result_code == 0)
    {
        size_t ids_len = json_object_array_length(json);
        char **ids = malloc(sizeof(char *) * ids_len);

        struct json_object *id;

        if (ids_len < topN)
        {
            topN = ids_len;
        }

        for (size_t i = 0; i < ids_len; i++)
        {
            id = json_object_array_get_idx(json, i);
            ids[i] = (char *)json_object_get_string(id);
        }

        pthread_t threads[topN];

        for (size_t i = 0; i < topN; i++)
        {
            struct fetch_item_arg *arg = malloc(sizeof(struct fetch_item_arg *));

            arg->id = (char *)malloc(strlen(ids[i]));
            strcpy(arg->id, ids[i]);
            arg->position = i;

            pthread_create(&threads[i], NULL, fetch_item_async, (void *)arg);
        }

        for (size_t i = 0; i < topN; i++)
        {
            void *item;
            pthread_join(threads[i], &item);
            top_stories[i] = (struct item *)item;
        }

        json_object_put(json);
    }
}
