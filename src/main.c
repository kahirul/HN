#include <curl/curl.h>
#include <json-c/json.h>
#include <math.h>

#include "hn.h"
#include "client.h"

void fetch_item(const char *id);

int main(int argc, char const *argv[])
{

    if (argc == 2)
    {
        fetch_item(argv[1]);
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

            int topN = 8;
            if (ids_len < topN)
            {
                topN = ids_len;
            }

            for (size_t i = 0; i < ids_len; i++)
            {
                id = json_object_array_get_idx(json, i);
                ids[i] = (char *)json_object_get_string(id);
            }

            for (int i = 0; i < topN; i++)
            {
                fetch_item(ids[i]);
            }

            json_object_put(json);
        }
    }

    return 0;
}

void fetch_item(const char *id)
{
    char url[100];
    snprintf(url, 100, "https://hacker-news.firebaseio.com/v0/item/%s.json", id);

    struct json_object *json;
    int result_code = get_json(url, &json);

    if (result_code == 0)
    {
        struct item *item = parse_item(json);
        printItem(*item, true);
        json_object_put(json);
    }
}