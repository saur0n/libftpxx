################################################################################
#   FTP access library (ftplib C++ wrapper)
#   Build rules
#   
#   © 2017, Sauron
################################################################################

CC=g++
CXXFLAGS=-shared -lftp -fPIC -std=gnu++11 -O3 -Wall 
SOURCES=*.cpp
HEADERS=*.hpp
OUTPUT=libftp++.so

all: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CC) $(CXXFLAGS) -o $(OUTPUT) $(SOURCES)

clean:
	rm -f *.o *.so

install:
	mv $(OUTPUT) /usr/lib64
	mkdir -p /usr/include/$(NAME)
	cp $(HEADERS) /usr/include/$(NAME)

.PHONY:
	all install clean
