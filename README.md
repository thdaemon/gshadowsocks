# gshadowsocks (Work is not completed)

A free GTK+ frontend of the shadowsocks client.

**THIS PROGRAM IS IN TESTING**

![Screenshot](https://raw.githubusercontent.com/thdaemon/gshadowsocks/unusable/img/screenshot.png)

To install this program, run

```
make defconfig
make
sudo make install
```

To build but not install it, and test the program, you can run

```
./mkconfig.sh --prefix=`pwd`
make
./gssocks
```

To build a .deb format package, run

```
make defconfig
make
make deb
```

If you want to install it in `/opt/gshadowsocks` not `/usr`, run

```
./mkconfig.sh --prefix=/opt/gshadowsocks
make
sudo make PREFIX=/opt/gshadowsocks install
```

**NOTE** About the prefix and PREFIX, see [this wiki](doc/prefix-and-PREFIX.md).

## License

This program is distributed in the hope that it  will be useful, but WITHOUT ANY WARRANTY.

This software is licensed under the GNU GPLv3 and you can also use newer versions license. see [LICENSE file](LICENSE)
 
