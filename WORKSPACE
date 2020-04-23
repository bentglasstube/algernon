load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "libgam",
    remote = "https://git.sr.ht/~bentglasstube/gam",
    commit = "b0defdf5ce783d16e851d6b6f58eac5932fa192f",
)

git_repository(
    name = "mxebzl",
    remote = "https://github.com/cfrantz/mxebzl.git",
    tag = "20191215_RC01",
)

load("@mxebzl//compiler:repository.bzl", "mxe_compiler")

mxe_compiler(
    deps = [
        "compiler",
        "SDL2",
        "SDL2-extras",
        "xz",
        "pthreads",
    ],
)
