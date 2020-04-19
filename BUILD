package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

cc_binary(
    name = "ld46",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

pkg_tar(
    name = "ld46-linux",
    extension = "tar.gz",
    strip_prefix = "/",
    package_dir = "ld46/",
    srcs = [
        ":ld46",
        "//content",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "title_screen.cc",
        "maze_screen.cc",
    ],
    hdrs = [
        "title_screen.h",
        "maze_screen.h",
    ],
    deps = [
        "@libgam//:screen",
        "@libgam//:text",
        ":appearing_text",
        ":entities",
        ":maze",
        ":timer"
    ],
)

cc_library(
    name = "maze",
    srcs = [ "maze.cc" ],
    hdrs = [ "maze.h" ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
        "@libgam//:util",
    ],
)

cc_library(
    name = "timer",
    srcs = ["timer.cc"],
    hdrs = ["timer.h"],
)

cc_library(
    name = "entities",
    srcs = [
        "enemy.cc",
        "entity.cc",
        "flower.cc",
        "mouse.cc",
        "powerup.cc",
    ],
    hdrs = [
        "enemy.h",
        "entity.h",
        "flower.h",
        "mouse.h",
        "powerup.h",
    ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
        "@libgam//:util",
        ":maze",
        ":rect",
        ":timer",
    ],
)

cc_library(
    name = "rect",
    srcs = ["rect.cc"],
    hdrs = ["rect.h"],
);

cc_library(
    name = "appearing_text",
    srcs = ["appearing_text.cc"],
    hdrs = ["appearing_text.h"],
    deps = [
        "@libgam//:audio",
        "@libgam//:graphics",
        "@libgam//:text",
    ],
);


