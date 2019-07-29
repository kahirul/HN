#include <curl/curl.h>
#include <json-c/json.h>

#include "hn.h"
#include "client.h"

int main(int argc, char const *argv[])
{

    if (argc == 2)
    {
        char url[100];
        snprintf(url, 100, "https://hacker-news.firebaseio.com/v0/item/%s.json", argv[1]);

        struct json_object *json;
        int result_code = get_json(url, &json);

        if (result_code == 0)
        {
            struct item *item = parse_item(json);
            printItem(*item);
            json_object_put(json);
        }
    } else {
        char *url = "https://hacker-news.firebaseio.com/v0/topstories.json";
        struct json_object *json;
        int result_code = get_json(url, &json);

        if (result_code == 0)
        {
            // struct item *item = parse_item(json);
            // printItem(*item);
            size_t ids_len = json_object_array_length(json);
            char **ids = malloc(sizeof(char *) * ids_len);

            struct json_object *id;

            for (size_t i = 0; i < ids_len; i++)
            {
                id = json_object_array_get_idx(json, i);
                ids[i] = (char *)json_object_get_string(id);
            }

            // printf("%s\n", json_object_to_json_string(json));
            json_object_put(json);
        }

    }

    return 0;
}