#!/usr/bin/env bash

pkgname="gshadowsocks"
version=`cat version`
depends=`cat pkgsrc/deb_depends`

DEB_Section="net"
DEB_Priority="optional"

read -p "The arch? (e.g. i386, armhf, amd64, etc): " arch
[ "$arch" = '' ] && exit 1

name="${pkgname}-${arch}-${version}"

rm -rf build-package/${name}_deb build-package/${name}.deb
mkdir -p build-package/${name}_deb/DEBIAN

make PREFIX="`pwd`/build-package/${name}_deb/" install

cat > build-package/${name}_deb/DEBIAN/control << EOF
Package: ${pkgname}
Version: ${version}
Architecture: ${arch}
Maintainer: Tian Hao <thxdaemon@gmail.com>
Depends: ${depends}
Section: ${DEB_Section}
Priority: ${DEB_Priority}
Homepage: https://github.com/thdaemon/gshadowsocks
Description: A free GTK+ frontend of the shadowsocks client.
EOF

(cd build-package; dpkg-deb -b "${name}_deb" "${name}.deb")
rm -r build-package/${name}_deb

