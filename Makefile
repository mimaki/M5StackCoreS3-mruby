all:
	mkdir -p components/mruby_component/mruby/build/esp32/include
	idf.py build

flash:
	idf.py flash

config:
	idf.py menuconfig

clean:
	idf.py fullclean clean

.PHONY: all