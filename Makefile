# This file was automagically generated by mbed.org. For more information, 
# see http://mbed.org/handbook/Exporting-to-GCC-ARM-Embedded

###############################################################################
# Boiler-plate

# cross-platform directory manipulation
ifeq ($(shell echo $$OS),$$OS)
    MAKEDIR = if not exist "$(1)" mkdir "$(1)"
    RM = rmdir /S /Q "$(1)"
else
    MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
    RM = '$(SHELL)' -c "rm -rf \"$(1)\""
endif

OBJDIR := BUILD
# Move to the build directory
ifeq (,$(filter $(OBJDIR),$(notdir $(CURDIR))))
.SUFFIXES:
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKETARGET = '$(MAKE)' --no-print-directory -C $(OBJDIR) -f '$(mkfile_path)' \
		'SRCDIR=$(CURDIR)' $(MAKECMDGOALS)
.PHONY: $(OBJDIR) clean
all:
	+@$(call MAKEDIR,$(OBJDIR))
	+@$(MAKETARGET)
$(OBJDIR): all
Makefile : ;
% :: $(OBJDIR) ; :
clean :
	$(call RM,$(OBJDIR))

else

# trick rules into thinking we are in the root, when we are in the bulid dir
VPATH = ..

# Boiler-plate
###############################################################################
# Project settings

PROJECT := myProject


# Project settings
###############################################################################
# Objects and Paths

