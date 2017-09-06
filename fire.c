#include <zmq.h>
#include <assert.h>
#include "Cello.h"
#include "fire.h"

//
// Created by galileo on 31/8/17.
//

int main(int argc, char **argv) {

    var sing_mutating_commands = build_redis_mutating_single_key_cmds();
    var mul_mutating_keys_commands = build_redis_mutating_multiple_key_cmds();
    var mul_mutating_kv_commands = build_redis_mutating_multiple_kv_cmds();

    var sing_accessor_commands = build_redis_accessor_single_key_cmds();
    var mul_accessor_commands = build_redis_accessor_multiple_key_cmds();

    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);

    puts("[+] Starting ZMQ fire TCP server.");
    printf("[+] Binding to %s\n", FIRE_BIND_ADDRESS);

    int rc = zmq_bind(responder, FIRE_BIND_ADDRESS);
    assert(rc == 0);

    puts("[+] DONE. Waiting for incoming fire requests");

    char buffer[ZMQ_BUFFER_SIZE];
    while (1) {
        zmq_recv(responder, buffer, ZMQ_BUFFER_SIZE, 0);
        printf("Fire received:  %s\n", buffer);
        char *command = strdup(buffer);
        char *cmd_name = get_cmd(buffer);
        if (cmd_name != NULL) {
            if ((search(sing_mutating_commands, $S(cmd_name)) == true) ||
                ((search(sing_accessor_commands, $S(cmd_name)) == true))) {
                char *key = get_key();
                handle_single(key, responder);
            } else if ((search(mul_mutating_keys_commands, $S(cmd_name))) == true) {
                handle_multiple_keys(cmd_name, command, responder);
            } else {
                zmq_send(responder, COMMAND_NOT_FOUND, 18, 0);
            }
        } else {
            zmq_send(responder, COMMAND_NOT_FOUND, 18, 0);
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

/**
 * Handles Single key mutator or accessor Redis command
 * @param key
 * @param responder
 */
void handle_single(char *key, void *responder) {
    if (key != NULL) {
        puts("[+] Sending key and metadata...");
        var json = get_single_as_json(key);
        zmq_send(responder, c_str(json), len(json), 0);
        puts("[+] DONE.");
    } else {
        zmq_send(responder, NO_KEY_FOUND, 13, 0);
    }
}

var get_single_as_json(char *key) {
    var json = new(String, $S("{\"key\":"));
    var cello_key = new(String, $S(key));
    append(json, cello_key);
    append(json, $S(", \"key_kind\": \"SINGLE\", \"cmd_kind\": \"NOOP\"}"));
    return json;
}

void handle_multiple_keys(char *cmd_name, char *full_cmd, void *responder) {

    if (strcmp(cmd_name, "UNLINK") == 0) {
        var keys = new(Array, String);
        char *token = NULL;
        strtok(full_cmd, " "); // Get rid of the first token which is the command name.
        do {
            token = strtok(NULL, " ");
            if (token != NULL) {
                push(keys, $S(strdup(token)));
            }
        } while (token != NULL);
        var json = new(String, $S("\"{\"keys\":["));
        size_t count = len(keys);
        for (int i = 0; i < count; i++) {
            char *k = c_str(get(keys, $I(i)));
            append(json, $S(k));
            if (i == count - 1) {
                append(json, $S("]"));
            } else {
                append(json, $S(","));
            }
        }
        append(json, $S(", \"key_kind\": \"MULTIPLE\", \"cmd_kind\": \"K\"}\""));
        zmq_send(responder, c_str(json), len(json), 0);
    }
}

var build_redis_mutating_single_key_cmds() {

    var commands = new(Array, String);
    push(commands, $S("APPEND"));

    push(commands, $S("DECR"));
    push(commands, $S("DECRBY"));
    push(commands, $S("DEL"));

    push(commands, $S("EXPIRE"));
    push(commands, $S("EXPIREAT"));

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
    push(commands, $S("SREM"));

    push(commands, $S("ZADD"));
    push(commands, $S("ZINCRBY"));

    return commands;
}

var build_redis_mutating_multiple_key_cmds() {
    var commands = new(Array, String);

    push(commands, $S("BLPOP"));
    push(commands, $S("BRPOP"));

    push(commands, $S("UNLINK"));

    return commands;
}

var build_redis_mutating_multiple_kv_cmds() {
    var commands = new(Array, String);

    push(commands, $S("MSET"));
    push(commands, $S("MSETNX"));

    return commands;
}

var build_redis_accessor_single_key_cmds() {
    var commands = new(Array, String);

    push(commands, $S("GEOPOS"));
    push(commands, $S("GEODIST"));
    push(commands, $S("GEORADIUS"));
    push(commands, $S("GEORADIUSBYMEMBER"));
    push(commands, $S("GET"));
    push(commands, $S("GETBIT"));
    push(commands, $S("GETRANGE"));

    push(commands, $S("HEXISTS"));
    push(commands, $S("HGET"));
    push(commands, $S("HGETALL"));
    push(commands, $S("HKEYS"));
    push(commands, $S("HLEN"));
    push(commands, $S("HMGET"));
    push(commands, $S("HSTRLEN"));
    push(commands, $S("HVALS"));
    push(commands, $S("HSCAN"));

    push(commands, $S("LINDEX"));
    push(commands, $S("LLEN"));
    push(commands, $S("LRANGE"));

    push(commands, $S("PTTL"));

    push(commands, $S("SCARD"));
    push(commands, $S("SISMEMBER"));
    push(commands, $S("SISMEMBERS"));
    push(commands, $S("SRANDMEMBER"));
    push(commands, $S("STRLEN"));

    push(commands, $S("TTL"));
    push(commands, $S("TYPE"));

    push(commands, $S("ZCARD"));
    push(commands, $S("ZCOUNT"));
    push(commands, $S("ZLEXCOUNT"));
    push(commands, $S("ZRANGE"));
    push(commands, $S("ZRANGEBYLX"));
    push(commands, $S("ZREVRANGEBYLX"));
    push(commands, $S("ZRANGEBYSCORE"));
    push(commands, $S("ZRANK"));
    push(commands, $S("ZREM"));
    push(commands, $S("ZREMBYLX"));
    push(commands, $S("ZREMRANGEBYLX"));
    push(commands, $S("ZREMRANGEBYRANK"));
    push(commands, $S("ZREMRANGEBYSCORE"));
    push(commands, $S("ZREVRANGE"));
    push(commands, $S("ZREVRANGEBYSCORE"));
    push(commands, $S("ZREVRANK"));
    push(commands, $S("ZSCORE"));
    push(commands, $S("ZSCAN"));

    return commands;
}

var build_redis_accessor_multiple_key_cmds() {
    var commands = new(Array, String);

    push(commands, $S("EXISTS"));

    push(commands, $S("MGET"));

    push(commands, $S("PFCOUNT"));

    return commands;
}

bool search(var commands, var incoming_cmd) {
    return mem(commands, incoming_cmd);
}
