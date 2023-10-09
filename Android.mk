LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libcryptopp
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)cryptopp-CRYPTOPP_8_6_0/*.cpp)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/cryptopp-CRYPTOPP_8_6_0/
LOCAL_LDLIBS    := -llog

include $(BUILD_SHARED_LIBRARY)
