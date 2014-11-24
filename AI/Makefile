all:
	$(MAKE) -C src all
	mv src/*.o bin/
	ar ruc bin/libAI.a bin/*.o

clean:
	$(MAKE) -C src clean
	rm bin/*.o bin/libAI.a
