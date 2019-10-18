#! /bin/sh
# LIBTBX_DISPATCHER DO NOT EDIT
#
# THIS IS AN AUTOMATICALLY GENERATED FILE.
# DO NOT EDIT! CHANGES WILL BE LOST.
# To customize this auto-generated script create
#
#   dispatcher_include*.sh
#
# files in "/home/jdiaz/dials-dev20191018/build" and run
#
#   libtbx.refresh
#
# to re-generate the dispatchers (libtbx.refresh is a subset
# of the functionality of the libtbx/configure.py command).
#
# See also:
#   "/home/jdiaz/dials-dev20191018/build/dispatcher_include_template.sh"
#

# ----------------------------------------------------------------------------
# The shellrealpath function resolves an absolute physical path of its
# first argument and stores it in a global shell variable RESULT.
# The function returns nonzero for unreadable or invalid symlinks
# and resets the RESULT to an empty string.

shellrealpath() {
    local ORGDIR="$PWD"
    local TARGET="$1"
    RESULT=""
    # This test fails for a symlink loop.  We can do without resolution
    # of symlinks that point to existing unreadable files.
    [ -r "$TARGET" ] || return $?
    # Check if the readlink command exists.
    type readlink >/dev/null || return $?
    while true; do
        cd "$(dirname "$TARGET")"
        TARGET="$(basename "$TARGET")"
        if [ -L "$TARGET" ]; then
            TARGET="$(readlink "$TARGET")"
            continue
        fi
        RESULT="$(pwd -P)/$TARGET"
        break
    done
    cd "$ORGDIR"
}
# ----------------------------------------------------------------------------

unset PYTHONHOME
LC_ALL=en_US.UTF-8
export LC_ALL
LIBTBX_BUILD="$(shellrealpath "$0" && cd "$(dirname "$RESULT")/.." && pwd)"
export LIBTBX_BUILD
LIBTBX_PYEXE_BASENAME="python2.7"
export LIBTBX_PYEXE_BASENAME
LIBTBX_DISPATCHER_NAME="libtbx.sh"
export LIBTBX_DISPATCHER_NAME
# ---------------------------------------------------------------------------------------------
# included from /home/jdiaz/dials-dev20191018/build/dispatcher_include_dials.sh
if [ "$LIBTBX_DISPATCHER_NAME" != "libtbx.scons" ] &&    [ -z "$PHENIX_TRUST_OTHER_ENV" ]; then
  # work around broken library environments
  LD_LIBRARY_PATH=""
  PYTHONPATH=""
fi
# ---------------------------------------------------------------------------------------------
SSL_CERT_FILE="$LIBTBX_BUILD/../base/lib/python2.7/site-packages/certifi/cacert.pem"
export SSL_CERT_FILE
OPENBLAS_NUM_THREADS="1"
export OPENBLAS_NUM_THREADS
FONTCONFIG_PATH="$LIBTBX_BUILD/../base/etc/fonts"
export FONTCONFIG_PATH
if [ -n "$PYTHONPATH" ]; then
  PYTHONPATH="$LIBTBX_BUILD/../modules/cctbx_project:$LIBTBX_BUILD/../modules:$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib/site-python:$LIBTBX_BUILD/../base/lib/python2.7/site-packages:$PYTHONPATH"
  export PYTHONPATH
else
  PYTHONPATH="$LIBTBX_BUILD/../modules/cctbx_project:$LIBTBX_BUILD/../modules:$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib/site-python:$LIBTBX_BUILD/../base/lib/python2.7/site-packages"
  export PYTHONPATH
