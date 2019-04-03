TARGET_PROVIDES_INIT_RC ?= true
CONFIG_ESD := no
HTTP := android

PRODUCT_PACKAGES += \
	b2g.sh \
	b2g-info \
	b2g-prlimit \
	b2g-ps \
	bluetoothd \
	gonksched \
	init.bluetooth.rc \
	fakeappops \
	fs_config \
	gaia \
	gecko \
	init.rc \
	init.b2g.rc \
	killer \
	libttspico \
	rild \
	oom-msg-logger \
	api-daemon \
	api-daemon.sh \
	updater-daemon \
	updater-daemon.sh \
	ads-sdk \
	$(NULL)

-include external/svox/pico/lang/all_pico_languages.mk
-include gaia/gaia.mk

$(call inherit-product-if-exists, system/mmitest/mmitest.mk)

ifeq ($(B2G_VALGRIND),1)
include external/valgrind/valgrind.mk
endif

ifeq ($(ENABLE_DEFAULT_BOOTANIMATION),true)
PRODUCT_COPY_FILES += \
	gonk-misc/bootanimation.zip:system/media/bootanimation.zip
endif

ifeq ($(ENABLE_LIBRECOVERY),true)
PRODUCT_PACKAGES += \
  librecovery
endif

ifneq ($(DISABLE_SOURCES_XML),true)
PRODUCT_PACKAGES += \
	sources.xml
endif

ifeq ($(ENABLE_RADVD),true)
PRODUCT_PACKAGES += \
	radvd
endif

ifneq ($(TARGET_PROVIDES_INIT_RILPROXY_RC),true)
PRODUCT_PACKAGES += \
    init.rilproxy.rc \
    rilproxy
endif
