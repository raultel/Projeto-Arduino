all: upload

upload: morse/morse.ino morse/char_table.h
	arduino --upload morse/morse.ino
