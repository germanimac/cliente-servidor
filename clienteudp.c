#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>

#define DATA "Esta eh a mensagem que quero enviar"

main(argc, argv)
     int argc;
     char *argv[];

{
    char buf[1024];
    char msg[100];
    int sock,tamanho, length;
	struct sockaddr_in name;
	struct hostent *hp, *gethostbyname();

        /* Cria o socket de comunicacao */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0) {
	/*
	/- houve erro na abertura do socket
	*/
		perror("opening datagram socket");
		exit(1);
	}
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = 0;
    if (bind(sock,(struct sockaddr *)&name, sizeof name ) < 0) {
        perror("binding datagram socket");
        exit(1);
    }
    length = sizeof(name);
    if (getsockname(sock,(struct sockaddr *)&name, &length) < 0) {
        perror("getting socket name");
        exit(1);
    }
    printf("Socket port #%d\n",ntohs(name.sin_port));

	/* Associa */
        hp = gethostbyname(argv[1]);
        if (hp==0) {
            fprintf(stderr, "%s: unknown host ", argv[1]);
            exit(2);
        }
        bcopy ((char *)hp->h_addr, (char *)&name.sin_addr, hp->h_length);
	name.sin_family = AF_INET;
	name.sin_port = htons(atoi(argv[2]));
    gets(msg);
    if (sendto (sock,msg,sizeof msg, 0, (struct sockaddr *)&name,
                sizeof name)<0)
        perror("sending datagram message");
        
	/* Envia */
    if(fork() == 0){
        while(1){
        gets(msg);
            if (sendto (sock,msg,sizeof msg, 0, (struct sockaddr *)&name,
                        sizeof name)<0)
                    perror("sending datagram message");
        }
        
    }
        else{
            while(1){
            if(recvfrom(sock,buf,1024,0,(struct sockaddr *)&name,&tamanho)<0)
                perror("receiving datagram packet");
                printf("  %s\n", buf);
                
            }
    }
    close(sock);
    exit(0);
}
