pkgname=more-display
pkgver=0.1.0
pkgrel=1
pkgdesc="Add more external display"
arch=('x86_64')
url="https://yanoo.id"
license=('MIT')

# Runtime dependencies
depends=(
  'libdrm'
  'libevdi'
)

# Build-time  dependencies
makedepends=(
  'cmake'
  'gcc'
  'ninja'
)

# Optional but RECOMMENDED dependencies
optdepends=(
  'evdi-dkms: Kernel module for virtual display'
)

source=("more-display-${pkgver}.tar.gz")
sha256sums=('SKIP')

build() {
  cd "$srcdir/more-display-${pkgver}"
  cmake -B build -S .
  cmake --build build
}

package() {
  cd "$srcdir/more-display-${pkgver}"
  install -Dm755 build/more-display "$pkgdir/usr/bin/more-display"
  install -Dm644 etc/modules-load/evdi.conf "$pkgdir/etc/modules-load.d/evdi.conf"
}