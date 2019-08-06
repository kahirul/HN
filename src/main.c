#include <curl/curl.h>
#include <json-c/json.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

#include "hn.h"
#include "client.h"

void *fetch_item_async(void *arg);

struct item_id_idx {
    char *id;
    int position;
};

void fetch_item(const char *id, int position);

int main(int argc, char const *argv[])
{

    if (argc == 2)
    {
        fetch_item(argv[1], 0);
    }
    else
    {
        char *url = "https://hacker-news.firebaseio.com/v0/topstories.json";
        struct json_object *json;
        int result_code = get_json(url, &json);

        if (result_code == 0)
        {
            size_t ids_len = json_object_array_length(json);
            char **ids = malloc(sizeof(char *) * ids_len);

            struct json_object *id;

            size_t topN = 8;
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
                struct item_id_idx *arg = malloc(sizeof(struct item_id_idx *));

                arg->id = (char *)malloc(strlen(ids[i]));
                strcpy(arg->id, ids[i]);
                arg->position = i;

                pthread_create(&threads[i], NULL, fetch_item_async, (void *)arg);
            }

            for (size_t i = 0; i < topN; i++)
            {
                pthread_join(threads[i], NULL);
            }

            json_object_put(json);
        }
    }

    return 0;
}

void fetch_item(const char *id, int position)
{
    char url[100];
    snprintf(url, 100, "https://hacker-news.firebaseio.com/v0/item/%s.json", id);

    struct json_object *json;
    int result_code = get_json(url, &json);

    if (result_code == 0)
    {
        struct item *item = parse_item(json);
        printf("\nFetching top story #%d\n", position);
        printItem(*item, true);
        json_object_put(json);
    }
}

void *fetch_item_async(void *arg)
{

    struct item_id_idx *item_idx = (struct item_id_idx *)arg;

    fetch_item(item_idx->id,item_idx->position);

    return 0;
}
