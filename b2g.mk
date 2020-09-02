PRODUCT_PACKAGES += \
	ads-sdk \
	b2g-info \
	b2g-ps \
	b2g-procrank \
	b2g.sh \
	b2gkillerd \
	fakeappops \
	fakeSensorPrivacy \
	fakePackageManagerNative \
	gonkProcessInfo \
	gonksched \
	init.b2g.rc \
	init.bluetooth.rc \
	libcarthage \
	gecko \
	api-daemon \
	api-daemon.sh \
	updater-daemon \
	updater-daemon.sh \
	bluetoothd \
	librecovery \
	gaia \
	sources.xml \
	$(NULL)


# Dhcpcd
DHCPCD_USE_SCRIPT := yes

PRODUCT_PACKAGES += \
    dhcpcd-6.8.2 \
    dhcpcd.conf \
    dhcpcd-run-hooks \
    20-dns.conf \
    95-configured

ifeq ($(ENABLE_RADVD),true)
PRODUCT_PACKAGES += \
        radvd
endif

ifeq ($(ENABLE_DEFAULT_BOOTANIMATION),true)
PRODUCT_COPY_FILES += \
        gonk-misc/bootanimation.zip:system/media/bootanimation.zip \
        gonk-misc/poweron-sound.ogg:system/media/poweron-sound.ogg
endif

# Enabled build librecovery.
ENABLE_LIBRECOVERY := true

# IME dictionaries from TouchPal
ifeq ($(IME_ENGINE), touchpal)
-include external/libtouchpal/touchpal.mk
endif

$(call inherit-product-if-exists, gaia/gaia.mk)
$(call inherit-product-if-exists, external/svox/pico/lang/all_pico_languages.mk)
