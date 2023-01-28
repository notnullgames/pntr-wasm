include(FetchContent)

FETCHCONTENT_DECLARE(physfs GIT_REPOSITORY https://github.com/icculus/physfs.git GIT_TAG main)
FETCHCONTENT_MAKEAVAILABLE(physfs)
INCLUDE_DIRECTORIES(${physfs_SOURCE_DIR}/src)