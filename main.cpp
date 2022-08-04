#include <j-boost-logger/BoostLogger.h>
#include "src/RestServer.h"
#include "src/LoadData.h"

int main(){
    BoostLogger::get_instance().set_filter(boost::log::trivial::info);
    BOOST_LOG_TRIVIAL(trace) << "BoostLogger is initialized";

    LoadData::get_instance().init();




    RestServer server1("http://localhost:8080", 10);
    RestServer server2("http://localhost:8090", 10);
    server1.start_server();
    server2.start_server();
    /**
    http_listener_config listen_config;     //설정값 선언(디폴트 120초 타임아웃)
    listen_config.set_timeout(utility::seconds(10));    //10초로 변경

//listener생성. Server URL, Port 지정, 설정값 전달.
    http_listener listener(U("http://localhos:8080"), listen_config);
    listener.support(methods::GET, FunctionHandler::client_total);

    listener.support(methods::GET, [&listener](http_request req){   //기본적인 GET 요청에 대한 명령어 정리.
        auto path = listener.uri().path();
        auto scheme = listener.uri().scheme();
        auto user_info = listener.uri().user_info();
        auto host = listener.uri().host();
        auto port = listener.uri().port();
        auto query = listener.uri().query();
        auto fragment = listener.uri().fragment();

        std::cout << "path : " << path << std::endl;
        std::cout << "scheme : " <<scheme << std::endl;
        std::cout << "user_info : " <<user_info << std::endl;
        std::cout << "host : " <<host << std::endl;
        std::cout << "port : " <<port << std::endl;
        std::cout << "query : " <<query << std::endl;
        std::cout << "fragment : " <<fragment << std::endl;

        std::cout << req.request_uri().path();

        req.reply(status_codes::OK, "hello word!\n");
    });
     **/
    while(true){}
    return 0;

}