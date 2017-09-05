//
// Created by galileo on 30/8/17.
//

#include <zmq.h>

int main(void) {
    printf("Connecting to hello world server…\n");
    void *context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);
    zmq_connect(requester, "tcp://localhost:5555");

    char buffer[100];
    printf("Sending Hola\n");
    zmq_send(requester, "UNLINK key1 key2 key3 key4", 100, 0);
    zmq_recv(requester, buffer, 100, 0);
    printf("Received %s", buffer);
    zmq_close(requester);
    zmq_ctx_destroy(context);
    return 0;
}