OBJECTS += ./include/MPL3115A2.o
OBJECTS += ./mbed-os/drivers/AnalogIn.o
OBJECTS += ./mbed-os/drivers/BusIn.o
OBJECTS += ./mbed-os/drivers/BusInOut.o
OBJECTS += ./mbed-os/drivers/BusOut.o
OBJECTS += ./mbed-os/drivers/CAN.o
OBJECTS += ./mbed-os/drivers/Ethernet.o
OBJECTS += ./mbed-os/drivers/FlashIAP.o
OBJECTS += ./mbed-os/drivers/I2C.o
OBJECTS += ./mbed-os/drivers/I2CSlave.o
OBJECTS += ./mbed-os/drivers/InterruptIn.o
OBJECTS += ./mbed-os/drivers/InterruptManager.o
OBJECTS += ./mbed-os/drivers/RawSerial.o
OBJECTS += ./mbed-os/drivers/SPI.o
OBJECTS += ./mbed-os/drivers/SPISlave.o
OBJECTS += ./mbed-os/drivers/Serial.o
OBJECTS += ./mbed-os/drivers/SerialBase.o
OBJECTS += ./mbed-os/drivers/Ticker.o
OBJECTS += ./mbed-os/drivers/Timeout.o
OBJECTS += ./mbed-os/drivers/Timer.o
OBJECTS += ./mbed-os/drivers/TimerEvent.o
OBJECTS += ./mbed-os/drivers/UARTSerial.o
OBJECTS += ./mbed-os/events/EventQueue.o
OBJECTS += ./mbed-os/events/equeue/equeue.o
OBJECTS += ./mbed-os/events/equeue/equeue_mbed.o
OBJECTS += ./mbed-os/events/equeue/equeue_posix.o
OBJECTS += ./mbed-os/features/filesystem/Dir.o
OBJECTS += ./mbed-os/features/filesystem/File.o
OBJECTS += ./mbed-os/features/filesystem/FileSystem.o
OBJECTS += ./mbed-os/features/filesystem/bd/ChainingBlockDevice.o
OBJECTS += ./mbed-os/features/filesystem/bd/HeapBlockDevice.o
OBJECTS += ./mbed-os/features/filesystem/bd/MBRBlockDevice.o
OBJECTS += ./mbed-os/features/filesystem/bd/SlicingBlockDevice.o
OBJECTS += ./mbed-os/features/filesystem/fat/ChaN/ccsbcs.o
OBJECTS += ./mbed-os/features/filesystem/fat/ChaN/ff.o
OBJECTS += ./mbed-os/features/filesystem/fat/FATFileSystem.o
OBJECTS += ./mbed-os/features/frameworks/greentea-client/source/greentea_metrics.o
OBJECTS += ./mbed-os/features/frameworks/greentea-client/source/greentea_serial.o
OBJECTS += ./mbed-os/features/frameworks/greentea-client/source/greentea_test_env.o
OBJECTS += ./mbed-os/features/frameworks/unity/source/unity.o
OBJECTS += ./mbed-os/features/frameworks/utest/mbed-utest-shim.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/unity_handler.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_case.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_default_handlers.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_greentea_handlers.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_harness.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_shim.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_stack_trace.o
OBJECTS += ./mbed-os/features/frameworks/utest/source/utest_types.o
OBJECTS += ./mbed-os/features/mbedtls/platform/src/mbed_trng.o
OBJECTS += ./mbed-os/features/mbedtls/src/aes.o
OBJECTS += ./mbed-os/features/mbedtls/src/aesni.o
OBJECTS += ./mbed-os/features/mbedtls/src/arc4.o
OBJECTS += ./mbed-os/features/mbedtls/src/asn1parse.o
OBJECTS += ./mbed-os/features/mbedtls/src/asn1write.o
OBJECTS += ./mbed-os/features/mbedtls/src/base64.o
OBJECTS += ./mbed-os/features/mbedtls/src/bignum.o
OBJECTS += ./mbed-os/features/mbedtls/src/blowfish.o
OBJECTS += ./mbed-os/features/mbedtls/src/camellia.o
OBJECTS += ./mbed-os/features/mbedtls/src/ccm.o
OBJECTS += ./mbed-os/features/mbedtls/src/certs.o
OBJECTS += ./mbed-os/features/mbedtls/src/cipher.o
OBJECTS += ./mbed-os/features/mbedtls/src/cipher_wrap.o
OBJECTS += ./mbed-os/features/mbedtls/src/cmac.o
OBJECTS += ./mbed-os/features/mbedtls/src/ctr_drbg.o
OBJECTS += ./mbed-os/features/mbedtls/src/debug.o
OBJECTS += ./mbed-os/features/mbedtls/src/des.o
OBJECTS += ./mbed-os/features/mbedtls/src/dhm.o
OBJECTS += ./mbed-os/features/mbedtls/src/ecdh.o
OBJECTS += ./mbed-os/features/mbedtls/src/ecdsa.o
OBJECTS += ./mbed-os/features/mbedtls/src/ecjpake.o
OBJECTS += ./mbed-os/features/mbedtls/src/ecp.o
OBJECTS += ./mbed-os/features/mbedtls/src/ecp_curves.o
OBJECTS += ./mbed-os/features/mbedtls/src/entropy.o
OBJECTS += ./mbed-os/features/mbedtls/src/entropy_poll.o
OBJECTS += ./mbed-os/features/mbedtls/src/error.o
OBJECTS += ./mbed-os/features/mbedtls/src/gcm.o
OBJECTS += ./mbed-os/features/mbedtls/src/havege.o
OBJECTS += ./mbed-os/features/mbedtls/src/hmac_drbg.o
OBJECTS += ./mbed-os/features/mbedtls/src/md.o
OBJECTS += ./mbed-os/features/mbedtls/src/md2.o
OBJECTS += ./mbed-os/features/mbedtls/src/md4.o
OBJECTS += ./mbed-os/features/mbedtls/src/md5.o
OBJECTS += ./mbed-os/features/mbedtls/src/md_wrap.o
OBJECTS += ./mbed-os/features/mbedtls/src/memory_buffer_alloc.o
OBJECTS += ./mbed-os/features/mbedtls/src/net_sockets.o
OBJECTS += ./mbed-os/features/mbedtls/src/oid.o
OBJECTS += ./mbed-os/features/mbedtls/src/padlock.o
OBJECTS += ./mbed-os/features/mbedtls/src/pem.o
OBJECTS += ./mbed-os/features/mbedtls/src/pk.o
OBJECTS += ./mbed-os/features/mbedtls/src/pk_wrap.o
OBJECTS += ./mbed-os/features/mbedtls/src/pkcs11.o
OBJECTS += ./mbed-os/features/mbedtls/src/pkcs12.o
OBJECTS += ./mbed-os/features/mbedtls/src/pkcs5.o
OBJECTS += ./mbed-os/features/mbedtls/src/pkparse.o
OBJECTS += ./mbed-os/features/mbedtls/src/pkwrite.o
OBJECTS += ./mbed-os/features/mbedtls/src/platform.o
OBJECTS += ./mbed-os/features/mbedtls/src/ripemd160.o
OBJECTS += ./mbed-os/features/mbedtls/src/rsa.o
OBJECTS += ./mbed-os/features/mbedtls/src/sha1.o
OBJECTS += ./mbed-os/features/mbedtls/src/sha256.o
OBJECTS += ./mbed-os/features/mbedtls/src/sha512.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_cache.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_ciphersuites.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_cli.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_cookie.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_srv.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_ticket.o
OBJECTS += ./mbed-os/features/mbedtls/src/ssl_tls.o
OBJECTS += ./mbed-os/features/mbedtls/src/threading.o
OBJECTS += ./mbed-os/features/mbedtls/src/timing.o
OBJECTS += ./mbed-os/features/mbedtls/src/version.o
OBJECTS += ./mbed-os/features/mbedtls/src/version_features.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509_create.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509_crl.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509_crt.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509_csr.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509write_crt.o
OBJECTS += ./mbed-os/features/mbedtls/src/x509write_csr.o
OBJECTS += ./mbed-os/features/mbedtls/src/xtea.o
OBJECTS += ./mbed-os/features/netsocket/NetworkInterface.o
OBJECTS += ./mbed-os/features/netsocket/NetworkStack.o
OBJECTS += ./mbed-os/features/netsocket/Socket.o
OBJECTS += ./mbed-os/features/netsocket/SocketAddress.o
OBJECTS += ./mbed-os/features/netsocket/TCPServer.o
OBJECTS += ./mbed-os/features/netsocket/TCPSocket.o
OBJECTS += ./mbed-os/features/netsocket/UDPSocket.o
OBJECTS += ./mbed-os/features/netsocket/WiFiAccessPoint.o
OBJECTS += ./mbed-os/features/netsocket/cellular/generic_modem_driver/OnboardCellularInterface.o
OBJECTS += ./mbed-os/features/netsocket/cellular/generic_modem_driver/PPPCellularInterface.o
OBJECTS += ./mbed-os/features/netsocket/cellular/generic_modem_driver/UARTCellularInterface.o
OBJECTS += ./mbed-os/features/netsocket/nsapi_dns.o
OBJECTS += ./mbed-os/hal/mbed_flash_api.o
OBJECTS += ./mbed-os/hal/mbed_gpio.o
OBJECTS += ./mbed-os/hal/mbed_lp_ticker_api.o
OBJECTS += ./mbed-os/hal/mbed_pinmap_common.o
OBJECTS += ./mbed-os/hal/mbed_ticker_api.o
OBJECTS += ./mbed-os/hal/mbed_us_ticker_api.o
OBJECTS += ./mbed-os/platform/ATCmdParser.o
OBJECTS += ./mbed-os/platform/CallChain.o
OBJECTS += ./mbed-os/platform/FileBase.o
OBJECTS += ./mbed-os/platform/FileHandle.o
OBJECTS += ./mbed-os/platform/FilePath.o
OBJECTS += ./mbed-os/platform/FileSystemHandle.o
OBJECTS += ./mbed-os/platform/LocalFileSystem.o
OBJECTS += ./mbed-os/platform/Stream.o
OBJECTS += ./mbed-os/platform/mbed_alloc_wrappers.o
OBJECTS += ./mbed-os/platform/mbed_application.o
OBJECTS += ./mbed-os/platform/mbed_assert.o
OBJECTS += ./mbed-os/platform/mbed_board.o
OBJECTS += ./mbed-os/platform/mbed_critical.o
OBJECTS += ./mbed-os/platform/mbed_error.o
OBJECTS += ./mbed-os/platform/mbed_interface.o
OBJECTS += ./mbed-os/platform/mbed_mem_trace.o
OBJECTS += ./mbed-os/platform/mbed_mktime.o
OBJECTS += ./mbed-os/platform/mbed_poll.o
OBJECTS += ./mbed-os/platform/mbed_retarget.o
OBJECTS += ./mbed-os/platform/mbed_rtc_time.o
OBJECTS += ./mbed-os/platform/mbed_sdk_boot.o
OBJECTS += ./mbed-os/platform/mbed_semihost_api.o
OBJECTS += ./mbed-os/platform/mbed_stats.o
OBJECTS += ./mbed-os/platform/mbed_wait_api_no_rtos.o
OBJECTS += ./mbed-os/platform/mbed_wait_api_rtos.o
OBJECTS += ./mbed-os/rtos/Mutex.o
OBJECTS += ./mbed-os/rtos/RtosTimer.o
OBJECTS += ./mbed-os/rtos/Semaphore.o
OBJECTS += ./mbed-os/rtos/Thread.o
OBJECTS += ./mbed-os/rtos/mbed_boot.o
OBJECTS += ./mbed-os/rtos/rtos_idle.o
OBJECTS += ./mbed-os/rtos/rtx4/cmsis_os1.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/RTX_Config.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/TARGET_RTOS_M4_M7/TOOLCHAIN_GCC/irq_cm4f.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rt_OsEventObserver.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_delay.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_evflags.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_evr.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_kernel.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_lib.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_memory.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_mempool.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_msgqueue.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_mutex.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_semaphore.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_system.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_thread.o
OBJECTS += ./mbed-os/rtos/rtx5/TARGET_CORTEX_M/rtx_timer.o
OBJECTS += ./mbed-os/rtos/rtx5/mbed_rtx_handlers.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_HEXIWEAR/PeripheralPins.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_HEXIWEAR/fsl_clock_config.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_HEXIWEAR/mbed_overrides.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device/TOOLCHAIN_GCC_ARM/startup_MK64F12.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device/system_MK64F12.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_adc16.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_clock.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_cmp.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_cmt.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_common.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_crc.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_dac.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_dmamux.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_dspi.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_dspi_edma.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_edma.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_enet.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_ewm.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_flash.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_flexbus.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_flexcan.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_ftm.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_gpio.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_i2c.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_i2c_edma.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_llwu.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_lptmr.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_mpu.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_pdb.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_pit.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_pmc.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_rcm.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_rnga.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_rtc.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_sai.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_sai_edma.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_sdhc.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_sim.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_smc.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_uart.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_uart_edma.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_vref.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers/fsl_wdog.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/pwmout_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/serial_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/spi_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/storage_driver.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/trng_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/us_ticker.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/analogin_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/analogout_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/dma_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/flash_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/gpio_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/gpio_irq_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/i2c_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/lp_ticker.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/pinmap.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/port_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/rtc_api.o
OBJECTS += ./mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api/sleep.o
OBJECTS += ./sources/main.o


