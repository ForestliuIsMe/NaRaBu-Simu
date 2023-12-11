load("@rules_cc//cc:defs.bzl","cc_binary","cc_library")

cc_library(
    name = "cache_if",
    hdrs = ["cache_if.h"],
    deps = [
        "//platform/core/module_if:module_if"
    ],
    visibility = ["//visibility:public"]
)

cc_library(
    name = "fully_asso_cache",
    srcs = [
        "fully_asso_cache.cc",
    ],
    hdrs = [
        "fully_asso_cache.h",
    ],
    deps = [
        ":cache_if",
    ],
    visibility = ["//visibility:public"]
)

cc_binary(
    name = "fully_asso_cache_test",
    srcs = [
        "fully_asso_cache.cc",
        "fully_asso_cache.h",
    ],
    deps = [
        ":cache_if",
    ],
    visibility = ["//visibility:public"]
)

