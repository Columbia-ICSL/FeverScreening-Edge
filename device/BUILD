load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "flirone_datatypes",
    srcs = ["flirone_datatypes.cc"],
    hdrs = ["flirone_datatypes.h"],
    deps = [
      ":flirone_calibration_utils",
      "//common:base64",
      "//common:encryption_utils",
      "@libjson//:libjson",
      "@com_github_google_glog//:glog",
    ]
)

cc_library(
    name = "flirone_interface",
    srcs = ["flirone_interface.cc"],
    hdrs = ["flirone_interface.h"],
    deps = [
      ":flirone_datatypes",
      "//common:status",
      "//common:time",
      "@com_github_google_glog//:glog",
      "@libusb//:libusb",
    ],
    copts = [
      "-fopenmp",
    ]
)

cc_library(
    name = "http_post_server_interface",
    srcs = ["http_post_server_interface.cc"],
    hdrs = ["http_post_server_interface.h"],
    deps = [
      "@libevent//:libevent",
      "@com_github_google_glog//:glog",
      "@libusb//:libusb",
    ]
)

cc_library(
    name = "flirone_calibration_utils",
    srcs = ["flirone_calibration_utils.cc"],
    hdrs = ["flirone_calibration_utils.h"],
    deps = [
      "@com_github_google_glog//:glog",
    ],
    copts = [
      "-fopenmp",
    ]
)