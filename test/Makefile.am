
COMMON_TEST_CFLAGS = -I $(top_srcdir)/include \
                     -I $(top_srcdir)/lib \
                     -fsanitize=address

check_LTLIBRARIES = libnala.la

libnala_la_SOURCES = nala.c

check_PROGRAMS = test1

test1_SOURCES = test1.c
test1_LDADD = libnala.la $(top_builddir)/lib/libstateblaster.la
test1_CFLAGS = $(COMMON_TEST_CFLAGS)

TESTS = test1
