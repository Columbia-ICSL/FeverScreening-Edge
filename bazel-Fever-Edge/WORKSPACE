load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# gtest
git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    branch = "v1.10.x",
)

# gflags
http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

# glogs
http_archive(
    name = "com_github_google_glog",
    sha256 = "62efeb57ff70db9ea2129a16d0f908941e355d09d6d83c9f7b18557c0a7ab59e",
    strip_prefix = "glog-d516278b1cd33cd148e8989aec488b6049a4ca0b",
    urls = ["https://github.com/google/glog/archive/d516278b1cd33cd148e8989aec488b6049a4ca0b.zip"],
)

# libusb
http_archive(
    name = "libusb",
    urls = ["https://github.com/libusb/libusb/archive/v1.0.24.tar.gz"],
    strip_prefix = "libusb-1.0.24",
    sha256 = "b7724c272dfc5713dce88ff717efd60f021ca5b7c8e30f08ebb2c42d2eea08ae",
    build_file = "@//third_party:libusb.BUILD",
)

# hiredis for c
http_archive(
    name = "libredis",
    urls = ["https://github.com/redis/hiredis/archive/v1.0.0.tar.gz"],
    strip_prefix = "hiredis-1.0.0",
    sha256 = "2a0b5fe5119ec973a0c1966bfc4bd7ed39dbce1cb6d749064af9121fe971936f",
    build_file = "@//third_party:libredis.BUILD",
)

# nlohmann json
http_archive(
    name = "libjson",
    urls = ["https://github.com/nlohmann/json/archive/v3.9.1.tar.gz"],
    strip_prefix = "json-3.9.1",
    sha256 = "4cf0df69731494668bdd6460ed8cb269b68de9c19ad8c27abc24cd72605b2d5b",
    build_file = "@//third_party:libjson.BUILD",
)

# cryptopp
http_archive(
    name = "libcryptopp",
    urls = ["https://github.com/weidai11/cryptopp/archive/CRYPTOPP_8_2_0.tar.gz"],
    strip_prefix = "cryptopp-CRYPTOPP_8_2_0",
    sha256 = "e3bcd48a62739ad179ad8064b523346abb53767bcbefc01fe37303412292343e",
    build_file = "@//third_party:libcryptopp.BUILD",
)

# libevent
http_archive(
    name = "libevent",
    urls = ["https://github.com/libevent/libevent/releases/download/release-2.1.12-stable/libevent-2.1.12-stable.tar.gz"],
    strip_prefix = "libevent-2.1.12-stable",
    sha256 = "92e6de1be9ec176428fd2367677e61ceffc2ee1cb119035037a27d346b0403bb",
    build_file = "@//third_party:libevent.BUILD",
)
