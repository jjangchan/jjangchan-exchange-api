//
// Created by jjangchan on 2022/08/02.
//

#ifndef QRAFT_EXCHANGE_API_RESTSERVER_H
#define QRAFT_EXCHANGE_API_RESTSERVER_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <memory>
#include "RestHandler.h"
#include <j-boost-logger/BoostLogger.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace utility;


class RestServer{
private:
    std::unique_ptr<RestHandler> handler = nullptr;
    bool is_start;
public:
    RestServer(const std::string &url,
               const int time_out_seconds){
        http_listener_config config;
        config.set_timeout(utility::seconds(time_out_seconds)); // 타임아웃 설정
        utility::string_t u_url = U(url);
        try{
            handler = std::unique_ptr<RestHandler>(new RestHandler(u_url, config));
        } catch (std::exception &e){
            BOOST_LOG_TRIVIAL(error) << e.what();
        }
        is_start = false;
    }

    void start_server(){
        if(is_start){
            BOOST_LOG_TRIVIAL(warning) << "already started listener";
            return;
        }

        BOOST_LOG_TRIVIAL(info) << "start server, url : "+handler->get_url();
        handler->open().wait();
        is_start = true;
    }

    void stop_server(){
        if(!is_start){
            BOOST_LOG_TRIVIAL(warning) << "already stopped listener";
            return;
        }
        BOOST_LOG_TRIVIAL(info) << "stop server";
        handler->close();
        is_start = false;

    }

};

#endif //QRAFT_EXCHANGE_API_RESTSERVER_H
