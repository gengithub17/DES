all: main
	./main

OUTPUT=data.txt
file: main
	./main > $(OUTPUT)
	code $(OUTPUT)

main: src/*.c options.h lib/*.h
	gcc -o main src/*.c -I./lib -include options.h

clean:
	rm main $(OUTPUT)
