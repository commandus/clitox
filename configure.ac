AC_PREREQ([2.63])
AC_INIT([pkt2mon], [0.1], [andrey.ivanov@ikfia.ysn.ru])
AM_INIT_AUTOMAKE([1.11 foreign no-define subdir-objects])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_SRCDIR([pkt2mon.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
[case "${enableval}" in
        yes) debug=true ;;
        no)  debug=false ;;
        *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

# Checks for libraries.
AC_CHECK_LIB(glfw, glfwInit, [], AC_MSG_ERROR([Could not find libglfw. Try $ ./configure LDFLAGS='-Lyour-glfw-lib-path']))
AC_CHECK_LIB(GL, glPushMatrix, [], AC_MSG_ERROR([Could not find libGL. Try $ ./configure LDFLAGS='-Lyour-GL-lib-path']))

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stdint.h string.h sys/time.h termios.h unistd.h])
AC_CHECK_HEADERS([argtable2.h], [], AC_MSG_ERROR([Could not find argtable2.h. Try $ ./configure CFLAGS='-Iyour-argtable2-include-path]))
AC_CHECK_HEADERS([GLFW/glfw3.h], [], AC_MSG_ERROR([Could not find GLFW/glfw3.h. Try $ ./configure CFLAGS='-Iyour-GLFW-include-path]))

# Checks for typedefs, structures, and compiler characteristics.
# AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([memmove memset])

AC_OUTPUT
