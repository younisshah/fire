//
// Created by galileo on 31/8/17.
//
#include "Cello.h";
#include "fire.h";

void build_redis_mutating_cmd_array() {
    var commands = new(Array, String);
    push(commands, $S("SET"));
    push(commands, $S("GET"));
    push(commands, $S("GETBIT"));
    push(commands, $S("GETSET"));
    push(commands, $S("HDEL"));
    push(commands, $S("HEXISTS"));
    push(commands, $S("HGET"));
    push(commands, $S("HGETALL"));
    push(commands, $S("HINCRBY"));
    push(commands, $S("HINCRBYFLOAT"));
    push(commands, $S("HKEYS"));
    push(commands, $S("HLEN"));
    push(commands, $S("DEL"));
    push(commands, $S("APPEND"));
    push(commands, $S("DECR"));
    push(commands, $S("DUMP"));
    push(commands, $S("EXPIRE"));
    push(commands, $S("EXPIRE"));
}
