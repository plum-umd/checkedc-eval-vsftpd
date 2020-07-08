CCONV=~/checkedc-clang/build/bin/cconv-standalone
INCLUDES=~/checkedc/include
BASEDIR=.

# -dump-stats \
# -dump-intermediate \

CPATH=$INCLUDES $CCONV \
-output-postfix=checked \
-extra-arg-before=-D_FORTIFY_SOURCE=2 \
-extra-arg-before=-w \
-base-dir="$BASEDIR" \
$BASEDIR/ftpdataio.c \
$BASEDIR/ascii.c \
$BASEDIR/str.c \
$BASEDIR/seccompsandbox.c \
$BASEDIR/utility.c \
$BASEDIR/twoprocess.c \
$BASEDIR/ftpcmdio.c \
$BASEDIR/filestr.c \
$BASEDIR/postprivparent.c \
$BASEDIR/access.c \
$BASEDIR/netstr.c \
$BASEDIR/strlist.c \
$BASEDIR/ptracesandbox.c \
$BASEDIR/privsock.c \
$BASEDIR/ssl.c \
$BASEDIR/sysutil.c \
$BASEDIR/prelogin.c \
$BASEDIR/opts.c \
$BASEDIR/logging.c \
$BASEDIR/readwrite.c \
$BASEDIR/banner.c \
$BASEDIR/hash.c \
$BASEDIR/privops.c \
$BASEDIR/ls.c \
$BASEDIR/tunables.c \
$BASEDIR/postlogin.c \
$BASEDIR/secutil.c \
$BASEDIR/secbuf.c \
$BASEDIR/sslslave.c \
$BASEDIR/ipaddrparse.c \
$BASEDIR/oneprocess.c \
$BASEDIR/features.c \
$BASEDIR/standalone.c \
$BASEDIR/sysstr.c \
$BASEDIR/ftppolicy.c \
$BASEDIR/main.c \
$BASEDIR/parseconf.c \
$BASEDIR/sysdeputil.c \
$BASEDIR/tcpwrap.c