INCLUDE_PATHS += -I../
INCLUDE_PATHS += -I../.
INCLUDE_PATHS += -I.././include
INCLUDE_PATHS += -I.././mbed-os
INCLUDE_PATHS += -I.././mbed-os/cmsis
INCLUDE_PATHS += -I.././mbed-os/cmsis/TARGET_CORTEX_M
INCLUDE_PATHS += -I.././mbed-os/cmsis/TARGET_CORTEX_M/TOOLCHAIN_GCC
INCLUDE_PATHS += -I.././mbed-os/drivers
INCLUDE_PATHS += -I.././mbed-os/events
INCLUDE_PATHS += -I.././mbed-os/events/equeue
INCLUDE_PATHS += -I.././mbed-os/features
INCLUDE_PATHS += -I.././mbed-os/features/filesystem
INCLUDE_PATHS += -I.././mbed-os/features/filesystem/bd
INCLUDE_PATHS += -I.././mbed-os/features/filesystem/fat
INCLUDE_PATHS += -I.././mbed-os/features/filesystem/fat/ChaN
INCLUDE_PATHS += -I.././mbed-os/features/frameworks
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/greentea-client
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/greentea-client/greentea-client
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/greentea-client/source
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/unity
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/unity/source
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/unity/unity
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/utest
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/utest/source
INCLUDE_PATHS += -I.././mbed-os/features/frameworks/utest/utest
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/importer
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/inc
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/inc/mbedtls
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/platform
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/platform/inc
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/platform/src
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/src
INCLUDE_PATHS += -I.././mbed-os/features/mbedtls/targets
INCLUDE_PATHS += -I.././mbed-os/features/nanostack
INCLUDE_PATHS += -I.././mbed-os/features/netsocket
INCLUDE_PATHS += -I.././mbed-os/features/netsocket/cellular
INCLUDE_PATHS += -I.././mbed-os/features/netsocket/cellular/generic_modem_driver
INCLUDE_PATHS += -I.././mbed-os/features/netsocket/cellular/utils
INCLUDE_PATHS += -I.././mbed-os/features/storage
INCLUDE_PATHS += -I.././mbed-os/hal
INCLUDE_PATHS += -I.././mbed-os/hal/storage_abstraction
INCLUDE_PATHS += -I.././mbed-os/platform
INCLUDE_PATHS += -I.././mbed-os/rtos
INCLUDE_PATHS += -I.././mbed-os/rtos/rtx4
INCLUDE_PATHS += -I.././mbed-os/rtos/rtx5
INCLUDE_PATHS += -I.././mbed-os/rtos/rtx5/TARGET_CORTEX_M
INCLUDE_PATHS += -I.././mbed-os/rtos/rtx5/TARGET_CORTEX_M/TARGET_RTOS_M4_M7
INCLUDE_PATHS += -I.././mbed-os/rtos/rtx5/TARGET_CORTEX_M/TARGET_RTOS_M4_M7/TOOLCHAIN_GCC
INCLUDE_PATHS += -I.././mbed-os/targets
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_HEXIWEAR
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device/TOOLCHAIN_GCC_ARM
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers
INCLUDE_PATHS += -I.././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api
INCLUDE_PATHS += -I.././sources

