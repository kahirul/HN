#ifndef HN_H
#define HN_H

#include <stdio.h>
#include <json-c/json.h>

struct item
{
    int id;
    char *type;
    char *by;
    int time;

    char *text;
    int parent;
    char **kids;
    char *url;
    int score;
    char *title;
    int descendants;

    size_t kids_len;
} item;

char *join(char **collection, size_t col_size, char *delimiter);

struct item *createItem(int id, const char *type, const char *by, int time);

void setText(struct item *item, const char *text);
void setParent(struct item *item, int parent);
void setKids(struct item *item, char **kids, size_t kids_len);
void setURL(struct item *item, const char *url);
void setScore(struct item *item, int score);
void setTitle(struct item *item, const char *title);
void setDescendants(struct item *item, int descendants);

struct item *parse_item(struct json_object *json);

void printItem(struct item item);

#endif
