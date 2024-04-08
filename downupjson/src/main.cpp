#include <iostream>
#include "include/MemaData.h"
#include <nlohmann/json.hpp>
#include <curl/curl.h>




int main(int argc, char *argv[]) {



    
    nlohmann::json jArray;
    // 打印参数数量
     MemaData memaData(argv[1],argv[2],jArray);
    
    
    if (memaData.filenameCheck()) {
    // 文件名检查通过
    }
    if (memaData.serverURLCheck()) {
        // URL检查通过
    }
    memaData.jsonData= memaData.TxTtoJson(jArray);
    //std::cout << memaData.jsonData.dump(4) << std::endl;
    // 将 JSON 对象转换为字符串
    std::string jsonData =  memaData.jsonData.dump();
    //std::cout << jsonData<< std::endl;
    memaData.SendDataToServer(memaData);

    return 0;
}