LIBRARY_PATHS :=
LIBRARIES :=
LINKER_SCRIPT ?= .././mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device/TOOLCHAIN_GCC_ARM/MK64FN1M0xxx12.ld

# Objects and Paths
###############################################################################
# Tools and Flags

AS      = 'arm-none-eabi-gcc' '-x' 'assembler-with-cpp' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-O0' '-g3' '-DMBED_DEBUG' '-DMBED_TRAP_ERRORS_ENABLED=1' '-mcpu=cortex-m4' '-mthumb' '-mfpu=fpv4-sp-d16' '-mfloat-abi=softfp'
CC      = 'arm-none-eabi-gcc' '-std=gnu99' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-O0' '-g3' '-DMBED_DEBUG' '-DMBED_TRAP_ERRORS_ENABLED=1' '-mcpu=cortex-m4' '-mthumb' '-mfpu=fpv4-sp-d16' '-mfloat-abi=softfp'
CPP     = 'arm-none-eabi-g++' '-std=gnu++98' '-fno-rtti' '-Wvla' '-c' '-Wall' '-Wextra' '-Wno-unused-parameter' '-Wno-missing-field-initializers' '-fmessage-length=0' '-fno-exceptions' '-fno-builtin' '-ffunction-sections' '-fdata-sections' '-funsigned-char' '-MMD' '-fno-delete-null-pointer-checks' '-fomit-frame-pointer' '-O0' '-g3' '-DMBED_DEBUG' '-DMBED_TRAP_ERRORS_ENABLED=1' '-mcpu=cortex-m4' '-mthumb' '-mfpu=fpv4-sp-d16' '-mfloat-abi=softfp'
LD      = 'arm-none-eabi-gcc'
ELF2BIN = 'arm-none-eabi-objcopy'
PREPROC = 'arm-none-eabi-cpp' '-E' '-P' '-Wl,--gc-sections' '-Wl,--wrap,main' '-Wl,--wrap,_malloc_r' '-Wl,--wrap,_free_r' '-Wl,--wrap,_realloc_r' '-Wl,--wrap,_memalign_r' '-Wl,--wrap,_calloc_r' '-Wl,--wrap,exit' '-Wl,--wrap,atexit' '-Wl,-n' '-mcpu=cortex-m4' '-mthumb' '-mfpu=fpv4-sp-d16' '-mfloat-abi=softfp'


