#include "WeatherApp.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

WeatherApp::WeatherApp() {
    const char* authEnv = std::getenv("APP_AUTHOR");
    const char* portEnv = std::getenv("APP_PORT");

    author = authEnv ? authEnv : "Unknown author";
    port = portEnv ? std::stoi(portEnv) : 8080;
    apiKey = "API_KEY";
}

void WeatherApp::logStartup() const {
    std::time_t now = std::time(nullptr);
    std::clog << "==========================================" << std::endl;
    std::clog << "[Start-up date]: " << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << std::endl;
    std::clog << "[Author]: " << author << std::endl;
    std::clog << "[TCP Port]: " << port << std::endl;
    std::clog << "==========================================" << std::endl;
}

void WeatherApp::setupRoutes() {
    svr.Get("/", [this](const httplib::Request &, httplib::Response &res) {
       res.set_content(this->getIndexTemplate(), "text/html");
    });
    svr.Get("/weather", [this](const httplib::Request &req, httplib::Response &res) {
        std::string city = req.has_param("city") ? req.get_param_value("city") : "Lublin";

        httplib::Client cli("api.weatherapi.com");
        auto response = cli.Get(("/v1/current.json?key=" + this->apiKey + "&q=" + city).c_str());

        if (response && response->status == 200) {
            res.set_content(this->getWeatherTemplate(response->body), "text/html");
        } else {
            res.status = 500;
            res.set_content("Error while fetching data", "text/plain");
        }
    });
}

std::string WeatherApp::getIndexTemplate() const {
    std::string html = R"HTML(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>WeatherApp</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    text-align: center;
                    background-color: #333333; /* Dark grey background */
                    color: #ffffff; /* White text for contrast */
                    padding-top: 50px;
                }
                select, button {
                    padding: 10px;
                    font-size: 16px;
                    margin: 10px;
                    border-radius: 5px;
                    border: none;
                }
                button {
                    background-color: #555555;
                    color: white;
                    cursor: pointer;
                }
                button:hover {
                    background-color: #777777;
                }
            </style>
            <script>
                const locations = {
                    "Poland": ["Warsaw", "Lublin"],
                    "United Kingdom": ["London", "Liverpool"]
                };

                function updateCities() {
                    const country = document.getElementById('country').value;
                    const citySelect = document.getElementById('city');
                    citySelect.innerHTML = '';
                    locations[country].forEach(city => {
                        let opt = document.createElement('option');
                        opt.value = city;
                        opt.innerHTML = city;
                        citySelect.appendChild(opt);
                    });
                }
            </script>
        </head>
        <body onload="updateCities()">

            <h1>WeatherApp</h1>

            <form action="/weather" method="get">
                <label for="country"><b>Country:</b></label>
                <select id="country" onchange="updateCities()">
                    <option value="Poland">Poland</option>
                    <option value="United Kingdom">United Kingdom</option>
                </select>

                <label for="city"><b>City:</b></label>
                <select id="city" name="city"></select>

                <br><br>
                <button type="submit">Check Weather</button>
            </form>

            <hr style="margin-top: 50px; width: 40%; border: 0.5px solid #555;">
            <p>Author: <b>)HTML";

    html += author;

    html += R"HTML(</b></p>
        </body>
        </html>
    )HTML";

    return html;
}

std::string WeatherApp::getWeatherTemplate(const std::string& jsonData) const {
    std::string html = R"HTML(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>Weather Result - WeatherApp</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    text-align: center;
                    background-color: #333333; /* Dark grey background */
                    color: #ffffff;
                    padding-top: 50px;
                }
                .card {
                    background: #444444; /* Slightly lighter grey for the card */
                    border-radius: 15px; padding: 25px;
                    display: inline-block; box-shadow: 0 4px 15px rgba(0,0,0,0.5);
                    text-align: left; line-height: 1.8; min-width: 300px;
                    color: #ffffff;
                }
                h1 { color: #ffffff; border-bottom: 2px solid #555; padding-bottom: 10px; text-align: center; }
                .label { font-weight: bold; color: #bbbbbb; }
                .val { font-weight: bold; color: #00d4ff; float: right; }
                .back-btn { display: block; margin-top: 25px; text-align: center; text-decoration: none;
                            color: white; background: #555555; padding: 10px; border-radius: 5px; }
                .back-btn:hover { background: #666666; }
            </style>
        </head>
        <body>
            <div class="card">
                <h1>Weather Info</h1>
                <div id="content">Loading...</div>
                <a href="/" class="back-btn">Back to Menu</a>
            </div>

            <script>
                const data = )HTML";

    html += jsonData;

    html += R"HTML(;
                const content = document.getElementById('content');

                try {
                    content.innerHTML = `
                        <p><span class="label">Country:</span> <span class="val">${data.location.country}</span></p>
                        <p><span class="label">City:</span> <span class="val">${data.location.name}</span></p>
                        <p><span class="label">Temperature:</span> <span class="val">${data.current.temp_c} °C</span></p>
                        <p><span class="label">Humidity:</span> <span class="val">${data.current.humidity} %</span></p>
                        <p><span class="label">Wind Speed:</span> <span class="val">${data.current.wind_kph} km/h</span></p>
                    `;
                } catch(e) {
                    content.innerHTML = "<p style='color:red;'>Error processing JSON data.</p>";
                }
            </script>
        </body>
        </html>
    )HTML";
    return html;
}

void WeatherApp::run() {
    logStartup();
    setupRoutes();
    std::clog << "[INFO] Server is starting at 0.0.0.0:" << port << "..." << std::endl;

    if (!svr.listen("0.0.0.0", port)) {
        std::clog << "[ERROR] Failed to start server on port " << port << "!" << std::endl;
        std::exit(1);
    }
}