all:
	clang++ -Oz -march=native -fno-stack-protector -s -o battery_monitor battery_monitor.cpp
clean:
	rm battery_monitor
install:
	cp battery_monitor /usr/bin/battery_monitor
