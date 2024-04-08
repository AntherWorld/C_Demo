#ifndef MemaData_h
#define MemaData_h
#include <iostream>
#include <vector>
#include <string_view>
#include <nlohmann/json.hpp>
class MemaData{
    public:
    MemaData(std::string filename,std::string serverURL,nlohmann::json jArray);
    std::string filename;
    std::string serverURL;
    nlohmann::json jsonData;
    bool filenameCheck();
    bool serverURLCheck();
    nlohmann::json TxTtoJson(nlohmann::json jArray);
    void SendDataToServer(MemaData & memaData);
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp); 

    
};
#endif