//
// Created by galileo on 31/8/17.
//

#ifndef FIRE_FIRE_H
#define FIRE_FIRE_H

#define FIRE_BIND_ADDRESS "tcp://*:5555"
#define ZMQ_BUFFER_SIZE 2048

#define NO_KEY_FOUND "NO_KEY_FOUND"
#define COMMAND_NOT_FOUND "COMMAND_NOT_FOUND"
#define SINGLE " SINGLE"
#define NOOP " NOOP"

bool search(var commands, var incoming_cmd);

var build_redis_mutating_single_key_cmds();

var build_redis_mutating_multiple_key_cmds();

var build_redis_accessor_single_key_cmds();

var build_redis_accessor_multiple_key_cmds();

var build_redis_mutating_multiple_kv_cmds();

void handle_mutating_multiple_keys(char *, char *, void *);

void handle_accessor_multiple_keys(char *, void *);

void handle_single(char *, void *);

char *get_cmd(char *str);

char *get_key();

var get_single_as_json(char *);

#endif //FIRE_FIRE_H
