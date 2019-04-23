CC	=	gcc
CFLAGS	=	-g -Iinclude
LINKOPT	=	-c 

AR	=	ar
AROPTS	=	-rcs

TARGET1	=	parse_url
TARGET2	=	http_func
TARGET3	=	main
TARGET4	=	libmicrohttp.a
TARGET5	=	shell_colors

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
	./$(TARGET3)

clean:
	rm -rf $(TARGET3) *.o *.a

install: $(TARGET4)
	install -C -m 644 $(TARGET4) /usr/lib
	install -C -m 644 ./include/$(TARGET1).h /usr/include
	install -C -m 644 ./include/$(TARGET2).h /usr/include
	install -C -m 644 ./include/$(TARGET5).h /usr/include

uninstall:
	rm -f /usr/include/$(TARGET1).h /usr/include/$(TARGET2).h /usr/include/$(TARGET5).h /usr/lib/$(TARGET4)
