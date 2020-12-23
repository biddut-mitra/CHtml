#CHTML Version 3.28

CC=c++
#CC=g++
OPTION= -D __RELEASE__

MV=/bin/mv
MKDIR=/bin/mkdir
RM=/bin/rm -f
RMDIR=/bin/rm -r -f

INSTALL=/usr/bin/install

INSTALL_LIB=/usr/local/lib/

INSTALL_H=/usr/local/include/chtml

LIB= -L/usr/lib -L/usr/local/lib
INC= -I/usr/include -I/usr/local/include

.SUFFIXES: .cpp .o 

.cpp.o:  
	$(CC) -c $(SOURCE_FILES) $(INC) $(OPTION)

GENERATED_FILES= libchtml.a \
				libchtml.so.3.2.8

SOURCE_FILES=CHtml.cpp 

all: $(GENERATED_FILES)

HEADERS_FILES=CHtml.h 
	
OBJS= CHtml.o 

$(OBJS): $(SOURCE_FILES) $(HEADERS_FILES)

libchtml.a:$(OBJS) 
	$(AR) -rv libchtml.a $(OBJS)
	
libchtml.so.3.2.8:$(OBJS) 
	$(CC) -shared -Wl,-soname,libchtml.so.3 -o libchtml.so.3.2.8
	
clean:
	$(RM) *.o $(GENERATED_FILES) 
		
install:$(GENERATED_FILES)
	$(INSTALL) -m 0744 libchtml.a $(INSTALL_LIB)   
	$(INSTALL) -m 0744 libchtml.so.3.2.8 $(INSTALL_LIB)   
	$(MKDIR) $(INSTALL_H)
	$(INSTALL) -m 0444 $(HEADERS_FILES) $(INSTALL_H)  
	$(INSTALL) -m 0444 chtml.3.gz  /usr/share/man/man3/
uninstall:
	$(RM)  /usr/share/man/man3/chtml.3.gz
	$(RM) $(INSTALL_LIB)libchtml.so.3.2.8
	$(RM) $(INSTALL_LIB)libchtml.a
	$(RMDIR) $(INSTALL_H)


