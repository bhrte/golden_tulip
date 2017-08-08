
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <memory.h>

#define GETDEEPFIFO             0x54AA
#define SETDEEPFIFO             0x54AB
#define SETFCR                  0x54BA
#define SETTTR			0x54B1
#define SETRTR			0x54B2
#define GETTTR			0x54B3
#define GETRTR			0x54B4

#define TIOCGGETNRPORTS         0x546C //number of port for arg
#define TIOCGNUMOFPORT          0x545F //number of all port
#define TIOCGGETDEVID           0x5468
#define TIOCGGETBDNO            0x5469 //number of board
#define TIOCGGETINTERFACE       0x546A
#define TIOCGGETREV             0x546B
#define TIOCGGETPORTTYPE        0x546D

#define TIOCSMULTIECHO          0x5440
#define TIOCSPTPNOECHO          0x5441

#define ENABLE	1
#define DISABLE	0
int fcr[256];
void Usage(void)
{
	printf("Usage     : ./sb_ioctl Command [Port Name] [Parameter]\n");
	printf("Command   : GETINFO     > Print All Port Infomation(DeepFifo,FCR,TTR,RTR)\n");
	printf("          : setdeepfifo > Setting [Port Name] of deepfifo to [Parameter]\n");
	printf("	  : getdeepfifo > Print [Port Name] of deepfifo\n");
	printf("	  : setfcr      > Setting [Port Name] of FCR to [Parameter]\n");
	printf("	  : getfcr      > Print [Port Name] of FCR\n");
	printf("	  : setttr      > Setting [Port Name] of TTR to [Parameter]\n");
	printf("	  : getttr      > Print [Port Name] of TTR\n");
	printf("	  : setrtr      > Setting [Port Name] of RTR to [Parameter]\n");
	printf("	  : getrtr      > Print [Port Name] of RTR\n");
	printf("Port Name : /dev/ttyMP0 ~ /dev/ttyMP256\n");
	printf("Parameter : Parameter is Hexadecimal Number\n");
}


void ReadFCR(int maxPort)
{
	FILE *fp;
        int ret,i,fd;
        char tmp[100];
	char temp;
	fpos_t fa;
	
	for(i=0;i<maxPort;i++)
		fcr[i] = 1;

        if((fp=fopen("/etc/sb_mp","r+"))<0){
                printf("Can't Open /etc/sb_mp");
                return;
        }
	fseek(fp,17,SEEK_SET);
	
	for(i=0;i<maxPort;i++){
		ret = fscanf(fp,"%x,",&fcr[i]);
	}

	fclose(fp);

}