C_FLAGS += -std=gnu99
C_FLAGS += -DTARGET_KSDK2_MCUS
C_FLAGS += -D__MBED__=1
C_FLAGS += -DDEVICE_I2CSLAVE=1
C_FLAGS += -DTARGET_LIKE_MBED
C_FLAGS += -DTARGET_Freescale
C_FLAGS += -DDEVICE_PORTINOUT=1
C_FLAGS += -DTARGET_RTOS_M4_M7
C_FLAGS += -DDEVICE_RTC=1
C_FLAGS += -DTOOLCHAIN_object
C_FLAGS += -DDEVICE_SERIAL_ASYNCH=1
C_FLAGS += -D__CMSIS_RTOS
C_FLAGS += -DFSL_RTOS_MBED
C_FLAGS += -DTARGET_HEXIWEAR
C_FLAGS += -DARM_MATH_CM4
C_FLAGS += -DTARGET_DEBUG
C_FLAGS += -DTARGET_LIKE_CORTEX_M4
C_FLAGS += -DDEVICE_ANALOGOUT=1
C_FLAGS += -DTARGET_M4
C_FLAGS += -DTARGET_UVISOR_UNSUPPORTED
C_FLAGS += -DTARGET_K64F
C_FLAGS += -DDEVICE_SPI_ASYNCH=1
C_FLAGS += -DDEVICE_SERIAL=1
C_FLAGS += -DDEVICE_INTERRUPTIN=1
C_FLAGS += -DDEVICE_I2C=1
C_FLAGS += -DDEVICE_PORTOUT=1
C_FLAGS += -D__CORTEX_M4
C_FLAGS += -DDEVICE_STDIO_MESSAGES=1
C_FLAGS += -DCPU_MK64FN1M0VMD12
C_FLAGS += -D__FPU_PRESENT=1
C_FLAGS += -DDEVICE_PORTIN=1
C_FLAGS += -DMBED_BUILD_TIMESTAMP=1502031010.81
C_FLAGS += -DTOOLCHAIN_GCC
C_FLAGS += -DDEVICE_TRNG=1
C_FLAGS += -D__MBED_CMSIS_RTOS_CM
C_FLAGS += -DDEVICE_SLEEP=1
C_FLAGS += -DTOOLCHAIN_GCC_ARM
C_FLAGS += -DTARGET_MCUXpresso_MCUS
C_FLAGS += -DDEVICE_SPI=1
C_FLAGS += -DDEVICE_ERROR_RED=1
C_FLAGS += -DDEVICE_SPISLAVE=1
C_FLAGS += -DDEVICE_ANALOGIN=1
C_FLAGS += -DDEVICE_PWMOUT=1
C_FLAGS += -DDEVICE_FLASH=1
C_FLAGS += -DTARGET_MCU_K64F
C_FLAGS += -DTARGET_CORTEX_M
C_FLAGS += -DDEVICE_SERIAL_FC=1
C_FLAGS += -include
C_FLAGS += mbed_config.h

