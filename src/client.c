#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>

#include "client.h"

size_t curl_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;

    struct curl_fetch_t *p = (struct curl_fetch_t *)userp;

    p->payload = (char *)realloc(p->payload, p->size + realsize + 1);

    if (p->payload == NULL)
    {
        fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");

        free(p->payload);

        return -1;
    }

    memcpy(&(p->payload[p->size]), contents, realsize);

    p->size += realsize;

    p->payload[p->size] = 0;

    return realsize;
}

CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_t *fetch)
{
    CURLcode resultCode;

    fetch->payload = (char *)calloc(1, sizeof(fetch->payload));

    if (fetch->payload == NULL)
    {
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");

        return CURLE_FAILED_INIT;
    }

    fetch->size = 0;

    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *)fetch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/7.54");
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    resultCode = curl_easy_perform(ch);

    return resultCode;
}

int get_json(char *url, struct json_object **json)
{
    CURL *ch;
    CURLcode result_code;

    enum json_tokener_error json_err = json_tokener_success;

    struct curl_fetch_t fetch;
    struct curl_fetch_t *fetch_p = &fetch;
    struct curl_slist *headers = NULL;

    if ((ch = curl_easy_init()) == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create curl handle in test");
        return 1;
    }

    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);

    result_code = curl_fetch_url(ch, url, fetch_p);

    curl_easy_cleanup(ch);
    curl_slist_free_all(headers);

    if (result_code != CURLE_OK || fetch_p->size < 1)
    {
        fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said (%s)", url, curl_easy_strerror(result_code));
        return 2;
    }

    if (fetch_p->payload != NULL)
    {
        *json = json_tokener_parse_verbose(fetch_p->payload, &json_err);
        free(fetch_p->payload);
    }
    else
    {
        fprintf(stderr, "ERROR: Failed to populate payload");
        free(fetch_p->payload);
        return 3;
    }

    if (json_err != json_tokener_success)
    {
        fprintf(stderr, "ERROR: Failed to parse JSON string");
        json_object_put(*json);
        return 4;
    }

    return result_code;
}