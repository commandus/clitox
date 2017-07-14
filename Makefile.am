SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

if DEBUG
commoncppflags = -std=c++11 -g3 -O0
else
commoncppflags = -std=c++11 -O2
endif

#
#	Exclude generated sources from distribution
#
nodist_clitox_SOURCES =
BUILT_SOURCES =
CLEANFILES =

#
#	Binaries
#
bin_PROGRAMS=clitox
nobase_dist_include_HEADERS = \
	clitox.h clitox-config.h toxclient.h clientlist.h toxreceiver.h toxreceiverstream.h toxmessage.h

common_src = 

commonlibs = -L/usr/local/lib/ -largtable2

#
#	clitox
#
clitox_SOURCES= \
	clitox-config.cpp  clitox.cpp toxclient.cpp clientlist.cpp toxreceiverstream.cpp toxmessage.cpp \
	$(common_src)

clitox_LDADD = $(commonlibs) -lsodium -ltoxcore
clitox_CPPFLAGS = $(commoncppflags)

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	README.md HISTORY \
	.kdev_include_paths
