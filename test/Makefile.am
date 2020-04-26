
COMMON_TEST_CFLAGS = -I $(top_srcdir)/include \
                     -I $(top_srcdir)/lib \
					 $(JSONC_CFLAGS) \
					 -DTCM_TEST_SRC=\"$(abs_top_srcdir)/test/\" \
                     -fsanitize=address \
					 -fno-omit-frame-pointer

COMMON_TEST_LDADD = libnala.la \
					$(top_builddir)/lib/libtcm.la \
					$(JSONC_LIBS)

check_LTLIBRARIES = libnala.la

libnala_la_SOURCES = nala.c

check_PROGRAMS = test1

test1_SOURCES = test1.c
test1_LDADD = $(COMMON_TEST_LDADD)
test1_CFLAGS = $(COMMON_TEST_CFLAGS)

TESTS = test1
