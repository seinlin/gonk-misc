# The external/sepolicy/kaios_sepolicy.mk is a kaios sepolicy enabler
ifeq ($(KAIOS_SEPOLICY_ENABLED),true)
BOARD_SEPOLICY_DIRS += gonk-misc/sepolicy/common
ifeq ($(call is-vendor-board-platform,QCOM),true)
BOARD_SEPOLICY_DIRS += gonk-misc/sepolicy/qualcomm
endif
ifeq ("$(PRODUCT_MANUFACTURER)", "SPRD")
BOARD_SEPOLICY_DIRS += gonk-misc/sepolicy/spreadtrum
endif
endif
