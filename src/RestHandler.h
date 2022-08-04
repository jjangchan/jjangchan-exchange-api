//
// Created by jjangchan on 2022/08/02.
//

#ifndef QRAFT_EXCHANGE_API_RESTHANDLER_H
#define QRAFT_EXCHANGE_API_RESTHANDLER_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <map>
#include <functional>
#include <sstream>
#include <vector>
#include "LoadData.h"
#include <j-boost-logger/BoostLogger.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace utility;

class RestHandler{
private:
    http_listener listener;
    std::map<std::string, std::function<void(http_request, std::vector<std::string>&)>> work_call_back_func;
    std::atomic<int> work_count;

public:
    RestHandler(utility::string_t &url, http_listener_config config) : listener(url, config) {
        listener.support(methods::GET, std::bind(&RestHandler::get_handle, this, std::placeholders::_1));
        // api들을 수행하는 콜백 함수, 람다로 표현
        work_call_back_func = {
                {"total", [&](http_request req, std::vector<std::string>& v) {
                    LoadData& data = LoadData::get_instance();
                    std::string id = v[3];
                    unsigned long long total = data.get_total().get(id);
                    web::json::value body = web::json::value::object();

                    body["client_id"] = web::json::value::string(U(id));
                    body["total"] = web::json::value::number(total);

                    req.reply(status_codes::OK, body);

                }},
                {"list", [&](http_request req, std::vector<std::string>& v) {
                    LoadData& data = LoadData::get_instance();
                    std::vector<web::json::value> send_json;
                    for(SET::Set<std::string>::Iterator iter = data.get_clients().begin(); iter != data.get_clients().end(); ++iter)
                        send_json.push_back(web::json::value::string(U(*iter)));
                    web::json::value body = web::json::value::object();
                    body[U("clients")] = web::json::value::array(send_json);
                    req.reply(status_codes::OK, body);
                }},
                {"range", [&](http_request req, std::vector<std::string>& v) {
                    LoadData& data = LoadData::get_instance();
                    // is number ?
                    for(char& c : v[3]){
                        if(!std::isdigit(c)){
                            req.reply(status_codes::NotFound, "invalid min\n");
                            BOOST_LOG_TRIVIAL(trace) << "invalid min";
                            return;
                        }
                    }
                    for(char& c : v[4]){
                        if(!std::isdigit(c)){
                            req.reply(status_codes::NotFound, "invalid max\n");
                            BOOST_LOG_TRIVIAL(trace) << "invalid max";
                            return;
                        }
                    }

                    int min = atoi(v[3].c_str());
                    int max = atoi(v[4].c_str());
                    if(min >= max){
                        req.reply(status_codes::NotFound, "max must be greater than min !\n");
                        BOOST_LOG_TRIVIAL(trace) << "max must be greater than min";
                        return;
                    }

                    std::vector<unsigned long long> volumes;
                    unsigned long long v_sum = 0;
                    for(HashMap<int, unsigned long long>::Iterator iter = data.get_range().begin(); iter != data.get_range().begin(); ++iter){
                        int price = iter.get_key();
                        if(price >= min && price < max){
                            unsigned long long volume = iter.get_value();
                            volumes.push_back(volume);
                            v_sum +=  volume;
                        }
                    }
                    double average = (double)v_sum/(double)volumes.size();
                    double variance = 0;
                    double sum = 0;
                    for(auto ull : volumes){
                        sum += pow(ull-average,2);
                    }
                    variance = sum/volumes.size();
                    double result = sqrt(variance);

                    web::json::value body = web::json::value::object();
                    body[U("average")] = web::json::value::number(average);
                    body[U("std")] = web::json::value::number(result);
                    req.reply(status_codes::OK, body);

                    //int total = data.get_total().get(id);
                }},
                {"check", [&](http_request req, std::vector<std::string>& v) {
                    LoadData& data = LoadData::get_instance();
                    std::vector<web::json::value> send_json;

                    for(int i = 0; i < data.get_miss().size(); i++){
                        send_json.push_back(web::json::value::number(data.get_miss()[i]));
                    }
                    web::json::value body = web::json::value::object();
                    body[U("missing")] = web::json::value::array(send_json);
                    req.reply(status_codes::OK, body);
                }}
        };
    }
    pplx::task<void> open() { return listener.open(); }
    pplx::task<void> close() { return listener.close(); }
    std::string get_url() const {return listener.uri().to_string();};

private:
    void get_handle(http_request req){
        std::vector<std::string> v;
        std::istringstream ss(req.request_uri().path());
        std::string temp;
        while(getline(ss, temp, '/')) v.push_back(temp);
        //req.reply(status_codes::OK, path);

        work_count++;
        if(work_call_back_func.count(v[2])){ // api가 있다면...
            work_call_back_func[v[2]](req, v);
        }else{
            req.reply(status_codes::NotFound, "invalid api\n");
            BOOST_LOG_TRIVIAL(info) << "not contain key";
        }
        work_count--;

    }

};

#endif //QRAFT_EXCHANGE_API_RESTHANDLER_H
