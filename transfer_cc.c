#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/in.h>


// char cong[5][9]={"bbr","cubic","illinois","c2tcp","westwood"};
char cong[5][9]={"bbr","cubic","illinois","westwood", "vegas"};

void updatehash(unsigned long key, char *val)
{
        char cmd[1000];
        sprintf(cmd, "sudo ./change_cong_map %lu %s", key, val);
        

        char data[10] = {0};
        strcpy(data, val);

        // sem_p(sem_id, 1);
        printf("parent data: %s\n", data);
        system(cmd);
        // sem_v(sem_id, 0);
        printf("finish update hash.\n");

}

//因为python无法向c传递long和string，于是就将long拆分后进行传输
 void updateCongHash(long pk, long beishu,long yushu, int pv,int ipv)
{
    printf("enter update cong hash\n");
    printf("pv:%d,ipv:%d,pk:%ld,beishu:%ld,yushu:%ld\n", pv,ipv,pk,beishu,yushu);
    if (sem_id == 0)
    {
        init();
    }
    printf("id: %d\n", sem_id);

    char *pval=cong[pv];
    char *ipval=cong[ipv];
    long ipk= beishu*1000+yushu;
    printf("ps:%s,ips:%s,pk:%ld,ipk:%lld\n", pval,ipval, (long)pk,ipk);

    updatehash((long)pk, pval);
    printf("begin to update ipk\n");
    updatehash((unsigned long)ipk, ipval);
    printf("finish update all hash\n");

    return;
}
