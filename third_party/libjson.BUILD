load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "libjson",
    srcs = [
      ],
    hdrs = [
      "single_include/nlohmann/json.hpp",
    ],
    strip_include_prefix = "single_include",
    linkstatic = 1,
)
