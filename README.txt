Installation Instructions
==========================

|||||||| OSX ONLY |||||||||

1. Vamp Plugins Installation
ZoneRush is a Vamp Host. In order to compile this, you must download the latest version of Vamp (2.6 as of compile time)

Step 1. Generate binaries by running

    $ ./configure
    $ make -f build/Makefile.osx

Step 2. Move the generated libvamp-hostsdk and libvamp-sdk .a and .dylib files into the /usr/local/lib folder

Step 3. Move ./vamp ./vamp-hostsdk ./vamp-sdk into /usr/local/include folder

Step 4. DONE.

