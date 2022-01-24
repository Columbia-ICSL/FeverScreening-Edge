load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
package(
    default_visibility = ["//visibility:public"],
)

filegroup(
    name = "libevent_fg",
    srcs = [
      "install/include/event2/buffer.h",
      "install/include/event2/buffer_compat.h",
      "install/include/event2/bufferevent.h",
      "install/include/event2/bufferevent_compat.h",
      "install/include/event2/bufferevent_struct.h",
      "install/include/event2/dns.h",
      "install/include/event2/dns_compat.h",
      "install/include/event2/dns_struct.h",
      "install/include/event2/event-config.h",
      "install/include/event2/event.h",
      "install/include/event2/event_compat.h",
      "install/include/event2/event_struct.h",
      "install/include/event2/http.h",
      "install/include/event2/http_compat.h",
      "install/include/event2/http_struct.h",
      "install/include/event2/keyvalq_struct.h",
      "install/include/event2/listener.h",
      "install/include/event2/rpc.h",
      "install/include/event2/rpc_compat.h",
      "install/include/event2/rpc_struct.h",
      "install/include/event2/tag.h",
      "install/include/event2/tag_compat.h",
      "install/include/event2/thread.h",
      "install/include/event2/util.h",
      "install/include/event2/visibility.h",
    ],
)

cc_library(
    name = "libevent",
    srcs = [
      ":libevent_genrule",
      ],
    hdrs = [
      ":libevent_fg",
    ],
    linkstatic = 1,
    strip_include_prefix = "install/include",
)

genrule(
    name = "libevent_genrule",
    srcs = glob(
      ["**",],
      exclude = [
      "**/msvc/**",
      "**/Xcode/**",
    ]),
    outs = [
      "install/include/event2/buffer.h",
      "install/include/event2/buffer_compat.h",
      "install/include/event2/bufferevent.h",
      "install/include/event2/bufferevent_compat.h",
      "install/include/event2/bufferevent_struct.h",
      "install/include/event2/dns.h",
      "install/include/event2/dns_compat.h",
      "install/include/event2/dns_struct.h",
      "install/include/event2/event-config.h",
      "install/include/event2/event.h",
      "install/include/event2/event_compat.h",
      "install/include/event2/event_struct.h",
      "install/include/event2/http.h",
      "install/include/event2/http_compat.h",
      "install/include/event2/http_struct.h",
      "install/include/event2/keyvalq_struct.h",
      "install/include/event2/listener.h",
      "install/include/event2/rpc.h",
      "install/include/event2/rpc_compat.h",
      "install/include/event2/rpc_struct.h",
      "install/include/event2/tag.h",
      "install/include/event2/tag_compat.h",
      "install/include/event2/thread.h",
      "install/include/event2/util.h",
      "install/include/event2/visibility.h",
      "install/lib/libevent-2.1.so.7",
      "install/lib/libevent-2.1.so.7.0.1",
      "install/lib/libevent.a",
      "install/lib/libevent_core-2.1.so.7",
      "install/lib/libevent_core-2.1.so.7.0.1",
      "install/lib/libevent_core.a",
      "install/lib/libevent_core.la",
      "install/lib/libevent_core.so",
      "install/lib/libevent_extra-2.1.so.7",
      "install/lib/libevent_extra-2.1.so.7.0.1",
      "install/lib/libevent_extra.a",
      "install/lib/libevent_extra.la",
      "install/lib/libevent_extra.so",
      "install/lib/libevent.la",
      "install/lib/libevent_pthreads-2.1.so.7",
      "install/lib/libevent_pthreads-2.1.so.7.0.1",
      "install/lib/libevent_pthreads.a",
      "install/lib/libevent_pthreads.la",
      "install/lib/libevent_pthreads.so",
      "install/lib/libevent.so",
    ],
    cmd = "tree . &&\
        cd external/libevent/ && \
        mkdir -p ../../$(GENDIR)/external/libevent/install && \
        ./configure --prefix=`pwd`/../../$(GENDIR)/external/libevent/install --disable-openssl && \
        make -j8 && \
        make install && \
        tree ../../",
)
