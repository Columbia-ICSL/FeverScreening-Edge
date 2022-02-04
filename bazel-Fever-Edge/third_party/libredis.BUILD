load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "libredis",
    srcs = [
      ":libredis_genrule",
      ],
    hdrs = [
      "include/hiredis/adapters/ae.h",
      "include/hiredis/adapters/glib.h",
      "include/hiredis/adapters/ivykis.h",
      "include/hiredis/adapters/libevent.h",
      "include/hiredis/adapters/libev.h",
      "include/hiredis/adapters/libuv.h",
      "include/hiredis/adapters/macosx.h",
      "include/hiredis/adapters/qt.h",
      "include/hiredis/alloc.h",
      "include/hiredis/async.h",
      "include/hiredis/hiredis.h",
      "include/hiredis/read.h",
      "include/hiredis/sds.h",
    ],
    strip_include_prefix = "include",
    linkstatic = 1,
)

genrule(
    name = "libredis_genrule",
    srcs = glob(
      ["**",]),
    outs = [
      "lib/libhiredis.a",
      "lib/libhiredis.so",
      "lib/libhiredis.so.1.0.0",
      "include/hiredis/adapters/ae.h",
      "include/hiredis/adapters/glib.h",
      "include/hiredis/adapters/ivykis.h",
      "include/hiredis/adapters/libevent.h",
      "include/hiredis/adapters/libev.h",
      "include/hiredis/adapters/libuv.h",
      "include/hiredis/adapters/macosx.h",
      "include/hiredis/adapters/qt.h",
      "include/hiredis/alloc.h",
      "include/hiredis/async.h",
      "include/hiredis/hiredis.h",
      "include/hiredis/read.h",
      "include/hiredis/sds.h",
    ],
    cmd = "tree . &&\
        cd external/libredis/ && \
        mkdir install && \
        make -sj && \
        make install DESTDIR=`pwd`/install && \
        mkdir -p ../../$(GENDIR)/external/libredis/ && \
        cp -r ./install/usr/local/* ../../$(GENDIR)/external/libredis"
)
