all:
	mkdir -p components/mruby_component/mruby/build/esp32/include
	idf.py build

flash:
	idf.py flash

clean:
	idf.py fullclean clean

.PHONY: all