CROSS_COMPILE?=/root/armv7l-tizen/bin/armv7l-tizen-linux-gnueabi-

.PHONY: decrypt_spm
decrypt_spm:
	$(CROSS_COMPILE)gcc -std=c99 $(@).c -Wl,--unresolved-symbols=ignore-in-shared-libs -L../tizen_libs_T -lappdrm -o $(@)