void GetInfo(int maxPort)
{
	int i,ret,fd;
	char tmp[100];

	printf("===DEEPFIFO INFO===");
	for(i=0;i<maxPort;i++){
                sprintf(tmp,"/dev/ttyMP%d",i);
                 if( (fd = open(tmp,O_RDWR)) < 0 ){
                        printf("Can't open /dev/ttyMP%d",i);
                         printf("Please check your board in slot..\n");
                        return ;
                 }
		if((i%8)==0)
			printf("\n");
                ret = ioctl(fd,GETDEEPFIFO,10);
                printf("MP%d=%x\t",i,ret);
                close(fd);
        }
	printf("\n\n\n");
	
	printf("===TTRINFO===");
        for(i=0;i<maxPort;i++){
                sprintf(tmp,"/dev/ttyMP%d",i);
                 if( (fd = open(tmp,O_RDWR)) < 0 ){
                        printf("Can't open /dev/ttyMP%d",i);
                         printf("Please check your board in slot..\n");
                        return ;
                 }
		if((i%8)==0)
                        printf("\n");
                ret = ioctl(fd,GETTTR,10);
                printf("MP%d=%x\t",i,ret);
                close(fd);
        }
        printf("\n\n\n");

	printf("===RTRINFO===");
        for(i=0;i<maxPort;i++){
                sprintf(tmp,"/dev/ttyMP%d",i);
                 if( (fd = open(tmp,O_RDWR)) < 0 ){
                        printf("Can't open /dev/ttyMP%d",i);
                         printf("Please check your board in slot..\n");
                        return ;
                 }
		if((i%8)==0)
                        printf("\n");
                ret = ioctl(fd,GETRTR,10);
                printf("MP%d=%x\t",i,ret);
                close(fd);
        }
        printf("\n\n\n");

	printf("===FCRINFO===");
        for(i=0;i<maxPort;i++){
		if((i%8)==0)
                        printf("\n");
                printf("MP%d=%x\t",i,fcr[i]);
        }
        printf("\n");

}
void Save(int maxPort)
{
	FILE *fp;
	int ret,i,fd;
	char tmp[100];

	if((fp=fopen("/etc/sb_mp","r+"))<0){
		printf("Can't Open /etc/sb_mp");
		return;
	}
	fprintf(fp,"#!/bin/bash\n\n");

        fprintf(fp,"fcr=");
        for(i=0;i<maxPort;i++){
                if((i+1)==maxPort)
                        fprintf(fp,"%x",fcr[i]);
                else
                        fprintf(fp,"%x,",fcr[i]);
        }


	/* SAVE DEEP FIFO SETTING */
	fprintf(fp,"\n\ndeep=");
	for(i=0;i<maxPort;i++){
                sprintf(tmp,"/dev/ttyMP%d",i);
                 if( (fd = open(tmp,O_RDWR)) < 0 ){
                        printf("Can't open /dev/ttyMP%d",i);
                         printf("Please check your board in slot..\n");
                        return ;
                 }
                ret = ioctl(fd,GETDEEPFIFO,10);
		if((i+1)==maxPort)
			fprintf(fp,"%x",ret);
		else
			fprintf(fp,"%x,",ret);
                close(fd);
        }
	
	/* SAVE TTR SETTING */
	fprintf(fp,"\n\nttr=");
        for(i=0;i<maxPort;i++){
                sprintf(tmp,"/dev/ttyMP%d",i);
                 if( (fd = open(tmp,O_RDWR)) < 0 ){
                        printf("Can't open /dev/ttyMP%d",i);
                         printf("Please check your board in slot..\n");
                        return ;
                 }
                ret = ioctl(fd,GETTTR,0);
                if((i+1)==maxPort)
                        fprintf(fp,"%x",ret);
                else
                        fprintf(fp,"%x,",ret);
                close(fd);
        }

	/* SAVE RTR SETTING */
        fprintf(fp,"\n\nrtr=");
        for(i=0;i<maxPort;i++){
                sprintf(tmp,"/dev/ttyMP%d",i);
                 if( (fd = open(tmp,O_RDWR)) < 0 ){
                        printf("Can't open /dev/ttyMP%d",i);
                         printf("Please check your board in slot..\n");
                        return ;
                 }
                ret = ioctl(fd,GETRTR,0);
                if((i+1)==maxPort)
                        fprintf(fp,"%x",ret);
                else
                        fprintf(fp,"%x,",ret);
                close(fd);
        }
	

	fprintf(fp,"\n\nmodprobe golden_tulip deep=$deep rtr=$rtr ttr=$ttr fcr_arr=$fcr\n\n");
	fclose(fp);

}

void SET_PORT(char *port, char *cmd, int num)
{
	int fd, ret;
	int pt;

	if((fd=open(port,O_RDWR))<0){
                printf("Can't open %s\n",port);
                printf("Please check your board in slot..\n");
                     return ;
         }

	if(strcmp(cmd,"setdeepfifo")==0){
        	 ret = ioctl(fd,SETDEEPFIFO,num);
		printf("ret = %d\n",ret);
	}
	else if(strcmp(cmd,"getdeepfifo")==0){
		ret = ioctl(fd,GETDEEPFIFO,num);
		printf("%s_DEEPFIFO = %x\n",port,ret);
	}
	else if(strcmp(cmd,"setfcr")==0){
		ret  = ioctl(fd,SETFCR,num);
		sscanf(port,"/dev/ttyMP%d",&pt);
		fcr[pt] = num;	
		printf("ret = %d\n",ret);
	}
	else if(strcmp(cmd,"getfcr")==0){
		sscanf(port,"/dev/ttyMP%d",&pt);
		printf("%s_FCR = %x\n",port,fcr[pt]);
	}	

	else if(strcmp(cmd,"getttr")==0){
		ret = ioctl(fd,GETTTR,0);
		printf("%s_TTR = %x\n",port,ret);
	}
	else if(strcmp(cmd,"setttr")==0){
		ret = ioctl(fd,SETTTR,num);
		printf("ret = %d\n",ret);
	}
	else if(strcmp(cmd,"getrtr")==0){
		ret = ioctl(fd,GETRTR,0);
		printf("%s_RTR = %x\n",port,ret);
	}	
	else if(strcmp(cmd,"setrtr")==0){
		ret = ioctl(fd,SETRTR,0);
		printf("ret = %d\n",ret);
	}
        close(fd);


}

int main(int argc, char *argv[])
{
 	int ret,all_port;
	int fd,i;
	int hex;
	if( (fd = open("/dev/ttyMP0",O_RDWR)) < 0 ){
                printf("Can't open /dev/ttyMP0\n");
                printf("Please check your board in slot..\n");
                return -1;
        }
        all_port = ioctl(fd,TIOCGNUMOFPORT,0);

	close(fd);
	
	ReadFCR(all_port);
	
	if(argc>1){
		if(strcmp(argv[1],"GETINFO")==0)
			GetInfo(all_port);
		else{
			sscanf(argv[3],"%x",&hex);

			SET_PORT(argv[2],argv[1],hex);
		}
	}
	else
	{
		Usage();	
	}
	Save(all_port);

	return 0;	
			
}
