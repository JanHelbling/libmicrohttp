CC	=	gcc
CFLAGS	=	-g -Iinclude
LINKOPT	=	-c

AR	=	ar
AROPTS	=	-rcs

TARGET1	=	parse_url
TARGET2	=	http_func
TARGET3	=	main
TARGET4	=	libmicrohttp.a

default:	all test

$(TARGET1).o:	src/$(TARGET1).c
	$(CC) $(LINKOPT) $(CFLAGS) src/$(TARGET1).c

$(TARGET2).o:	src/$(TARGET2).c
	$(CC) $(LINKOPT) $(CFLAGS) src/$(TARGET2).c

$(TARGET3).o:	$(TARGET3).c
	$(CC) $(LINKOPT) $(CFLAGS) $(TARGET3).c

$(TARGET4):	$(TARGET1).o $(TARGET2).o
	$(AR) $(AROPTS) $(TARGET4) $(TARGET1).o $(TARGET2).o

all:	$(TARGET1).o $(TARGET2).o $(TARGET3).o
	$(CC) $(CFLAGS) $(TARGET1).o $(TARGET2).o $(TARGET3).o -o $(TARGET3)

test:	$(TARGET3)
	./$(TARGET3)

clean:
	rm -rf $(TARGET3) *.o *.a
