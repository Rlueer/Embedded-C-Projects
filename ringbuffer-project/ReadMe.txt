compile:
gcc tests/test_main.c src/ring_buffer.c src/uart_parser.c -Iinclude -o test.exe

run (windows):
test.exe
