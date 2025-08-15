// Compile: g++ docker_ip_api.cpp -o docker_ip_api -lcurl
// Install nlohmann/json: sudo apt install nlohmann-json3-dev

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define DOCKER_SOCKET "/var/run/docker.sock"

struct Memory {
    std::string response;
};

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    Memory *mem = static_cast<Memory*>(userp);
    mem->response.append(static_cast<char*>(data), realsize);
    return realsize;
}

class DockerClient {
public:
    DockerClient(std::string socket_path) : socket_path(socket_path) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    ~DockerClient() {
        curl_global_cleanup();
    }

    std::string request(const std::string &endpoint) {
        CURL *curl = curl_easy_init();
        if (!curl) return "";

        Memory chunk;
        std::string url = "http://localhost" + endpoint;

        curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, socket_path.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        return chunk.response;
    }

private:
    std::string socket_path;
};

struct ContainerInfo {
    std::string name;
    std::map<std::string, std::string> network_ips; // network name -> IP
};

int main() {
    DockerClient docker(DOCKER_SOCKET);

    // Step 1: Get list of containers
    std::string containers_str = docker.request("/containers/json");
    if (containers_str.empty()) {
        std::cerr << "Failed to fetch container list" << std::endl;
        return 1;
    }

    json containers = json::parse(containers_str);
    std::vector<ContainerInfo> container_list;

    // Step 2: For each container, fetch details
    for (auto &cont : containers) {
        std::string id = cont["Id"];
        std::string name = cont["Names"][0];

        std::string details_str = docker.request("/containers/" + id.substr(0, 12) + "/json");
        if (details_str.empty()) continue;

        json details = json::parse(details_str);
        ContainerInfo info;
        info.name = name;

        auto networks = details["NetworkSettings"]["Networks"];
        for (auto it = networks.begin(); it != networks.end(); ++it) {
            std::string net_name = it.key();
            std::string ip = it.value()["IPAddress"];
            info.network_ips[net_name] = ip;
        }

        container_list.push_back(info);
    }

    // Step 3: Print result
    std::cout << "Container Name\tNetwork\t\tIP Address\n";
    std::cout << "----------------------------------------------\n";
    for (auto &c : container_list) {
        for (auto &net : c.network_ips) {
            std::cout << c.name << "\t" << net.first << "\t" << net.second << "\n";
        }
    }

    return 0;
}
