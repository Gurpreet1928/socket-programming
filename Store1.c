/* tcpclient.c */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
//#include <dos.h>

#define WHPORT 21740
#define S1PORT3 5740
#define S1PORT4 6740
#define S1PORT5 9740
#define S1PORT6 7740
#define S1PORT7 8740
#define S1PORT8 11740
int main()
{
      int a[20],yes =1,rv,m; 
      int i, n[5],readData1[20],truck_vector1[20];
      char item[256];  
      char store1[1024],store12[1024];
      int sock,sockfd, bytes_recieved;
      int addr_len, bytes_read;
      char send_data[1024],recv_data[1024];
      struct hostent *host;
      struct sockaddr_in server_addr,client_addr;  
      struct addrinfo hints, *servinfo, *p;
      struct sockaddr_in addr;
      socklen_t addrlen;
      addrlen = sizeof(addr);
      const char hostN[20] = "nunki.usc.edu";
        
    // START:  print IP address and dynamically generated PORT number--------------//
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if ((rv = getaddrinfo("nunki.usc.edu", NULL, &hints, &servinfo)) != 0) {
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
       printf("Phase 1: The Store_1 has TCP port number %d and IP address %s \n",ntohs(addr.sin_port),inet_ntoa(addr.sin_addr));
       
       //freeaddrinfo(servinfo); // release it
      // END : print IP address and dynamically generated PORT number--------------//
       FILE *fp;
	     if((fp = fopen("Store-1.txt", "r")) == NULL) {
	        printf("No such file\n");
	        exit(1);
	     }
       i=0;
	     while(1) {
	        int ret = fscanf(fp, "%s %d" ,item, &a[i]);
	        if(ret == 2)
               m=1;
	            //printf("%d ", a[i]);
	        else if(errno != 0) {
	            perror("Could not read");
	            break;
	        } else if(ret == EOF) {
	            break;
	        } else {
	            printf("No match\n");
	        }
	        i++;
        }
        sprintf(store1,"ax%dx%dx%d\0",a[0],a[1],a[2]);
        //printf("hi");
        fclose(fp);
       //send to warehouse//
 
        host = (struct hostent *)gethostbyname(hostN);
       if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
             perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;     
        server_addr.sin_port = htons(WHPORT);  
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);        
        //server_addr.sin_addr.s_addr = INADDR_ANY;
         bzero(&(server_addr.sin_zero),8); 

        if (connect(sock, (struct sockaddr *)&server_addr,
                    sizeof(struct sockaddr)) == -1) 
        {
            perror("Connect");
             exit(1);
        }

        while(1)
        {
        
          send(sock,store1,strlen(store1), 0); 
         printf("Phase 1: The outlet vector <%d,%d,%d> for store_1 has been sent to the central warehouse\n",a[0],a[1],a[2]);
         printf("End of Phase 1 for store_1\n");
           close(sock);
           break;
          //}
          }
 //--------------Start of phase 2-------------------------//      
//receive truck vector from warehouse---//   
 if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(S1PORT3);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
       // server_addr.sin_addr.s_addr = INADDR_ANY;
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
           sscanf(recv_data,"%dx%dx%dx\0",&readData1[0],&readData1[1],&readData1[2]);
           printf("Phase 2: The Store_1 has UDP port number %d and IP address %s \n",ntohs(server_addr.sin_port),inet_ntoa(addr.sin_addr)); 
            printf("Phase 2: Store_1 received the truck-vector<%d,%d,%d> from the central warehouse\n",readData1[0],readData1[1],readData1[2]);    
               
            for(i=0;i<3;i++)
            {
              truck_vector1[i]=readData1[i]+a[i];
            }
            for(i=0;i<3;i++)
            {
              if(truck_vector1[i]>0)
               a[i]=0;
              else
                 {
                  a[i]=truck_vector1[i];  
                  truck_vector1[i]=0;
                }        
            }
 //Now send the trcuck vector to Store 2 from port : 6629 of Store 1 to port 9629 of Store 2--///
       if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(S1PORT4);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        //server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);
        
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(S1PORT5);
        client_addr.sin_addr = *((struct in_addr *)host->h_addr);
        //client_addr.sin_addr.s_addr = INADDR_ANY;
         bzero(&(server_addr.sin_zero),8);


        if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind");
            exit(1);
         }

      sprintf(store12,"%dx%dx%dx\0", truck_vector1[0],truck_vector1[1],truck_vector1[2]);
      printf("Phase 2: The Store_1 has UDP port number %d and IP address %s \n",ntohs(server_addr.sin_port),inet_ntoa(addr.sin_addr));       
      sendto(sock,store12, strlen(store12), 0,(struct sockaddr*)&client_addr,sizeof(struct sockaddr));     
      printf("Phase2: The updated truck-vector<%d,%d,%d> has been sent to store_2\n",truck_vector1[0],truck_vector1[1],truck_vector1[2]);
      //sleep(2);
      printf("Phase2: Store_1 updated outlet-vector is <%d,%d,%d>\n", a[0],a[1],a[2]);
      fflush(stdout);
 //Second loop : receive data from Store 4 ----------------///  
 //UDP once again to receive and process data from store4 and send it to store2
      if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(S1PORT6);
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
        recvfrom(sock,recv_data,1024,0, NULL, NULL);
       }
       //sleep(2);
       sscanf(recv_data,"%dx%dx%dx\0",&readData1[0],&readData1[1],&readData1[2]);
       printf("Phase 2: The Store_1 has UDP port number %d and IP address %s \n",ntohs(server_addr.sin_port),inet_ntoa(addr.sin_addr));
       printf("Phase 2: Truck-vector<%d,%d,%d> has been received from Store_4\n",readData1[0],readData1[1],readData1[2]);  
        for(i=0;i<3;i++)
         {
              truck_vector1[i]=readData1[i]+a[i];
         }
         for(i=0;i<3;i++)
          {
              if(truck_vector1[i]>0)
               a[i]=0;
                 else
                 {
                    a[i]=truck_vector1[i];    
                    truck_vector1[i]=0;
                }        
          }
//Now send the trcuck vector to Store 2 from port : 8629 of Store 1 to port 11629 of Store 2--///
       if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(S1PORT7);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
       // server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);
        
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(S1PORT8);
        client_addr.sin_addr = *((struct in_addr *)host->h_addr);
        //client_addr.sin_addr.s_addr = INADDR_ANY;
         bzero(&(server_addr.sin_zero),8);


        if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Bind");
            exit(1);
         }

        sprintf(store12,"%dx%dx%dx\0", truck_vector1[0],truck_vector1[1],truck_vector1[2]);
        //send the truck vector to the client
        sendto(sock,store12, strlen(store12), 0,(struct sockaddr*)&client_addr,sizeof(struct sockaddr));  
        sleep(2);        
        printf("Phase 2: The Store_1 has UDP port number %d and IP address %s \n",ntohs(server_addr.sin_port),inet_ntoa(addr.sin_addr));       
        printf("Phase2: The updated truck-vector<%d,%d,%d> has been sent to store_2\n",truck_vector1[0],truck_vector1[1],truck_vector1[2]);
         printf("Phase2: Store_1 updated outlet-vector is <%d,%d,%d>\n", a[0],a[1],a[2]); 
        fflush(stdout);
        printf("End of Phase 2 for store_1\n"); 
        close(sock);
   
     return 0;
}