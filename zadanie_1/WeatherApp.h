#ifndef WEATHER_WEATHERAPP_H
#define WEATHER_WEATHERAPP_H

#include "httplib.h"

class WeatherApp {
private:
    std::string author;
    int port;
    std::string apiKey;
    httplib::Server svr;

    void setupRoutes();
    std::string getIndexTemplate() const;
    std::string getWeatherTemplate(const std::string& jsonData) const;
    void logStartup() const;

public:
    WeatherApp();
    void run();
};

#endif //WEATHER_WEATHERAPP_H