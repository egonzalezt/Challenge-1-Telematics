#include <queue>
using std::queue;
#include <iostream>
using namespace std;

class Yacurl{
    private:
        int socketFd;
        struct addrinfo *ai_result;
        string PORT;
    public:
        Yacurl();
        Yacurl(string socketPort);
        ~Yacurl();
        struct addrinfo *getAiresult();
        void usage();
        void freeAddrInfo(struct addrinfo);
        void headerContentProcessing(char *buff);
        void urlProcessing(string *url,string *src);
        void dnsResolve(string name,string dir);
        void closeSocket(int sock);
        void getUrls(queue<string>& urls,string host,string route);
        string getRequest(char* hostname, char* dir);
        int sendSock(int socket, const char *message);
        int receive(int socket,string *fileRoute,bool createDir);
        int getSocket();
        int headerProcessing(int sock,string* docType);
};