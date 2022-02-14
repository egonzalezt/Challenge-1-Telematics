#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string> // for string class
using namespace std;
#include "get.hpp"
#include <queue>

void getMedia(string fileRoute,string url){
  string name=url;
  string dir="";
  string get_req;
  char* http_get;

  Yacurl *test = new Yacurl();

  test->urlProcessing(&name,&dir);

  test->dnsResolve(name, dir);
	
	get_req = test->getRequest(&name[0],&dir[0]);

  http_get = &get_req[0];

  cout<<http_get<<endl;

	int ret = test->sendSock(test->getSocket(), http_get);

	if(ret < 0) {
    std::cerr << "Sending failed" << std::endl;
	  test->closeSocket(test->getSocket());
	  freeaddrinfo(test->getAiresult());
	  free(http_get);
	}
  
	test->receive(test->getSocket(),&fileRoute,false);

	freeaddrinfo(test->getAiresult());

	test->closeSocket(test->getSocket());
}

int main(int argc, char *argv[])
{
  string name(argv[1]);
  string dir="";
  string get_req;
  string fileRoute="";
  queue<string> urls;
  char* http_get;

  Yacurl *test;

  if(argc >=3){
    test= new Yacurl(argv[2]);
  }else{
    test= new Yacurl();
  }

  if((argc < 2)||(argc > 3)) {
  	test->usage();
  }

  test->urlProcessing(&name,&dir);

  test->dnsResolve(name, dir);
	
	get_req = test->getRequest(&name[0],&dir[0]);

  http_get = &get_req[0];//cast string to char[]

  cout<<http_get<<endl;

	int ret = test->sendSock(test->getSocket(), http_get);

	if(ret < 0) {
    std::cerr << "Sending failed" << std::endl;
	  test->closeSocket(test->getSocket());
    //The freeaddrinfo() function frees one or more addrinfo structures returned by getaddrinfo()
	  freeaddrinfo(test->getAiresult());
	  free(http_get);// deallocates a block of memory previously allocated
	  return EXIT_FAILURE;
	}
  
  cout<<"Request sent"<<endl;

	test->receive(test->getSocket(),&fileRoute,true);

	freeaddrinfo(test->getAiresult());

	test->closeSocket(test->getSocket());

  test->getUrls(urls,name,fileRoute);
  
  if(!urls.empty()){
    while (!urls.empty()) {
      string urlToFind = urls.front();
      try{
        if(urlToFind.length()>0)
        {
          getMedia(fileRoute,urlToFind); 
        }
      }catch(exception e){
      }
      urls.pop();
    }
  }

  cout<<"Process finished"<<endl;

	return EXIT_SUCCESS;
}