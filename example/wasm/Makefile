CC=clang
CFLAGS=\
  --target=wasm32 \
  -Os \
  -I./libc \
  -nostdlib \
  -Wall
LDFLAGS=\
  -Wl,--no-entry \
  -Wl,--export-all \
  -Wl,--import-memory \
  --target=wasm32 \
  -nostdlib \
  -Os

json-pointer-loc.wasm: json-pointer-loc.o 
	$(CC) -o $@ $< $(LDFLAGS)

json-pointer-loc.o: ../../json-pointer-loc.c ../../json-pointer-loc.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f json-pointer-loc.o json-pointer-loc.wasm

