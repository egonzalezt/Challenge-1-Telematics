#include <sys/types.h>//def data types
#include <sys/socket.h>//Linux sockets
#include <sys/stat.h>//create folders

#include <netdb.h>//for dns
#include <unistd.h>//to finish sockets

#include <bits/stdc++.h>//cstring libraries

#include <iostream>
#include <string>
using namespace std;
#include "get.hpp"
#define RECV_BUFFER_SIZE 1024

//Constructors
Yacurl::Yacurl(){
    socketFd=-1;
    addrinfo *ai_result=NULL;
    PORT="80";
}

Yacurl::Yacurl(string socketPort){
    socketFd=-1;
    addrinfo *ai_result=NULL;
    PORT=socketPort;
}

//Functions
void Yacurl::dnsResolve(string name,string dir){
    int ai_res;//address info result

	struct addrinfo ai_hints;

	memset(&ai_hints, 0, sizeof(struct addrinfo));//set 0 by default
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_socktype = SOCK_STREAM;//TCP

	ai_res = getaddrinfo(&name[0], &PORT[0], &ai_hints, &ai_result);

	if(ai_res) {
        std::cerr << "DNS failed ->" <<gai_strerror(ai_res)<< std::endl;//gai_strerror describes the error
		freeaddrinfo(ai_result);
		usage();
	}

	struct addrinfo *ai_tmp;

	char addr_str[NI_MAXHOST];//defined maxhost
    
    //ai_result contains multiple addrinfo list
	for(ai_tmp = ai_result; ai_tmp != NULL; ai_tmp = ai_tmp->ai_next) {
        //avoid sockets without the specific family and socket type
		if((socketFd = socket(ai_tmp->ai_family, ai_tmp->ai_socktype, 0)) < 0) {
			continue;
		} else {
			int rev_dns = getnameinfo(ai_tmp->ai_addr, ai_tmp->ai_addrlen, addr_str, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if(!rev_dns) {
                cout<<"Found ip address for name"<<endl;
                cout<<"Connecting to "<<addr_str<<endl;
                cout<<"Protocol version "<<ai_tmp->ai_socktype<<endl;
                
                //Establish connection with the server
			    if(connect(socketFd, ai_tmp->ai_addr, ai_tmp->ai_addrlen)) {
                    std::cerr << "Connection failed" << std::endl;
			    	close(socketFd);
			    	socketFd = -1;
			    	continue;
			    }
			}
			break;
		}
	}
	if(socketFd < 0) {
		perror("Creating socket failed");
		freeaddrinfo(ai_result);
		exit(EXIT_FAILURE);
	}
}

int Yacurl::getSocket(){
    return socketFd;
}

void Yacurl::closeSocket(int sock){
    close(sock);
}

struct addrinfo *Yacurl::getAiresult(){
    return ai_result;
}

void Yacurl::usage()
{
    cout<<"To use Yacurl put the url as an argument"<<endl;
    exit(EXIT_FAILURE);
}

string Yacurl::getRequest(char* hostname, char *dir) {
    cout<<"GET REQUEST"<<endl;

    string http_get_stub="GET /";
    http_get_stub+=dir;
    http_get_stub+=" HTTP/1.1\r\nHost: ";
    http_get_stub+=hostname;
    http_get_stub+="\r\nConnection: close";
    http_get_stub+="\r\n\r\n";
    
	return http_get_stub;
}

int Yacurl::sendSock(int socket, const char *message) {

	if(message == NULL) {
		return 0;
	}

	int length = strlen(message);

    if(send(socket, message, length, 0) < 0)
    {
      perror("Send failed : ");
      return 0;
    }
    
	return 1;
}

void Yacurl::headerContentProcessing(char *buff){
    char *ptr2=buff;
    char* token;  
    int count = 0;
    //remove spaces and uppercase
    for (int i = 0; buff[i]; i++){
        if(buff[i]>=65&&buff[i]<=90){
          buff[i]=buff[i]+32;
        }
        if (buff[i] != ' '){
          buff[count++] = buff[i]; 
        }
    }
    buff[count] = '\0';
}

int Yacurl::headerProcessing(int sock,string *docType){
    char buff[1024]="";
    char *ptr=buff;//access to each element on array
    int bytes_received;
    char result[20]="";
    char* token;
    string typeFile="";

    while(bytes_received = recv(sock, ptr, 1, 0)){
        if(bytes_received==-1){
            std::cerr << "Error Parsing Header" << std::endl;
            exit(1);
        }
        if((ptr[-3]=='\r')  && (ptr[-2]=='\n' ) && (ptr[-1]=='\r')  && (*ptr=='\n' )){
            break;
        }
        ptr++;
    }

    ptr=buff;
    cout<<"Response"<<endl;
    cout<<buff<<endl;

    headerContentProcessing(buff);

    ptr=strstr(ptr,"content-type:");  
    token = strtok(ptr, ";\r\n");

    if(ptr){
        string s(token);
        int pos2 = s.find("/");
        if(pos2!=-1){
            typeFile = s.substr(pos2 + 1);  
        }
    } 
    *docType = typeFile;

    return  bytes_received ;
}

int Yacurl::receive(int socket,string *fileRoute,bool createDir) {
    int bytes=0;
    int cont=1;
    int bytes_received;
    string dirname = "Results";
	string docType="";    
    string name="";

    int docSize= headerProcessing(socket,&docType);

    if (createDir&&(mkdir("Results", 0777) == -1))
    {
        while(true){
            dirname = "Results";
            dirname+=to_string(cont);
            if (mkdir(&dirname[0], 0777) == -1){
                cont++;
                continue;
            }
            else{
                break;
            }
        }
    }
    if(createDir){
        *fileRoute="./"+dirname+"/";
        name ="./"+dirname+"/file";
        name += ".";
        name +=docType;
    }else{
        cont=1;
        string test = *fileRoute+"file"+"."+docType;
        while(true){
            if (FILE *file = fopen(&test[0], "r")) {
                test = *fileRoute+"file"+to_string(cont)+"."+docType;
                fclose(file);
                cont++;
            } else {
                name=test;
                break;
            }
        }
    }

    char recv_buffer[RECV_BUFFER_SIZE];
	memset(recv_buffer, 0, RECV_BUFFER_SIZE);

    FILE* fd=fopen(&name[0],"wb");
    
    while(bytes_received = recv(socket, recv_buffer, 1024, 0)){
        if(bytes_received==-1){
            perror("recieve");
            exit(3);
        }
        fwrite(recv_buffer,1,bytes_received,fd);
        bytes+=bytes_received;

        if(bytes==docSize)
        {
          break;
        }  
    } 

    fclose(fd);

    cout<<"Data saved search file-> "<<name<<endl;

    return 1;
}