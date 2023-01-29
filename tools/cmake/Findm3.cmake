include(FetchContent)

FETCHCONTENT_DECLARE(m3 GIT_REPOSITORY https://github.com/wasm3/wasm3.git GIT_TAG main)
FETCHCONTENT_MAKEAVAILABLE(m3)
INCLUDE_DIRECTORIES(${m3_SOURCE_DIR}/source)