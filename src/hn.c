#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>

#include "hn.h"

char *join(char **collection, size_t col_size, char *delimiter)
{
    char *buffer = malloc(sizeof(char) * 500);
    buffer[0] = '\0';

    int len = 0;
    for (size_t i = 0; i < col_size; i++)
    {
        strncat(buffer, collection[i], strlen(collection[i]));
        strncat(buffer, delimiter, strlen(delimiter));
        len += strlen(collection[i]);
        len += 2;
    }
    buffer[len - strlen(delimiter)] = '\0';

    return buffer;
}

struct item *createItem(int id, const char *type, const char *by, int time)
{
    struct item *newItem = malloc(sizeof(struct item));
    newItem->id = id;

    newItem->type = malloc(strlen(type) + 1);
    strcpy(newItem->type, type);

    newItem->by = malloc(strlen(by) + 1);
    strcpy(newItem->by, by);

    newItem->time = time;

    return newItem;
}

void setText(struct item *item, const char *text)
{
    item->text = malloc(strlen(text) + 1);
    strcpy(item->text, text);
}

void setParent(struct item *item, int parent)
{
    printf("%d\n", parent);
    item->parent = parent;
}

void setKids(struct item *item, char **kids, size_t kids_len)
{
    item->kids = malloc(sizeof(kids) * kids_len);
    for (size_t i = 0; i < kids_len; i++)
    {
        item->kids[i] = malloc(strlen(kids[i]) + 1);
        strcpy(item->kids[i], kids[i]);
    }
    item->kids_len = kids_len;
}

void setURL(struct item *item, const char *url)
{
    item->url = malloc(strlen(url) + 1);
    strcpy(item->url, url);
}

void setScore(struct item *item, int score)
{
    item->score = score;
}

void setTitle(struct item *item, const char *title)
{
    item->title = malloc(strlen(title) + 1);
    strcpy(item->title, title);
}

void setDescendants(struct item *item, int descendants)
{
    item->descendants = descendants;
}

struct item *parse_item(struct json_object *json)
{
    struct json_object *tmp;

    json_object_object_get_ex(json, "id", &tmp);
    int id = json_object_get_int(tmp);

    json_object_object_get_ex(json, "time", &tmp);
    int time = json_object_get_int(tmp);

    json_object_object_get_ex(json, "by", &tmp);
    const char *by = json_object_get_string(tmp);

    json_object_object_get_ex(json, "type", &tmp);
    const char *type = json_object_get_string(tmp);

    struct item *item = createItem(id, type, by, time);

    if (json_object_object_get_ex(json, "text", &tmp))
    {
        setText(item, json_object_get_string(tmp));
    }

    if (json_object_object_get_ex(json, "parent", &tmp))
    {
        setParent(item, json_object_get_int(tmp));
    }

    json_object_object_get_ex(json, "kids", &tmp);
    size_t kids_len = json_object_array_length(tmp);

    char **kids = malloc(sizeof(char *) * kids_len);
    struct json_object *kid;

    for (size_t i = 0; i < kids_len; i++)
    {
        kid = json_object_array_get_idx(tmp, i);
        kids[i] = (char *)json_object_get_string(kid);
    }

    setKids(item, kids, kids_len);

    if (json_object_object_get_ex(json, "url", &tmp))
    {
        setURL(item, json_object_get_string(tmp));
    }

    if (json_object_object_get_ex(json, "score", &tmp))
    {
        setScore(item, json_object_get_int(tmp));
    }

    if (json_object_object_get_ex(json, "title", &tmp))
    {
        setTitle(item, json_object_get_string(tmp));
    }

    if (json_object_object_get_ex(json, "descendants", &tmp))
    {
        setDescendants(item, json_object_get_int(tmp));
    }

    return item;
}

void printItem(struct item item, bool pretty)
{
    char *tab = " ";
    char *newLine = "";

    if (pretty) {
        tab = "\t";
        newLine = "\n";
    }

    printf("{%s", newLine);

    printf("%s\"id\": %d,%s", tab, item.id, newLine);
    printf("%s\"type\": \"%s\"%s", tab, item.type, newLine);
    printf("%s\"by\": \"%s\",%s", tab, item.by, newLine);
    printf("%s\"time\": %d,%s", tab, item.time, newLine);

    if (strcmp(item.type, "comment") == 0)
    {
        printf("%s\"text\": \"%s\",%s", tab, item.text, newLine);
        printf("%s\"parent\": %d,%s", tab, item.parent, newLine);
    }

    printf("%s\"kids\": [", tab);
    printf("%s],%s", join(item.kids, item.kids_len, ", "), newLine);

    if (strcmp(item.type, "story") == 0)
    {
        printf("%s\"url\": \"%s\",%s", tab, item.url, newLine);
        printf("%s\"score\": %d,%s", tab, item.score, newLine);
        printf("%s\"title\": \"%s\",%s", tab, item.title, newLine);
        printf("%s\"descendants\": %d%s", tab, item.descendants, newLine);
    }

    printf("}\n");
}
