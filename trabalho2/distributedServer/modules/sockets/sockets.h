#define DISTRIBUTED_IP "192.168.0.52"
#define DISTRIBUTED_PORT 10101
#define CENTRAL_IP "192.168.0.53"
#define CENTRAL_PORT 10001


void send_data(int code, int value, int decimal);
void *read_data(void *params);

