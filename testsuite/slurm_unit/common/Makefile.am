AUTOMAKE_OPTIONS = foreign

SUBDIRS = slurm_protocol_pack slurmdb_pack

AM_CPPFLAGS = -I$(top_srcdir) -ldl -lpthread
LDADD = $(top_builddir)/src/api/libslurm.o $(DL_LIBS)

check_PROGRAMS = \
	$(TESTS)

TESTS = \
	bitstring-test \
	job-resources-test \
	log-test \
	pack-test
