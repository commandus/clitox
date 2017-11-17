SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

if DEBUG
commoncppflags = -std=c++11 -g3 -O0 -I/usr/local/include
else
commoncppflags = -std=c++11 -O2 -I/usr/local/include
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
	clitox.h clitox-config.h toxclient.h clientlist.h toxreceiver.h toxreceiverstream.h toxmessage.h \
	rapidjson/allocators.h           rapidjson/encodings.h        rapidjson/fwd.h             rapidjson/memorystream.h    rapidjson/prettywriter.h  rapidjson/schema.h        rapidjson/writer.h \
	rapidjson/document.h             rapidjson/filereadstream.h   rapidjson/istreamwrapper.h  rapidjson/ostreamwrapper.h  rapidjson/rapidjson.h     rapidjson/stream.h \
	rapidjson/encodedstream.h        rapidjson/filewritestream.h  rapidjson/memorybuffer.h    rapidjson/pointer.h         rapidjson/reader.h        rapidjson/stringbuffer.h \
	rapidjson/error/en.h             rapidjson/error/error.h \
	rapidjson/internal/biginteger.h  rapidjson/internal/dtoa.h rapidjson/internal/itoa.h rapidjson/internal/pow10.h rapidjson/internal/stack.h rapidjson/internal/strtod.h \
	rapidjson/internal/diyfp.h       rapidjson/internal/ieee754.h rapidjson/internal/meta.h rapidjson/internal/regex.h rapidjson/internal/strfunc.h rapidjson/internal/swap.h \
	rapidjson/msinttypes/inttypes.h  rapidjson/msinttypes/stdint.h \
	intent/variablecolumn.h          intent/intenttemplate.h
	toxnodejson.h

common_src = 

commonlibs = -L/usr/local/lib/ -largtable2

#
#	clitox
#
clitox_SOURCES= \
	clitox-config.cpp  clitox.cpp toxclient.cpp clientlist.cpp toxreceiverstream.cpp toxmessage.cpp \
	intent/variablecolumn.cpp \
	toxnodejson.cpp intent/intenttemplate.cpp \
	$(common_src)

clitox_LDADD = $(commonlibs) -lsodium -ltoxcore
clitox_CPPFLAGS = $(commoncppflags)

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	README.md HISTORY \
	autogen.sh \
	.kdev_include_paths
