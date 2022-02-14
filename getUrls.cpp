#include <fstream>
#include <iostream>
#include <string> // for string class
#include <queue>
#include "get.hpp"

using namespace std;

void Yacurl::getUrls(queue<string>& urls,string host,string route){
    route+="file.html";
    std::ifstream file(route);
    if (file.is_open()) {
        cout<<"HTML file found getting external resources"<<endl;
        std::string line;
        while (std::getline(file, line)) {
            // using printf() in all tests for consistency
            int found;
            if((found = line.find("href="))<0)
            {
                found = line.find("src=");
            }
            if(found != string::npos){
                string url = line.substr(found+6, line.length());
                found=url.find('"');
                url =url.substr(0, found);
                
                if(url[0]=='/'){
                    url.erase(0, 1);
                }

                int position = url.find("/");

                if(position!=-1){
                    string name = url.substr(0,position);
                    if(name.find(".")==-1)
                    {
                        url = host+"/"+url;
                    }
                }
                int position2 = (url.substr(url.find_last_of(".") + 1,url.length())).length();
                if(position2<6){
                    urls.push(url);
                }
            }
        }
        file.close();
        if(urls.empty())
        {
            cout<<"HTML doesn't contains more external resources"<<endl;
        }
    }
}

Yacurl::~Yacurl() { delete &socketFd; delete &ai_result;}


void Yacurl::urlProcessing(string *url,string *src){
    string s = *url;
    string remove = "https://";
    int pos = s.find(remove);
    if(pos==-1){
        remove = "http://";
        pos = s.find(remove);
    }   
    if(pos!=-1){
        s.erase(pos,remove.length());
    }

    int pos2 = s.find("/");

    if(pos2!=-1){
        string name = s.substr(0,pos2);
        string route = s.substr(pos2 + 1);  
        *url = name;
        *src = route;
    }
}
