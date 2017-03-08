gcc -fPIC -c cpu.cpp sched_test.cpp process.cpp --std=c++11 -lstdc++
gcc -shared -o libschedtest.so.1.0.1 sched_test.o process.o cpu.o
cp libschedtest.so.1.0.1 /usr/local/lib/