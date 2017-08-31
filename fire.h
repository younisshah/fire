//
// Created by galileo on 31/8/17.
//

#ifndef FIRE_FIRE_H
#define FIRE_FIRE_H

#define FIRE_BIND_ADDRESS "tcp://*:5555"

void build_redis_mutating_cmd_array();
char* build_redis_accessor_cmd_array();

#endif //FIRE_FIRE_H
