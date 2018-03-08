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
	rilproxy \
	oom-msg-logger \
	api-daemon \
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
PRODUCT_COPY_FILES += \
        gonk-misc/fota_certs/bcab1d5d.0:system/etc/certs/bcab1d5d.0 \
        gonk-misc/fota_certs/bb3ec629.0:system/etc/certs/bb3ec629.0 \
        gonk-misc/fota_certs/219d9499.0:system/etc/certs/219d9499.0 \
        gonk-misc/fota_certs/bc3f2570.0:system/etc/certs/bc3f2570.0
