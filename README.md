# Yacurl
Challenge#1 for the subject "Topicos de telematica"

# Install

To be able to use this code, you need the GNU g++ compiler.

```bash
sudo apt install g++
```
or 

```bash
sudo apt install build-essential
```

## Note 
This code only works on GNU/Linux or OSX because Windows works with another library called WINSOCK

# Compile

To compile the code you need to install make if you install g++ as the first way 

```bash
sudo apt install make
```
On your terminal, you need to put 

```bash
make
```

result 

```bash
egonzalezt@egonzalezt:/mnt/f/Desktop/Sockets/Yacurl$ make
g++ -o Yacurl main.cpp get.cpp getUrls.cpp
```

# Usage

To use this code there are two ways to test it.

## First Way

```bash
./Yacurl <URL> <PORT>
```

## Second Way

using this way by default port 80 is set by default.

```bash
./Yacurl <URL>
```

## Result

When the code finishes the process automatically creates a folder name Result### where are located all the resources found.
