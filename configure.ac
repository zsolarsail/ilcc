
AC_PREREQ([2.63])
AC_INIT([olcc],[0.1-0])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB


AC_LANG(C++)
AC_MSG_CHECKING([for g++11 without opt])
AC_RUN_IFELSE( [AC_LANG_PROGRAM([[#include <stdlib.h>]],
                    [[ int b[16]; for(auto &x: b)x=0; ]])],
                    [
                        AC_MSG_RESULT([yes])
                        M11=1
                    ],
                    [
                        AC_MSG_RESULT([no])
                    ]
)



if [[ ! -z "$M11" ]]
then
    CXXFL="-Wall -Wextra -pthread"
else
    CXXFL="-Wall -Wextra -std=c++11 -pthread"
fi

AC_SUBST([AM_CXXFLAGS], $CXXFL)

AC_CONFIG_FILES(
    Makefile
    src/Makefile
    )

AC_OUTPUT
