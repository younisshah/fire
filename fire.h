//
// Created by galileo on 31/8/17.
//

#ifndef FIRE_FIRE_H
#define FIRE_FIRE_H

#define FIRE_BIND_ADDRESS "tcp://*:5555"
#define ZMQ_BUFFER_SIZE 512

bool search(var commands, var incoming_cmd);

var build_redis_mutating_single_key_cmds();

char *build_redis_accessor_cmd_array();

char *get_cmd(char *str);

char *get_key();

#endif //FIRE_FIRE_H
