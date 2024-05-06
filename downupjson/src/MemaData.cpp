#include "include/MemaData.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string_view>
#include <curl/curl.h>
MemaData::MemaData(std::string filename,std::string serverURL,nlohmann::json jArray):filename(filename),serverURL(serverURL),jsonData(jArray){

}

size_t MemaData::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
bool MemaData::filenameCheck(){
    if (this->filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") {
        //std::cout << "The file is a .txt file." << std::endl;
        return true;
    } else {
        std::cerr << "The file is not a .txt file." << std::endl;
        return false;
    }
}
bool MemaData::serverURLCheck(){
    if (this->serverURL.substr(0, 7) == "http://") {

       //std::cout << "The argument is likely an HTTP URL." << std::endl;
        return true;
    } else {
        std::cout << serverURL << std::endl;
        std::cerr << "The argument does not appear to be an HTTP URL." << std::endl;
        return false;
    }
}
nlohmann::json  MemaData::TxTtoJson(nlohmann::json jArray){
    std::vector<std::string> vector1;
    std::vector<std::string> vector2;
   
     // 文件路径
    std::string filePath = this->filename;
    // 创建ifstream类的对象
    std::ifstream file(filePath);

    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    char delimiter = ':';
    

    while (std::getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos == std::string::npos) {
            // 如果找不到冒号，返回原字符串和一个空字符串
            return {line, ""};
        }
        std::string firstPart = line.substr(0, delimiterPos);

        std::string secondPart = line.substr(delimiterPos + 1); // 跳过冒号
        bool found = false;
      // Iterate through each character in the string
    for (std::string::iterator it = secondPart.begin(); it != secondPart.end(); ++it) {
        // Check if the character is the byte value 0x20 (space)
        if (static_cast<unsigned char>(*it) == 0x20) {
            found = true;
            // Erase all characters from the space onwards
            secondPart.erase(it, secondPart.end());
            break;  // Exit loop if byte is found
        }
    }

        vector1.push_back(firstPart);
        vector2.push_back(secondPart);
     
    }

     if (vector1.size() == vector2.size())
     {
         for (size_t i = 0; i < vector1.size(); i++)
            {
                jArray[vector1[i]]=vector2[i];
            }
     }
    
    // for (auto& element : jArray.items()) {
    //     std::cout << "Key: " << element.key() << ", Value: " << element.value() << std::endl;
    // }
    
    // 关闭文件
    file.close();
    return jArray;
}


void MemaData::SendDataToServer(MemaData & memaData){

CURL *curl = curl_easy_init();
    if (curl) {
        // 构建 JSON 数据
        nlohmann::json jsonObj = memaData.jsonData;

        // 将 JSON 对象转换为字符串
        std::string jsonData = jsonObj.dump();

        // 用于存储服务器响应的字符串
        std::string readBuffer;

        // 设置 CURL 的 URL（即服务器地址）
        curl_easy_setopt(curl, CURLOPT_URL, memaData.serverURL.c_str());

        // 设置 HTTP 头部，表示我们发送的是 JSON 数据
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        std::cout <<jsonData.c_str() << std::endl;
        // 设置要发送的数据
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());


        // 设置写回函数，以便收集服务器的响应
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &MemaData::WriteCallback);

        // 设置写回函数的用户参数（这里是用于存储响应的字符串）
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // 执行 CURL 请求
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // 输出服务器的响应

            std::string substr = "\"Message\":\"OK\"";
            size_t pos = readBuffer.find(substr);
            if (pos != std::string::npos) {
                std::cout << "Success" << std::endl;
            } else {
                std::cout << "Failed" << std::endl;
            }
           // std::cout << "Server Response: " << readBuffer << std::endl;
        }
        
        // 清理
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }



}