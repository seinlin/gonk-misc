TARGET_PROVIDES_INIT_RC ?= true
CONFIG_ESD := no
HTTP := android

MAJOR_VERSION := $(word 1,$(subst ., ,$(PLATFORM_VERSION)))

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
	$(NULL)

ifneq ($(filter-out 0 1 2 3 4,$(MAJOR_VERSION)),)
# the below kaios_sepolicy.mk is a kaios sepolicy enabler
-include external/sepolicy/kaios_sepolicy.mk
ifeq ($(KAIOS_SEPOLICY_ENABLED),true)
BOARD_SEPOLICY_DIRS += \
	gonk-misc/sepolicy
endif
endif

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
