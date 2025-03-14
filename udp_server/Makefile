TARGET_PC = server
CC_BD = aarch64-linux-gnu-gcc
CC_PC = gcc
CFLAGS = -Wall

all:
	gcc -o $(TARGET_PC) $(TARGET_PC).c $(shell pkg-config --cflags --libs libjpeg) -pthread
# $(TARGET_PC): $(TARGET_PC).c
# 	$(CC_PC) $(CFLAGS) -pthread -o $@ $<

clean:
	rm -f $(TARGET_PC)


