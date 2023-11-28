
I have provided all dependancies that should be required within the libs and gtk folders. So it should be easy to build and run. If for some reason you do hit a dependancy issue. Unix is very easy to get gtk set up on so:
for macOs go the this link and follow the getting started steps: https://www.gtk.org/docs/installations/macos/ they havea fully built install script that handles everything for you
for linux https://www.gtk.org/docs/installations/linux/ go to here and follow install guide or just clone the repo for it.

You can compile with this command if you want to.:
gcc $( pkg-config --cflags gtk4 ) -o example-0 main.c $( pkg-config --libs gtk4 )

