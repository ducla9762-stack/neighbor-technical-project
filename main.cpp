#include <drogon/drogon.h>
#include <json/json.h>
#include "algo.hpp"  // Your bin-packing function
#include <sstream>

using namespace drogon;

int main() {
    app().registerHandler(
        "/",
        [](const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) {
            LOG_INFO << "Raw body: " << req->getBody() << "";
            auto jsonPtr = req->getJsonObject();
            if (!jsonPtr) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k400BadRequest);
                resp->setBody("Invalid JSON");
                callback(resp);
                return;
            }
            LOG_INFO << "OK HERE 1";
            Json::Value vehicles = *jsonPtr;
            LOG_INFO << "OK HERE 2"; 
            // Call your algorithm
            Json::Value listings = loadListings("listings.json");
            // LOG_INFO << listings;
            LOG_INFO << "OK HERE 3"; 
            Json::Value results = find_vehicle_storage(vehicles, listings);
            LOG_INFO << "OK HERE 4"; 
            auto resp = HttpResponse::newHttpJsonResponse(results);
            callback(resp);
        },
        {Post}
    );
    // // Create a POST endpoint
    // app().registerHandler(
    //     "/",
    //     [](const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback) {
    //         Json::Value vehicles;
    //         Json::CharReaderBuilder reader;
    //         std::string errs;

    //         std::istringstream s(std::string(req->getBody()));
    //         if (!Json::parseFromStream(reader, s, &vehicles, &errs)) {
    //             auto resp = HttpResponse::newHttpResponse();
    //             resp->setStatusCode(k400BadRequest);
    //             resp->setBody("Invalid JSON: " + errs);
    //             callback(resp);
    //             return;
    //         }

    //         // Call your algorithm
    //         Json::Value listings = loadListings("listings.json");
    //         Json::Value results = find_vehicle_storage(vehicles, listings);

    //         auto resp = HttpResponse::newHttpJsonResponse(results);
    //         callback(resp);
    //     },
    //     {Post}
    // );

    // Start the server
    app().addListener("0.0.0.0", 8080).run();
}