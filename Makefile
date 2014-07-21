TARGET = STAP
OBJS = main.o
 
CFLAGS = -O2 -G0 -Wall -g -std=c99
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
 
LIBDIR =
LDFLAGS =
LIBS = -lpspgum -lpspgu
 
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = STAP Cells
PSP_EBOOT_ICON = icon0.png
PSP_EBOOT_ICON1 = NULL
PSP_EBOOT_UNKPNG = NULL
PSP_EBOOT_PIC1 = NULL
PSP_EBOOT_SND0 = NULL
 
PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak