CC	=	gcc

DEBUG	=	0

CFLAGS	=	-g -Iinclude -O2 -pipe -DDEBUG=${DEBUG}
LINKOPT	=	-c

AR	=	ar
AROPTS	=	-rcs

TARGET1	=	parse_url
TARGET2	=	http_func
TARGET3	=	main
TARGET4	=	libmicrohttp.a

default:	all test

$(TARGET1).o:	src/$(TARGET1).c
	$(CC) $(LINKOPT) $(CFLAGS) src/$(TARGET1).c -lssl -lcrypto 

$(TARGET2).o:	src/$(TARGET2).c
	$(CC) $(LINKOPT) $(CFLAGS) src/$(TARGET2).c -lssl -lcrypto

$(TARGET3).o:	$(TARGET3).c
	$(CC) $(LINKOPT) $(CFLAGS) $(TARGET3).c -lssl -lcrypto

$(TARGET4):	$(TARGET1).o $(TARGET2).o
	$(AR) $(AROPTS) $(TARGET4) $(TARGET1).o $(TARGET2).o

all:	$(TARGET1).o $(TARGET2).o $(TARGET3).o
	$(CC) $(CFLAGS) $(TARGET1).o $(TARGET2).o $(TARGET3).o -o $(TARGET3) -lssl -lcrypto

test:	$(TARGET3)
	./$(TARGET3) https://www.jan-helbling.ch/

clean:
	rm -rf $(TARGET3) *.o *.a

install: $(TARGET4)
	install -C -m 644 $(TARGET4) /usr/lib
	install -C -m 644 ./include/libmicrohttp.h /usr/include

uninstall:
	rm -f /usr/include/libmicrohttp.h /usr/lib/$(TARGET4)