fi
if [ -n "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH="$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib64:$LIBTBX_BUILD/../base/lib:$LD_LIBRARY_PATH"
  export LD_LIBRARY_PATH
else
  LD_LIBRARY_PATH="$LIBTBX_BUILD/lib:$LIBTBX_BUILD/../base/lib64:$LIBTBX_BUILD/../base/lib"
  export LD_LIBRARY_PATH
fi
if [ -n "$PATH" ]; then
  PATH="$LIBTBX_BUILD/bin:$PATH"
  export PATH
else
  PATH="$LIBTBX_BUILD/bin"
  export PATH
fi
# ---------------------------------------------
# lines marked LIBTBX_PRE_DISPATCHER_INCLUDE_SH
# ---------------------------------------------
# --------------------------------------------------------------------------------------------------------------------
# included from /home/jdiaz/dials-dev20191018/build/dispatcher_include_dials.sh
export DIALS="/home/jdiaz/dials-dev20191018"
export DIALS_VERSION=dev20191018
export DIALS_ENVIRONMENT=1
export DIALS_MTYPE=intel-linux-2.6-x86_64
#
# GUI dependencies
CCTBX_BUILD_BASE="/home/jdiaz/dials-dev20191018/base"
LIBTBX_OS_NAME=`uname -s`
if [ "$PHENIX_GUI_ENVIRONMENT" = "1" ]; then
  if [ -z "$DISABLE_PHENIX_GUI" ]; then
    export BOOST_ADAPTBX_FPE_DEFAULT=1
    export BOOST_ADAPTBX_SIGNALS_DEFAULT=1
  fi
  # echo $LIBTBX_OS_NAME
  if [ "$LIBTBX_OS_NAME" = "Linux" ]; then
    export OLD_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
    export OLD_XDG_DATA_DIRS=$XDG_DATA_DIRS
    if [ -z "$LD_LIBRARY_PATH" ]; then
      LD_LIBRARY_PATH=/home/jdiaz/dials-dev20191018/base/lib:/home/jdiaz/dials-dev20191018/base/lib64
    else
      LD_LIBRARY_PATH=/home/jdiaz/dials-dev20191018/base/lib:/home/jdiaz/dials-dev20191018/base/lib64:$LD_LIBRARY_PATH
    fi
    export LD_LIBRARY_PATH
#
    unset GTK_MODULES
    GTK_PATH=$CCTBX_BUILD_BASE/lib/gtk-2.0/2.10.0
    PANGO_RC_FILE=$CCTBX_BUILD_BASE/etc/pango/pangorc
    GTK_IM_MODULE_FILE=$CCTBX_BUILD_BASE/etc/gtk-2.0/gtk.immodules
    GDK_PIXBUF_MODULE_FILE=$CCTBX_BUILD_BASE/etc/gtk-2.0/gdk-pixbuf.loaders
    GTK2_RC_FILES=$CCTBX_BUILD_BASE/share/themes/Clearlooks/gtk-2.0/gtkrc
    FONTCONFIG_PATH=$CCTBX_BUILD_BASE/etc/fonts
    FONTCONFIG_FILE=$CCTBX_BUILD_BASE/etc/fonts/fonts.conf
    if [ -z "$XDG_DATA_DIRS" ]; then
      XDG_DATA_DIRS=$CCTBX_BUILD_BASE/share:/usr/share
    else
      XDG_DATA_DIRS=$CCTBX_BUILD_BASE/share:$XDG_DATA_DIRS
    fi
    GNOME_DISABLE_CRASH_DIALOG=1
    export PANGO_RC_FILE
    export GTK_IM_MODULE_FILE
    export GDK_PIXBUF_MODULE_FILE
    export FONTCONFIG_PATH
    export FONTCONFIG_FILE
    export XDG_DATA_DIRS
    export GTK_PATH
    export GTK2_RC_FILES
    export GNOME_DISABLE_CRASH_DIALOG
    PATH=$CCTBX_BUILD_BASE/bin:$PATH
    export PATH
  fi
fi


if [ ! -z "$QB_PYTHONPATH" ]; then
  export PYTHONPATH=$PYTHONPATH:$QB_PYTHONPATH
fi
if [ ! -z "$QB_LD_LIBRARY_PATH" ]; then
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QB_LD_LIBRARY_PATH
fi
if [ ! -z "$QB_DYLD_LIBRARY_PATH" ]; then
  export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$QB_DYLD_LIBRARY_PATH
fi


# --------------------------------------------------------------------------------------------------------------------
# ----------------------------------------------
# lines marked LIBTBX_POST_DISPATCHER_INCLUDE_SH
# ----------------------------------------------
LIBTBX_PYEXE="$LIBTBX_BUILD/../base/bin/$LIBTBX_PYEXE_BASENAME"
export LIBTBX_PYEXE
if [ -n "$LIBTBX__VALGRIND_FLAG__" ]; then
  exec $LIBTBX_VALGRIND "$LIBTBX_BUILD/../modules/cctbx_project/libtbx/command_line/sh.sh" "$@"
elif [ $# -eq 0 ]; then
  exec "$LIBTBX_BUILD/../modules/cctbx_project/libtbx/command_line/sh.sh"
else
  exec "$LIBTBX_BUILD/../modules/cctbx_project/libtbx/command_line/sh.sh" "$@"
fi
