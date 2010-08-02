
MOD := mercmod
UTIL := ../util
PATCHER := ../patcher

CFLAGS := -Wall -O3 -fomit-frame-pointer -I$(PATCHER) -I. -I$(UTIL)

SOURCES := $(MOD).c d2.c d2_112a.c
OBJS := $(subst .c,.o,$(SOURCES)) d2_112a_asm.o

all: $(MOD).dll

$(MOD).dll: $(OBJS) $(PATCHER)/patcher.lib $(UTIL)/util.lib
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(MOD).dll *.o

-include $(addprefix .,$(subst .c,.d,$(SOURCES)))
.%.d: %.c
	$(CC) $(CFLAGS) -o $@ -MM $<
