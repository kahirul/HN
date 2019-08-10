#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <json-c/json.h>

struct curl_fetch_t
{
    char *payload;
    size_t size;
};

CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_t *fetch);

int get_json(char *url, json_object **json);

#endif
