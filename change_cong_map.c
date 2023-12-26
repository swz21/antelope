#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <linux/bpf.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

int main(int argc, char **argv) {
    // int obj_fd = bpf_obj_get("/sys/fs/bpf/tcp_prog");

    // struct bpf_object *obj;

    
    
    // long key1 = 20;
    // char value1[20] = "helloworld";
    long key1 = atoi(argv[1]);
    char value1[20];
    strncpy(value1, argv[2], sizeof(value1));

    // long key2 = atoi(argv[3]);
    // char value2[20];
    // strcpy(value2, argv[4], sizeof(value2));

    long result1, result2;

    if (key1 > 65535) {
        int ip_cong_map_fd = bpf_obj_get("/sys/fs/bpf/ip_cong_map");
        result1 = bpf_map_update_elem(ip_cong_map_fd, &key1, &value1, BPF_ANY);
    }
    // result2 = bpf_map_update_elem(ip_cong_map_fd, &key2, &value2, BPF_ANY);
    else {
        int cong_map_fd = bpf_obj_get("/sys/fs/bpf/cong_map");
        result1 = bpf_map_update_elem(cong_map_fd, &key1, &value1, BPF_ANY);
    }

    // char *con_str = bpf_map_lookup_elem(cong_map_fd, &key1);

    // printf("cong:%d, ip_cong:%d\n", cong_map_fd, ip_cong_map_fd);
    // printf("result1: %ld, result2: %ld\n", result1, result2);
    if (result1 == 0)
		printf("Map updated with new element\n");
	else
		printf("Failed to update map with new value: %ld (%s)\n", result1, strerror(errno));

    return;
}