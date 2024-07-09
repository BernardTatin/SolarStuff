Playing with C on Solaris...

The first version of my window come from 
http://www.geeks3d.com/20120102/programming-tutorial-simple-x11-x-window-code-sample-for-linux-and-mac-os-x/. 
It was done on Solaris10, it works on Solaris11.

## OS

### FreeBSD
Works on FreeBSD 14.1:

```
make -f Makefile.freebsd
```

### Solaris
I have to install one on an external SSD... Pfff

## Makefile(s)

### for GNU make

Well tested on FreeBSD with `gmake`.

### for FreeBSD make

The last line of the `Makefile` is:

```
.include <bsd.prog.mk>
```

All the lines before are only the configuration for the last line. That's why the two first lines are here:

```
MK_DEBUG_FILES = no
NO_MAN=
```

I don't want the debug version of the binary, I don't want to build the man page (because I give no files for that). 

It's easy to understand and you don't have to play with the strange syntax of this sort of things. I love that.

I found a small flaw: it's not so easy to find some documentation. The best one is on the [GitHub site of FreeBSD](https://github.com/lattera/freebsd/blob/master/share/mk/bsd.README). There is another related page: [STYLE.MAKEFILE(5)](https://man.freebsd.org/cgi/man.cgi?query=style.Makefile).
