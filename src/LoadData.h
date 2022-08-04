//
// Created by jjangchan on 2022/08/02.
//

#ifndef QRAFT_EXCHANGE_API_LOADDATA_H
#define QRAFT_EXCHANGE_API_LOADDATA_H

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <j-boost-logger/BoostLogger.h>
#include <j-data-structure/Vector.h>
#include <j-data-structure/HashMap.h>
#include <j-data-structure/Set.h>

class LoadData{
private:
    /**
    struct InfoData{
        std::string stocks_code;
        int unique_num;
        int volume;
        int price;
        InfoData(std::string _stocks_code, int _unique_num, int _volume, int _price) :
                stocks_code(_stocks_code),
                unique_num(_unique_num),
                volume(_volume),
                price(_price){}
    };
    HashMap<std::string, Vector<InfoData>> m_infors;
    Vector<InfoData> infos;
    **/

    // exam) api/total, api/range
    // key 값 기준으로 검색해야 하므로, 중복을 허용 하지 않고 최대한 빠르게 검색하는 자료구조는 map(O(logN)) 또는 unordered_map(O(1)) 이여서 선택
    // 한 고객이 최대 담을수 있는 금액은 아래와 같다
    // 최대 고유 번호, 체결량, 체결금액 : 999,999 , 최대 파일 생성 개수 : 6
    // 999,999 * 999,999 * 999,999 * 6 는 2^64-1 보다 작다, 따라서 max 값을 unsigned long long 으로 표현 가능 하다?
    // 만약 2^64-1 범위를 넘는 경우가 있으면 value 타입을 vector로 변경해야한다.
    HashMap<int, unsigned long long> range; // -> std::map, 정렬되어있는상태
    HashMap<std::string, unsigned long long> total;

    // exam) api/list, api/range, api/check
    // set은 중복되지 않고 정렬되어 있는 상태에 자료구조 이여서 선택
    SET::Set<std::string> clients;
    SET::Set<int> u_nums; // 최대 고유 번호 -> 999999
    Vector<int> miss;

    LoadData(){}
    ~LoadData(){}


public:
    inline static LoadData &get_instance(){
        static LoadData instance;
        return instance;
    }
    void init(){ // 오후 5시 돠면 init() 함수로 호출해서 log data를 초기화 해야함..
        std::string path;
        std::string date;
        /** 현재 날짜 조회해서 해당 data 가져오기
        date = get_date();
        **/
        // 특정 날짜 data 가져오기
        path = "../datas/";
        date = "2022-08-02";
        // get file list
        try{
            for(const auto& file : std::filesystem::directory_iterator(path+date)){
                std::ifstream in(file.path().c_str());
                std::string str;
                while(in){
                    getline(in, str);
                    if(str.size() == 28){
                        std::string id = str.substr(0,4);
                        std::string stocks_code = str.substr(4,6);
                        int unique_num = atoi(str.substr(10,6).c_str());
                        int volume = atoi(str.substr(16,6).c_str());
                        int price = atoi(str.substr(22,6).c_str());
                        int cal = volume*price;
                        total.put(id, cal);
                        range.put(price, volume);
                        clients.insert(id);
                        u_nums.insert(unique_num);
                    }
                }
            }

            for(SET::Set<std::string>::Iterator iter = clients.begin(); iter != clients.end(); ++iter){
                std::cout << *iter << ", total : " << total.get(*iter) << std::endl;
            }


            // 빈고유번호 찾기
            SET::Set<int>::Iterator iter = u_nums.begin();
            int step = 1;
            for(; iter != u_nums.end(); ++iter, step++){
                int current = *iter;
                if(step != current){ // step 이랑 일치하지 않으면
                    while(step < current){ //빈 고유번호 담기
                        miss.push_back(step);
                        step++;
                    }
                }
            }

            for(int i = 0; i < miss.size(); i++){
                std::cout << miss[i] << " ";
            }
            std::cout << std::endl;
        } catch (std::exception& e){
            BOOST_LOG_TRIVIAL(warning) << "empty directory , path : " + path+date;
        }


    }

    //const Vector<std::string>& get_clients(){return clients;};
    //const Vector<int>& get_miss(){return miss;};
    //const Vector<int>& get_volumes(){return volumes;}
    HashMap<int, unsigned long long>& get_range(){return range;}; // -> std::map, 정렬되어있는상태
    HashMap<std::string, unsigned long long>& get_total(){return total;};
    Vector<int>& get_miss(){return miss;};
    SET::Set<std::string>& get_clients(){return clients;};

private:
    std::string get_date(){
        time_t t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }
};

#endif //QRAFT_EXCHANGE_API_LOADDATA_H
