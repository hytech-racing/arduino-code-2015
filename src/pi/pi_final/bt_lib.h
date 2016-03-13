#ifndef __BT_LIB_H__
#define __BT_LIB_H__

#include <stdint.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

class BT {
public:
    BT(int port);
    void connect();
    void disconnect();
    int read();
    int send(uint8_t* data);
    int send(uint8_t* data, uint8_t length);

private:
    int sock;
    int client;
    int port;

    static const int DATA_LENGTH = 256;
    uint8_t buf[DATA_LENGTH] = { 0 };

    struct sockaddr_rc loc_addr = { 0 };
    struct sockaddr_rc rem_addr = { 0 };
};

#endif
