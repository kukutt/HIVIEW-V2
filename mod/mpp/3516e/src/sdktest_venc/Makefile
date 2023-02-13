# Hisilicon Hi35xx sample Makefile

ifeq ($(PARAM_FILE), )
     PARAM_FILE:=Makefile_root.param
     include $(PARAM_FILE)
endif

################# select sensor type for your sample #################
####    SONY_IMX335_MIPI_5M_30FPS_12BIT              #################
####    SONY_IMX335_MIPI_4M_30FPS_12BIT              #################
####    SONY_IMX327_MIPI_2M_30FPS_12BIT              #################
####    SONY_IMX327_MIPI_2M_30FPS_12BIT_WDR2TO1      #################
####    SONY_IMX307_MIPI_2M_30FPS_12BIT              #################
####    SONY_IMX307_MIPI_2M_30FPS_12BIT_WDR2TO1      #################
####    SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1      #################
####    OMNIVISION_OS05A_MIPI_5M_30FPS_12BIT         #################
####    BT1120_2M_30FPS_8BIT                         #################

#### the previous sequence only for Hi3516EV300,Hi3516DV200 etc. #####

####    BT656_2M_30FPS_8BIT                          #################
####    SONY_IMX307_2L_MIPI_2M_30FPS_12BIT           #################
####    SONY_IMX307_2L_MIPI_2M_30FPS_12BIT_WDR2TO1   #################
####    SONY_IMX327_2L_MIPI_2M_30FPS_12BIT           #################
####    SONY_IMX327_2L_MIPI_2M_30FPS_12BIT_WDR2TO1   #################
####    SMART_SC4236_MIPI_3M_30FPS_10BIT             #################
####    SMART_SC4236_MIPI_3M_20FPS_10BIT             #################
####    SMART_SC2235_DC_2M_30FPS_10BIT               #################
####    SMART_SC2231_MIPI_2M_30FPS_10BIT             #################
####    SMART_SC3235_MIPI_3M_30FPS_10BIT             #################
####    GALAXYCORE_GC2053_MIPI_2M_30FPS_10BIT        #################
####    GALAXYCORE_GC2053_MIPI_2M_30FPS_10BIT_FORCAR #################

SENSOR0_TYPE_D ?= SONY_IMX335_MIPI_4M_30FPS_12BIT 
export LIBHZ := ".so"
SENSOR_LIBS := $(REL_LIB)/lib_hiae${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libisp${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/lib_hidehaze${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/lib_hidrc${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/lib_hildci${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/lib_hiawb${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/lib_hiir_auto${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_imx327${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_imx307${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_imx307_2l${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_imx327_2l${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_imx335${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_sc4236${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_sc2235${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_sc2231${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_sc3235${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_gc2053${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_gc2053_forcar${LIBHZ}
SENSOR_LIBS += $(REL_LIB)/libsns_os05a${LIBHZ}
MPI_LIBS := $(REL_LIB)/libmpi${LIBHZ}

COMMON_DIR ?= $(PWD)/../common
AUDIO_ADP_DIR ?= $(PWD)/../audio/adp

INC_FLAGS := -I$(REL_INC)

ISP_VERSION := ISP_V2

CFLAGS += -Wall -g $(INC_FLAGS) -D$(HIARCH) -D$(HI_FPGA) -DHISISDK -lpthread -lm -ldl -D$(ISP_VERSION)
CFLAGS += -lstdc++
CFLAGS += $(LIBS_CFLAGS)
CFLAGS += $(MPP_CFLAGS)
CFLAGS += -DSENSOR0_TYPE_D=$(SENSOR0_TYPE_D)


################ select audio codec type for your sample ################
ACODEC_TYPE ?= ACODEC_TYPE_INNER
#external acodec
#ACODEC_TYPE ?= ACODEC_TYPE_TLV320AIC31
INNER_HDMI ?= n

ifeq ($(ACODEC_TYPE), ACODEC_TYPE_INNER)
	CFLAGS += -DHI_ACODEC_TYPE_INNER
endif

ifeq ($(ACODEC_TYPE), ACODEC_TYPE_TLV320AIC31)
	CFLAGS += -DHI_ACODEC_TYPE_TLV320AIC31
	INC_FLAGS += -I$(SDK_PATH)/$(EXTDRV)/tlv320aic31
endif

ifeq ($(INNER_HDMI), y)
	CFLAGS += -DHI_ACODEC_TYPE_HDMI
endif

################ select audio module library type for your sample ################
#AUDIO_MODULE_LIB_TYPE ?= AUDIO_LIB_TYPE_STATIC
#AUDIO_MODULE_LIB_TYPE ?= AUDIO_LIB_TYPE_SHARE
ifeq ($(AUDIO_MODULE_LIB_TYPE), AUDIO_LIB_TYPE_STATIC)
##### VQE #####
	CFLAGS += -DHI_VQE_USE_STATIC_MODULE_REGISTER
	AUDIO_LIBA += $(REL_LIB)/libhive_RES${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libhive_record${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libhive_HPF${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libhive_ANR${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libhive_AEC${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libhive_AGC${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libhive_EQ${LIBHZ}
##### AAC #####
	CFLAGS += -DHI_AAC_USE_STATIC_MODULE_REGISTER
	AUDIO_LIBA += $(REL_LIB)/libaacdec${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libaacenc${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libaaccomm${LIBHZ}
	CFLAGS += -DHI_AAC_HAVE_SBR_LIB
	AUDIO_LIBA += $(REL_LIB)/libaacsbrdec${LIBHZ}
	AUDIO_LIBA += $(REL_LIB)/libaacsbrenc${LIBHZ}
endif

#########################################################################
COMM_SRC := $(wildcard $(COMMON_DIR)/*.c)
COMM_SRC += $(COMMON_DIR)/../sample_venc.c
COMM_OBJ := $(COMM_SRC:%.c=%.o)
COMM_INC := -I$(COMMON_DIR)
COMM_INC += -I$(AUDIO_ADP_DIR)
COMM_INC += -I$(COMMON_DIR)/../../inc/
COMM_INC += -I$(COMMON_DIR)/../../inc/common/
COMM_INC += -I$(COMMON_DIR)/../../inc/hisisdk/

#########################################################################
LITEOS_BIN_SUPPORT_VI := y

ifeq ($(LITEOS_BIN_SUPPORT_VI), y)

endif


SRCS := $(wildcard *.c)
TARGET := $(SRCS:%.c=%)

TARGET_PATH := $(PWD)

# compile linux or HuaweiLite
#include $(PWD)/$(ARM_ARCH)_$(OSTYPE).mak


# target source
OBJS  := $(SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	@$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(MPI_LIBS) $(SENSOR_LIBS) $(AUDIO_LIBA) $(REL_LIB)/libsecurec${LIBHZ} -Wl,--end-group

clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)

cleanstream:
	@rm -f *.h264
	@rm -f *.h265
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4
