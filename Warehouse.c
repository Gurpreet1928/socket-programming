/* tcpserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
//#include <dos.h>

#define WHPORT "41800"
#define WHPORT1 21740 //changed
#define UWHPORT 31740
#define UWHPORT1 5740
#define UWHPORT2 32740 //changed
int main()
{       
    //--------------------declaring variables---------------------------------------------------
        int s1[3],s2[3],s3[3],s4[3];
        int readData[1024],x;
        int tcp_sock,sock, sockfd,connected, bytes_received ,true = 1,yes =1;  
        char send_data[1024],recv_data[1024], s,a[1024];       
        struct sockaddr_in server_addr,client_addr; 
        struct hostent *host;        
        int sin_size,rv;  
        int sum_vector[30], truck_vector[30];
        int counter=0,i=0;
        struct addrinfo hints, *servinfo, *p;
        struct sockaddr_in addr;
        socklen_t addrlen;
        addrlen = sizeof(addr);
        const char hostN[20] = "nunki.usc.edu";
         
    // START:  print IP address and dynamically generated PORT number--------------//
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if ((rv = getaddrinfo("nunki.usc.edu", WHPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
        }
        // loop through all the results and bind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
        perror("server: socket");
        continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
        close(sockfd);
        perror("server: bind");
        continue;
        }
        break;
        }
        if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
        }

        if (getsockname(sockfd,(struct sockaddr*)&addr , &addrlen) == -1) {
          perror("getsockname() failed");
          return -1;
        }
       printf("Phase 1: The central warehouse has TCP port number %d and IP address %s \n",ntohs(addr.sin_port),inet_ntoa(addr.sin_addr));freeaddrinfo(servinfo); // release it
      // END : print IP address and dynamically generated PORT number--------------//
    
      // START: Receive outlet vectors from all the Stores-------------------------// 
      host = (struct hostent *)gethostbyname(hostN);
        
       if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(WHPORT1);  
        //server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_addr = *((struct in_addr *)host->h_addr); 
        //bzero function used to set the value of server_addr.sin_zero to null        
        bzero(&(server_addr.sin_zero),8); 
        if (bind(tcp_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1) {
            perror("Unable to bind");
            exit(1);
        }
       if (listen(tcp_sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }
        fflush(stdout);
        while(1)
        {  
            sin_size = sizeof(struct sockaddr_in);
            connected = accept(tcp_sock, (struct sockaddr *)&client_addr,&sin_size);
            x=0;            
            while (1)
            {
              //receive data from clients and store in an array for processing
               bytes_received = recv(connected,recv_data,1024,0);
               recv_data[bytes_received]='\0'; 
               //printf("%s", recv_data);      
              
               sscanf(recv_data,"%cx%dx%dx%d",&s,&readData[0],&readData[1],&readData[2]);
              // printf("\nReceived data from client = %d %d %d " ,readData[0],readData[1],readData[2]);
               close(connected);
               
               if(s == 'a')
               { 
                 s1[0]=readData[0];
                 s1[1]=readData[1];
                 s1[2]=readData[2];
                 printf("Phase 1: The central warehouse received information from store 1\n");
                // printf("\nOutlet vector Store 1=<%d,%d,%d>\n",s1[0],s1[1],s1[2]);
                 break;
               }
                else if(s == 'b')
                {
                 s2[0]=readData[0];
                 s2[1]=readData[1];
                 s2[2]=readData[2];
                printf("Phase 1: The central warehouse received information from store 2\n");
                // printf("\nOutlet vector Store 2=<%d,%d,%d>\n",s2[0],s2[1],s2[2]);
                 break;
                 }
                else if(s == 'c')
                {
                  s3[0]=readData[0];
                  s3[1]=readData[1];
                  s3[2]=readData[2];
                  printf("Phase 1: The central warehouse received information from store 3\n");
                 // printf("\nOutlet vector Store 3=<%d,%d,%d>\n",s3[0],s3[1],s3[2]);
                  break;
                }else if(s == 'd')
                {
                  s4[0]=readData[0];
                  s4[1]=readData[1];
                  s4[2]=readData[2];
                  printf("Phase 1: The central warehouse received information from store 4\n");
                 // printf("\nOutlet vector Store 4=<%d,%d,%d>\n",s4[0],s4[1],s4[2]);
                }
                
              break;  
        }
        sleep(1);
       counter++;        
       if(counter == 4)
            break;
}        
// sum the values obtained from the client
      
      for(i=0;i<3;i++)
      {
        sum_vector[i]=s1[i]+s2[i]+s3[i]+s4[i];
        if(sum_vector[i] < 0)
           truck_vector[i]=-(sum_vector[i]);
        else
           truck_vector[i]=0;
       }
        
        fflush(stdout);
        //close TCP connection after truck vector generation
       close(tcp_sock);
       
       printf("End of Phase 1 for the central warehouse\n"); 
       sprintf(a,"%dx%dx%dx\0",truck_vector[0],truck_vector[1],truck_vector[2]);
  // END : Receive outlet vectors from all the Stores-------------------------// 
 //---------------------END OF PHASE 1--------------------------------------------------------//   

 //---------------------START OF PHASE 2------------------------------------------------------//
   
//START: sending truck vector to the Store 1--//   
     if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        //Static UDP port for warehouse when sending truck vector to Store-1 :31629
        server_addr.sin_port = htons(UWHPORT);
        //server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_addr = *((struct in_addr *)host->h_addr); 
        bzero(&(server_addr.sin_zero),8);
        
        client_addr.sin_family = AF_INET;
        //Static UDP port for Store 1 to send truck vector to Store-1 :5629
        client_addr.sin_port = htons(UWHPORT1);
        //client_addr.sin_addr.s_addr = INADDR_ANY;
        client_addr.sin_addr = *((struct in_addr *)host->h_addr); 
        bzero(&(server_addr.sin_zero),8);

      if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind");
            exit(1);
        }

          sendto(sock,a, strlen(a), 0,(struct sockaddr*)&client_addr,sizeof(struct sockaddr));     
          
  printf("Phase 2: The central warehouse has UDP port number %d and IP address %s \n",ntohs(server_addr.sin_port),inet_ntoa(addr.sin_addr));   
      
 printf("Phase 2: Sending the truck-vector to outlet store store_1. The truck vector value is <%d,%d,%d>.\n",truck_vector[0],truck_vector[1],truck_vector[2]);
 //sleep(2);
          //printf("sent");
          fflush(stdout);


 //-----------------------------PHASE 2 PART 3--------------------------//        
// Receive the truck vector back from the store 4-----------------//

   //START: receive final truck vector from store 4--//
       if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
      // printf("socket 2 created");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(UWHPORT2);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        //server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);
 
        if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind");
            exit(1);
         }

   strcpy(recv_data,"\0");
   printf("%s", recv_data);
   while (strcmp(recv_data,"\0")==0)
   {
    recvfrom(sock,recv_data,1024,0,NULL, NULL);
    }
     sscanf(recv_data,"%dx%dx%d",&truck_vector[0],&truck_vector[1],&truck_vector[2]);
  
     printf("Phase 2: The central warehouse has UDP port number %d and IP address %s \n",ntohs(server_addr.sin_port),inet_ntoa(addr.sin_addr));
      printf("Phase 2: The final truck-vector is received from the outlet store store_4, the truck-vector value is: <%d,%d,%d>\n",truck_vector[0],truck_vector[1],truck_vector[2]); 
      printf("End of Phase 2 for the central warehouse\n");
      close(sock);
    // END OF  PHASE 2 ------------------// 
      return 0;
      }
      
      
