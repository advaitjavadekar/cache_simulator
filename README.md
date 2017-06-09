# cache_simulator
Multilevel cache simulator using C++. L1 and L2 cache write-back write-allocate policy with replacement policies like lru and fifo. L2 cache of type non-inclusive,inclusive and exclusive.

example trace file is check.t
make it and run it using simulator parameters.
L1 and L2 cache size and associativities and replacement policies.
./cache_sim 64 32768 4 262144 8 1 1 check.t
