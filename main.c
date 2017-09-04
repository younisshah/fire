#include <zmq.h>
#include <assert.h>
#include "Cello.h"
#include "fire.h"

//
// Created by galileo on 31/8/17.
//

int main(int argc, char **argv) {

    var mutating_commands = build_redis_mutating_single_key_cmds();

    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);

    puts("[+] Starting ZMQ fire TCP server.");
    printf("[+] Binding to %s\n", FIRE_BIND_ADDRESS);

    int rc = zmq_bind(responder, FIRE_BIND_ADDRESS);
    assert(rc == 0);

    puts("[+] DONE. Waiting for incoming fire requests...");

    char buffer[ZMQ_BUFFER_SIZE];
    while (1) {
        zmq_recv(responder, buffer, ZMQ_BUFFER_SIZE, 0);
        printf("Fire received:  %s\n", buffer);
        char *cmd_name = get_cmd(buffer);
        printf("cmd: %s\n", cmd_name);
        if (cmd_name != NULL) {
            if (search(mutating_commands, $S(cmd_name)) == true) {
                char *key = get_key();
                if (key != NULL) {
                    printf("KEY: %s-\n", key);
                    puts("[+] Sending key...");
                    zmq_send(responder, key, len($S(key)), 0);
                    puts("[+] DONE.");
                } else {
                    /// KEY not present TODO
                    char *not_found  = "No redis key found!";
                    zmq_send(responder, not_found, len($S(not_found)), 0);
                }
            }
        } else {
            char *no_cmd = "No command found";
            zmq_send(responder, no_cmd, len($S(no_cmd)), 0);
        }
    }
}

/**
 * Get the Redis command name.
 * @param str
 * @return
 */
char *get_cmd(char *str) {
    char *token = strtok(str, " ");
    if (token != NULL) {
        return token;
    }
    return NULL;
}

/**
 * Get the Redis command key.
 * This should be called after calling `get_cmd(char *)`.
 * @return
 */
char *get_key() {
    char *token = strtok(NULL, " ");
    if (token != NULL) {
        return token;
    }
    return NULL;
}

var build_redis_mutating_single_key_cmds() {

    var commands = new(Array, String);
    push(commands, $S("APPEND"));

    push(commands, $S("DECR"));
    push(commands, $S("DECRBY"));
    push(commands, $S("DEL"));

    push(commands, $S("GEOADD"));
    push(commands, $S("GEOHASH"));
    push(commands, $S("GETSET"));

    push(commands, $S("HDEL"));
    push(commands, $S("HINCRBY"));
    push(commands, $S("HINCRBYFLOAT"));
    push(commands, $S("HMSET"));
    push(commands, $S("HSET"));
    push(commands, $S("HSETNX"));

    push(commands, $S("INCR"));
    push(commands, $S("INCRBY"));
    push(commands, $S("INCRFLOAT"));

    push(commands, $S("LINSERT"));
    push(commands, $S("LPOP"));
    push(commands, $S("LPUSH"));
    push(commands, $S("LPUSHX"));
    push(commands, $S("LREM"));
    push(commands, $S("LSET"));
    push(commands, $S("LTRIM"));

    push(commands, $S("MOVE"));

    push(commands, $S("PERSIST"));
    push(commands, $S("PEXPIRE"));
    push(commands, $S("PEXPIREAT"));
    push(commands, $S("PFADD"));
    push(commands, $S("PSETEX"));

    push(commands, $S("RENAME"));
    push(commands, $S("RENAMENX"));
    push(commands, $S("RESTORE"));
    push(commands, $S("RPOP"));
    push(commands, $S("RPUSH"));
    push(commands, $S("RPUSHX"));

    push(commands, $S("SADD"));
    push(commands, $S("SET"));
    push(commands, $S("SETBIT"));
    push(commands, $S("SETEX"));
    push(commands, $S("SETNX"));
    push(commands, $S("SETRANGE"));
    push(commands, $S("SPOP"));

    push(commands, $S("ZADD"));
    push(commands, $S("ZINCRBY"));

    return commands;
}

bool search(var commands, var incoming_cmd) {
    return mem(commands, incoming_cmd);
}
