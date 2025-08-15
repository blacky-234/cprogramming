//TODO: https://docs.docker.com/reference/api/engine/version/v1.43/#tag/Container/operation/ContainerList

//TODO: compailer command -> gcc docker_ip_api.c -o docker_ip_api -lcurl -lcjson
// Install cJSON: sudo apt install libcjson-dev

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define DOCKER_SOCKET "/var/run/docker.sock"

// Buffer for CURL response
struct Memory {
    char *response;
    size_t size;
};

// Callback for CURL to store response
static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL) return 0;
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
    return realsize;
}

// Make request to Docker API
char* docker_api_request(const char *endpoint) {
    CURL *curl;
    CURLcode res;
    struct Memory chunk = {0};

    curl = curl_easy_init();
    if (!curl) return NULL;

    char url[256];
    snprintf(url, sizeof(url), "http://localhost%s", endpoint);

    curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, DOCKER_SOCKET);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        free(chunk.response);
        chunk.response = NULL;
    }

    curl_easy_cleanup(curl);
    return chunk.response;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Step 1: Get all containers
    char *containers_json = docker_api_request("/containers/json");
    if (!containers_json) {
        fprintf(stderr, "Failed to fetch containers\n");
        return 1;
    }

    cJSON *containers = cJSON_Parse(containers_json);
    free(containers_json);

    if (!cJSON_IsArray(containers)) {
        fprintf(stderr, "Invalid container list\n");
        cJSON_Delete(containers);
        return 1;
    }

    // Step 2: For each container, get detailed info
    cJSON *container;
    cJSON_ArrayForEach(container, containers) {
        const cJSON *id = cJSON_GetObjectItemCaseSensitive(container, "Id");
        const cJSON *names = cJSON_GetObjectItemCaseSensitive(container, "Names");

        if (cJSON_IsString(id) && cJSON_IsArray(names)) {
            char details_endpoint[256];
            snprintf(details_endpoint, sizeof(details_endpoint), "/containers/%.12s/json", id->valuestring);

            char *details_json = docker_api_request(details_endpoint);
            if (!details_json) continue;

            cJSON *details = cJSON_Parse(details_json);
            free(details_json);

            if (details) {
                cJSON *networks = cJSON_GetObjectItemCaseSensitive(
                    cJSON_GetObjectItemCaseSensitive(details, "NetworkSettings"),
                    "Networks"
                );

                cJSON *net;
                cJSON_ArrayForEach(net, networks) {
                    cJSON *ip = cJSON_GetObjectItemCaseSensitive(net, "IPAddress");
                    if (cJSON_IsString(ip)) {
                        printf("%s : %s\n", names->child->valuestring, ip->valuestring);
                    }
                }
                cJSON_Delete(details);
            }
        }
    }

    cJSON_Delete(containers);
    curl_global_cleanup();
    return 0;
}
