LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CPPFLAGS := 
LOCAL_CFLAGS := -DEVHTP_SYS_ARCH=32

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/include/libevent \
	$(LOCAL_PATH)/include/libevhtp

LOCAL_SRC_FILES:= \
	libevent/buffer.c \
	libevent/bufferevent.c \
	libevent/bufferevent_filter.c \
	libevent/bufferevent_pair.c \
	libevent/bufferevent_ratelim.c \
	libevent/bufferevent_sock.c \
	libevent/devpoll.c \
	libevent/epoll.c \
	libevent/event.c \
	libevent/event_tagging.c \
	libevent/evmap.c \
	libevent/evport.c \
	libevent/evthread.c \
	libevent/evthread_pthread.c \
	libevent/evutil.c \
	libevent/evutil_rand.c \
	libevent/evutil_time.c \
	libevent/listener.c \
	libevent/log.c \
	libevent/poll.c \
	libevent/select.c \
	libevent/signal.c \
	libevent/strlcpy.c \
	libevhtp/evhtp.c \
	libevhtp/evhtp_numtoa.c \
	libevhtp/evthr.c \
	libevhtp/htparse.c \
	webif.cpp
	
LOCAL_MODULE:= webif
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
