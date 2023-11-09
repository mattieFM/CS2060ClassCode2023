You can compile with this command if you want to.:
gcc $( pkg-config --cflags gtk4 ) -o example-0 main.c $( pkg-config --libs gtk4 )



