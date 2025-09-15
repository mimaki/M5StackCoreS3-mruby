all:
	idf.py build

flash:
	idf.py flash

clean:
	idf.py fullclean clean

.PHONY: all