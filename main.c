#include <zmq.h>
#include <assert.h>
#include "fire.h"

//
// Created by galileo on 31/8/17.
//

int main(int argc, char **argv) {
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    puts("[+] Starting ZMQ fire TCP server.");
    printf("[+] Binding to %s\n", FIRE_BIND_ADDRESS);
    int rc = zmq_bind(responder, FIRE_BIND_ADDRESS);
    assert(rc == 0);
    puts("[+] DONE. Waiting for incoming fire requests...");
    while (1) {
        char buffer[10];
        zmq_recv(responder, buffer, 10, 0);
        printf("Recvd: %s\n", buffer);
        zmq_send(responder, "Merci", 5, 0);
    }
}
