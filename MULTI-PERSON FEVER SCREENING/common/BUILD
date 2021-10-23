load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "status",
    srcs = ["status.h"],
)

cc_library(
    name = "time",
    srcs = ["time.cc"],
    hdrs = ["time.h"],
)

cc_library(
    name = "base64",
    srcs = ["base64.cc"],
    hdrs = ["base64.h"],
)

cc_library(
    name = "encryption_utils",
    srcs = ["encryption_utils.cc"],
    hdrs = ["encryption_utils.h"],
    deps = [
      "@libcryptopp//:libcryptopp",
      "@com_github_google_glog//:glog",
    ]
)

cc_library(
    name = "config_utils",
    srcs = ["config_utils.cc"],
    hdrs = ["config_utils.h"],
    deps = [
      "@libjson//:libjson",
    ],
)