CXX_FLAGS += -std=gnu++98
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -Wvla
CXX_FLAGS += -DTARGET_KSDK2_MCUS
CXX_FLAGS += -D__MBED__=1
CXX_FLAGS += -DDEVICE_I2CSLAVE=1
CXX_FLAGS += -DTARGET_LIKE_MBED
CXX_FLAGS += -DTARGET_Freescale
CXX_FLAGS += -DDEVICE_PORTINOUT=1
CXX_FLAGS += -DTARGET_RTOS_M4_M7
CXX_FLAGS += -DDEVICE_RTC=1
CXX_FLAGS += -DTOOLCHAIN_object
CXX_FLAGS += -DDEVICE_SERIAL_ASYNCH=1
CXX_FLAGS += -D__CMSIS_RTOS
CXX_FLAGS += -DFSL_RTOS_MBED
CXX_FLAGS += -DTARGET_HEXIWEAR
CXX_FLAGS += -DARM_MATH_CM4
CXX_FLAGS += -DTARGET_DEBUG
CXX_FLAGS += -DTARGET_LIKE_CORTEX_M4
CXX_FLAGS += -DDEVICE_ANALOGOUT=1
CXX_FLAGS += -DTARGET_M4
CXX_FLAGS += -DTARGET_UVISOR_UNSUPPORTED
CXX_FLAGS += -DTARGET_K64F
CXX_FLAGS += -DDEVICE_SPI_ASYNCH=1
CXX_FLAGS += -DDEVICE_SERIAL=1
CXX_FLAGS += -DDEVICE_INTERRUPTIN=1
CXX_FLAGS += -DDEVICE_I2C=1
CXX_FLAGS += -DDEVICE_PORTOUT=1
CXX_FLAGS += -D__CORTEX_M4
CXX_FLAGS += -DDEVICE_STDIO_MESSAGES=1
CXX_FLAGS += -DCPU_MK64FN1M0VMD12
CXX_FLAGS += -D__FPU_PRESENT=1
CXX_FLAGS += -DDEVICE_PORTIN=1
CXX_FLAGS += -DMBED_BUILD_TIMESTAMP=1502031010.81
CXX_FLAGS += -DTOOLCHAIN_GCC
CXX_FLAGS += -DDEVICE_TRNG=1
CXX_FLAGS += -D__MBED_CMSIS_RTOS_CM
CXX_FLAGS += -DDEVICE_SLEEP=1
CXX_FLAGS += -DTOOLCHAIN_GCC_ARM
CXX_FLAGS += -DTARGET_MCUXpresso_MCUS
CXX_FLAGS += -DDEVICE_SPI=1
CXX_FLAGS += -DDEVICE_ERROR_RED=1
CXX_FLAGS += -DDEVICE_SPISLAVE=1
CXX_FLAGS += -DDEVICE_ANALOGIN=1
CXX_FLAGS += -DDEVICE_PWMOUT=1
CXX_FLAGS += -DDEVICE_FLASH=1
CXX_FLAGS += -DTARGET_MCU_K64F
CXX_FLAGS += -DTARGET_CORTEX_M
CXX_FLAGS += -DDEVICE_SERIAL_FC=1
CXX_FLAGS += -include
CXX_FLAGS += mbed_config.h

