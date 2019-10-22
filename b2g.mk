PRODUCT_PACKAGES += \
	b2g-info \
	b2g.sh \
	init.b2g.rc \
	libcarthage \
	gecko \
	$(NULL)


# Dhcpcd
DHCPCD_USE_SCRIPT := yes

PRODUCT_PACKAGES += \
    dhcpcd-6.8.2 \
    dhcpcd.conf \
    dhcpcd-run-hooks \
    20-dns.conf \
    95-configured
