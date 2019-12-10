PRODUCT_PACKAGES += \
	b2g-info \
	b2g.sh \
	init.b2g.rc \
	libcarthage \
	gecko \
	api-daemon \
	api-daemon.sh \
	updater-daemon \
	updater-daemon.sh \
	$(NULL)


# Dhcpcd
DHCPCD_USE_SCRIPT := yes

PRODUCT_PACKAGES += \
    dhcpcd-6.8.2 \
    dhcpcd.conf \
    dhcpcd-run-hooks \
    20-dns.conf \
    95-configured


ifeq ($(ENABLE_DEFAULT_BOOTANIMATION),true)
PRODUCT_COPY_FILES += \
        gonk-misc/bootanimation.zip:system/media/bootanimation.zip \
        gonk-misc/poweron-sound.ogg:system/media/poweron-sound.ogg
endif

