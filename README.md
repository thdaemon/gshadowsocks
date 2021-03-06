# gshadowsocks

A free GTK+ frontend of the shadowsocks client.

**THIS PROGRAM IS IN TESTING**

![Screenshot](https://raw.githubusercontent.com/thdaemon/gshadowsocks/unusable/img/screenshot.png)

Install the dependent packages

```
# for Debian, Ubuntu, etc
sudo apt install build-essential libgtk-3-dev libappindicator3-dev
```

And you need install [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev).

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

Icon files in `/share/gshadowsocks/icon/` are from [shadowsocks-windows](https://github.com/shadowsocks/shadowsocks-windows) project and it is licensed under GPLv3.

Icon files in `/share/pixmaps` are from [ShadowsocksX-NG](https://github.com/shadowsocks/ShadowsocksX-NG) project and it is licensed under GPLv3.

## TODO List

- [x] Make a basic usable version
- [x] Optimize ugly code
- [ ] Implement "Update GFWList"
- [ ] Create a Github pages of the project
- [ ] Add automatical starting feature