ASM_FLAGS += -x
ASM_FLAGS += assembler-with-cpp
ASM_FLAGS += -D__CORTEX_M4
ASM_FLAGS += -DCPU_MK64FN1M0VMD12
ASM_FLAGS += -DARM_MATH_CM4
ASM_FLAGS += -D__FPU_PRESENT=1
ASM_FLAGS += -D__MBED_CMSIS_RTOS_CM
ASM_FLAGS += -DTARGET_K64F
ASM_FLAGS += -D__CMSIS_RTOS
ASM_FLAGS += -DFSL_RTOS_MBED


LD_FLAGS :=-Wl,--gc-sections -Wl,--wrap,main -Wl,--wrap,_malloc_r -Wl,--wrap,_free_r -Wl,--wrap,_realloc_r -Wl,--wrap,_memalign_r -Wl,--wrap,_calloc_r -Wl,--wrap,exit -Wl,--wrap,atexit -Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp 
LD_SYS_LIBS :=-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys -Wl,--end-group

# Tools and Flags
###############################################################################
# Rules

.PHONY: all lst size


all: $(PROJECT).bin $(PROJECT).hex size


.asm.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.s.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.S.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Assemble: $(notdir $<)"
	@$(AS) -c $(ASM_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.c.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CC) $(C_FLAGS) $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	+@$(call MAKEDIR,$(dir $@))
	+@echo "Compile: $(notdir $<)"
	@$(CPP) $(CXX_FLAGS) $(INCLUDE_PATHS) -o $@ $<


$(PROJECT).link_script.ld: $(LINKER_SCRIPT)
	@$(PREPROC) $< -o $@



$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS) $(PROJECT).link_script.ld 
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(filter %.ld, $^) $(LIBRARY_PATHS) --output $@ $(filter %.o, $^) $(LIBRARIES) $(LD_SYS_LIBS)


$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@
	+@echo "===== bin file ready to flash: $(OBJDIR)/$@ =====" 

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@


# Rules
###############################################################################
# Dependencies

DEPS = $(OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)
endif

# Dependencies
###############################################################################
