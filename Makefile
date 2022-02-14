CC = g++
CFLAGS = -o Yacurl

main: main.cpp get.cpp getUrls.cpp
	$(CC) $(CFLAGS) main.cpp get.cpp getUrls.cpp