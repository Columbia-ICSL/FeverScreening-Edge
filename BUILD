load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

package(default_visibility = ["//visibility:public"])

filegroup(
  name = "fever_config_fg",
  srcs = [
    "config/launch.json",
  ],
)

pkg_tar(
  name = "flir_transponder_bin_pkg",
  srcs = [
    "//module:flirone_transponder"],
  include_runfiles = 1,
  mode = "0755",
  package_dir = "/bin",
  tags = ["tar"],
)

pkg_tar(
  name = "flir_transponder_pkg",
  srcs = [
    ":fever_config_fg"],
  include_runfiles = 1,
  mode = "0755",
  tags = ["tar"],
  deps = [
    ":flir_transponder_bin_pkg",
  ]
)

pkg_tar(
  name = "flir_client_x86_bin_pkg",
  srcs = [
    "//module:flirone_over_post"],
  include_runfiles = 1,
  mode = "0755",
  package_dir = "/bin",
  tags = ["tar"],
)

pkg_tar(
  name = "flir_client_x86_pkg",
  srcs = [
    ":fever_config_fg"],
  include_runfiles = 1,
  mode = "0755",
  tags = ["tar"],
  deps = [
    ":flir_client_x86_bin_pkg",
  ]
)
pkg_tar(
  name = "flir_client_aarch64_bin_pkg",
  srcs = [
    "//module:flirone_over_post"],
  include_runfiles = 1,
  mode = "0755",
  package_dir = "/bin",
  tags = ["tar"],
)

pkg_tar(
  name = "flir_client_aarch64_pkg",
  srcs = [
    ":fever_config_fg"],
  include_runfiles = 1,
  mode = "0755",
  tags = ["tar"],
  deps = [
    ":flir_client_aarch64_bin_pkg",
  ]
)
