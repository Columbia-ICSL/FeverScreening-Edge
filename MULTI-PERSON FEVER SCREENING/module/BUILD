package(
    default_visibility = ["//visibility:public"],
)

cc_binary(
    name = "flirone_over_redis",
    srcs = [
      "flirone_over_redis.cc"],
    deps = [
      "//device:flirone_interface",
      "@libcryptopp//:libcryptopp",
      "@libredis//:libredis",
      "@libusb//:libusb",
      "@com_github_google_glog//:glog",
    ],
    linkopts = [
      "-ludev",
    ],
    copts = [
      "-std=c++14",
    ]
)

cc_binary(
    name = "flirone_over_post",
    srcs = [
      "flirone_over_post.cc"],
    deps = [
      "//common:config_utils",
      "//device:flirone_interface",
      "@libcryptopp//:libcryptopp",
      "@libredis//:libredis",
      "@libusb//:libusb",
      "@com_github_google_glog//:glog",
    ],
    linkopts = [
      "-ludev",
      "-lcurl",
    ],
    copts = [
      "-std=c++14",
      "-O3",
    ]
)

cc_binary(
    name = "flirone_transponder",
    srcs = [
      "flirone_transponder.cc"],
    deps = [
      "//common:base64",
      "//common:config_utils",
      "//device:flirone_datatypes",
      "//device:http_post_server_interface",
      "@libcryptopp//:libcryptopp",
      "@libevent//:libevent",
      "@libjson//:libjson",
      "@libredis//:libredis",
      "@com_github_google_glog//:glog",
    ],
    copts = [
      "-std=c++14",
      "-O3",
    ]
)

cc_binary(
    name = "flirone_to_jpg",
    srcs = [
      "flirone_to_jpg.cc"],
    deps = [
      "//device:flirone_interface",
      "@libusb//:libusb",
      "@com_github_google_glog//:glog",
    ],
    linkopts = [
      "-ludev",
    ],
    copts = [
      "-std=c++14",
    ]
)