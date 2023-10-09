set(rain-text-core-core-sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/rain_text_core.cpp
)

set(rain-text-core-utils-sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/rain_text_core_utils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/cipher/aes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/cipher/chacha20.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/cipher/twofish.cpp
)

set(rain-text-core-sources
        ${rain-text-core-core-sources}
        ${rain-text-core-utils-sources}
)