package(
    default_visibility = ["//visibility:public"],
)

cc_test(
    name = "test_flirone_interface",
    srcs = ["test_flirone_interface.cc"],
    deps = [
      "//device:flirone_interface",
      "@gtest//:gtest",
      "@gtest//:gtest_main",
      "@libusb//:libusb",
    ],
    copts = [
      "-fopenmp",
    ]
)
