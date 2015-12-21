#
# compiler selection
#
CC		= gcc
#CC		= cc

CFLAGS		= -O2		# for optimization
#CFLAGS		= -g -O2	# for debug

#
# For use of mathematical function
#
#MATH		= 
MATH		= -lm	

DEST		= .

EXTHDRS		=

HDRS		= param.h

INSTALL		= cp

LD		= $(CC)

LDFLAGS		=

LIBS		=

MAKEFILE	= Makefile

OBJS		= ran1.o \
		  dierfc.o \
		  main.o \
		  awgn.o  \
		  pathloss.o \
		  poidev.o \
		  dB2real.o \
		  channel.o \
		  count.o

PRINT		= pr

PROGRAM		= a.out

SHELL		= /bin/sh

SRCS		= ran1.c \
		  dierfc.c \
		  main.c \
		  awgn.c  \
		  pathloss.c \
		  poidev.c \
		  dB2real.c \
		  channel.c \
		  count.c 

SYSHDRS		= /usr/include/machine/_limits.h \
		/usr/include/machine/_types.h \
		/usr/include/math.h \
		/usr/include/stdio.h \
		/usr/include/stdlib.h \
		/usr/include/sys/_null.h \
		/usr/include/sys/_types.h \
		/usr/include/sys/cdefs.h \
		/usr/include/sys/timespec.h \
		/usr/include/time.h

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		@echo "Linking $(PROGRAM) ..."
		@$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM) $(MATH)
		@echo "done"

clean:;		@rm -f $(OBJS) core

clobber:;	@rm -f $(OBJS) $(PROGRAM) core tags

depend:;	@mkmf -f $(MAKEFILE)

echo:;		@echo $(HDRS) $(SRCS)

index:;		@ctags -wx $(HDRS) $(SRCS)

install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@-strip $(PROGRAM)
		@if [ $(DEST) != . ]; then \
		(rm -f $(DEST)/$(PROGRAM); $(INSTALL) $(PROGRAM) $(DEST)); fi

print:;		@$(PRINT) $(HDRS) $(SRCS)

tags:           $(HDRS) $(SRCS); @ctags $(HDRS) $(SRCS)
###
main.o:/usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h \
	/usr/include/time.h  param.h
ran1.o: /usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h \
	/usr/include/time.h  param.h
dB2real.o: /usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h  \
	/usr/include/time.h  param.h
dierfc.o:/usr/include/math.h
awgn.o:/usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h param.h
channel.o:/usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h param.h 
pathloss.o:/usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h param.h
poidev.o:/usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h param.h
count.c:/usr/include/stdio.h  \
	 /usr/include/stdlib.h \
	/usr/include/math.h param.h
