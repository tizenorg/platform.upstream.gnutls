# Copyright (C) 2002-2014 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <http://www.gnu.org/licenses/>.
#
# As a special exception to the GNU General Public License,
# this file may be distributed as part of a program that
# contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the specification of how gnulib-tool is used.
# It acts as a cache: It is written and read by gnulib-tool.
# In projects that use version control, this file is meant to be put under
# version control, like the configure.ac and various Makefile.am files.


# Specification in the form of a command-line invocation:
#   gnulib-tool --import --dir=. --local-dir=gl/override --lib=libgnu --source-base=gl --m4-base=gl/m4 --doc-base=doc --tests-base=gl/tests --aux-dir=build-aux --with-tests --avoid=alignof-tests --avoid=lock-tests --avoid=lseek-tests --lgpl=2 --no-conditional-dependencies --libtool --macro-prefix=gl --no-vc-files alloca base64 byteswap c-ctype extensions func gendocs getline gettext gettimeofday hash-pjw-bare havelib iconv intprops lib-msvc-compat lib-symbol-versions maintainer-makefile manywarnings memmem-simple minmax netdb netinet_in pmccabe2html read-file snprintf stdint strcase strndup strtok_r strverscmp sys_socket sys_stat time_r u64 unistd valgrind-tests vasprintf vsnprintf warnings

# Specification in the form of a few gnulib-tool.m4 macro invocations:
gl_LOCAL_DIR([gl/override])
gl_MODULES([
  alloca
  base64
  byteswap
  c-ctype
  extensions
  func
  gendocs
  getline
  gettext
  gettimeofday
  hash-pjw-bare
  havelib
  iconv
  intprops
  lib-msvc-compat
  lib-symbol-versions
  maintainer-makefile
  manywarnings
  memmem-simple
  minmax
  netdb
  netinet_in
  pmccabe2html
  read-file
  snprintf
  stdint
  strcase
  strndup
  strtok_r
  strverscmp
  sys_socket
  sys_stat
  time_r
  u64
  unistd
  valgrind-tests
  vasprintf
  vsnprintf
  warnings
])
gl_AVOID([alignof-tests lock-tests lseek-tests])
gl_SOURCE_BASE([gl])
gl_M4_BASE([gl/m4])
gl_PO_BASE([])
gl_DOC_BASE([doc])
gl_TESTS_BASE([gl/tests])
gl_WITH_TESTS
gl_LIB([libgnu])
gl_LGPL([2])
gl_MAKEFILE_NAME([])
gl_LIBTOOL
gl_MACRO_PREFIX([gl])
gl_PO_DOMAIN([])
gl_WITNESS_C_MACRO([])
gl_VC_FILES([false])
