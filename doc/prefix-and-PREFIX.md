## gshadowsocks Wiki - prefix and PREFIX

gshadowsocks source tree has two "prefix" variable, the first is the `--prefix=` of `./mkconfig.sh` script, the second is the `PREFIX` of Makefile variable.

### prefix

NOTE: the `make defconfig` will call `./mkconfig.sh --prefix=/usr`

The `--prefix=` defines the prefix of the fixed paths which needed by `gsscoks` binary program. These fixed paths are usually the paths for resource files and helper programs.

For example, when `gsscoks` need a shadowsocks's aircraft icon, it may use `CONFIG_PREFIX "/share/gshadowsocks/icon/normal.png"`, the `CONFIG_PREFIX` is generated by `--prefix=` at the compiling time.

### PREFIX

`PREFIX` indicates the prefix of the installation path. Its default value is `/usr`. If you run

```
make PREFIX=/opt/gshadowsocks install
```

Makefile script will install the whole program and its file to `/opt/gshadowsocks`.

### Tips and Hacks

Why do I separate these two variables? Considering that you are compiling this software for another system (not compiling for the current host), `PREFIX` provides a way to allow you to set the target system's location (root filesystem). Looks like this:

```
./mkconfig.sh --prefix=/usr
make
make PREFIX=/home/me/new_system_root/usr install
```

In this way, the software will be installed to `/home/me/new_system_root/usr/...`, but the program still references `/usr/...`. This makes it easy to install the software on target system (not current host system) without any other modifications.

And `--prefix=` solves another problem. Sometimes you do not want to install the software in the system's `/usr` directory. For example, you are more inclined to use `/opt/${software_name}`. Unlike GNU/Linux users, FreeBSD users prefer to install third-party software to the `/usr/local` directory. At the point, the resource file path which referenced in the program needs to be updated. `--prefix=` will done this.

```
./mkconfig.sh --prefix=/opt/gshadowsocks
make
make PREFIX=/opt/gshadowsocks install
```

There is another interesting usage. If you want to directly run the compiled binary in the source tree, you only need to do this to let the program find the correct resource file path. Because the resource file in source tree is organized according to the standard rules and the `pwd` utility prints the current directory. This may be useful in debugging the program.

```
./mkconfig.sh --prefix=`pwd`
make
# and then run the binary
./gssocks
```