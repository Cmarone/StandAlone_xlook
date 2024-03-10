xlook_SOURCES = \
	saxl.c \
        array.c \
        cmds.c \
        cmds1.c \
        event.c \
        filtersm.c \
        fq.c \
        func.c \
        look_funcs.c \
        lookio.c \
        median.c \
        mem.c \
        messages.c \
        nrutil.c \
        perrfl.c \
        polyfit.c \
        qi_look.c \
        sort.c \
        special.c \
        strcmd.c \

xlook_HEADS = \
        array.h \
        cmds.h \
        cmds1.h \
        event.h \
        filtersm.h \
        fq.h \
        func.h \
        global.h \
        look_funcs.h \
        lookio.h \
        median.h \
        mem.h \
        messages.h \
        nr.h \
        nrutil.h \
        perrfl.h \
        polyfit.h \
        qi_look.h \
        sort.h \
        special.h \
        strcmd.h \


LIBS = -lm

CFLAGS =  -I/opt/local/bin -DOWTOOLKIT_WARNING_DISABLED


saxl: $(xlook_SOURCES)
	gcc -o saxl $(xlook_SOURCES) $(CFLAGS) $(LIBS)


