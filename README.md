# Fire

Fire is a Redis command analyser. 

The purpose of __Fire__ is to act as a Redis command key extractor which will be used by 
[Skyfire](https://github.com/younisshah/skyfire) for determining a sky server to send the given 
command to using Consistent Hashing.

__This is a work in progress__

### Dependencies

Fire uses:

1) [ZeroMQ](zeromq.org)
2) [Cello](http://libcello.org)

### Run
Assuming you've installed `ZeroMQ` and `Cello` and cloned `fire`, run fire like this:

```bash
> gcc -std=gnu99 -Wall ./fire.c -lzmq -lCello -lm -lpthread -o fire
> ./fire
```

#### Licence

GNU-GPL
