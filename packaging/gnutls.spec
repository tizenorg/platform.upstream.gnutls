%define gnutls_sover 28
%define gnutlsxx_sover 28
%define gnutls_ossl_sover 27

Name:           gnutls
Version:        3.3.5
Release:        0
Summary:        The GNU Transport Layer Security Library
License:        LGPL-3.0+ or GPL-3.0+
Group:          Security/Crypto Libraries
Url:            http://www.gnutls.org/
Source0:        http://ftp.gnu.org/gnu/gnutls/%{name}-%{version}.tar.xz
Source1:        baselibs.conf
Source1001:     gnutls.manifest
BuildRequires:  automake
BuildRequires:  byacc
BuildRequires:  gcc-c++
BuildRequires:  gperf
BuildRequires:  libidn-devel
BuildRequires:  makeinfo
BuildRequires:  pkgconfig(nettle)
BuildRequires:  libtasn1-devel
BuildRequires:  gettext-tools
BuildRequires:  libtool
BuildRequires:  p11-kit-devel >= 0.11
BuildRequires:  pkg-config
BuildRequires:  xz
BuildRequires:  zlib-devel

Provides: gnutls-utils = %{version}-%{release}

%description
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.

%package -n libgnutls
Summary:        The GNU Transport Layer Security Library
License:        LGPL-3.0+
Group:          Security/Crypto Libraries

%description -n libgnutls
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.

%package -n libgnutlsxx
Summary:        The GNU Transport Layer Security Library
License:        LGPL-3.0+
Group:          Security/Crypto Libraries

%description -n libgnutlsxx
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.


%package -n libgnutls-openssl
Summary:        The GNU Transport Layer Security Library
License:        GPL-3.0+
Group:          Security/Crypto Libraries

%description -n libgnutls-openssl
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.


%package -n libgnutls-devel
Summary:        Development package for gnutls
License:        LGPL-3.0+
Group:          Development/Libraries
Requires:       glibc-devel
Requires:       libgnutls = %{version}
Provides:       gnutls-devel = %{version}-%{release}

%description -n libgnutls-devel
Files needed for software development using gnutls.

%package -n libgnutlsxx-devel
Summary:        Development package for gnutls
License:        LGPL-3.0+
Group:          Development/Libraries
Requires:       libgnutls-devel = %{version}
Requires:       libgnutlsxx = %{version}
Requires:       libstdc++-devel

%description -n libgnutlsxx-devel
Files needed for software development using gnutls.


%package -n libgnutls-openssl-devel
Summary:        Development package for gnutls
License:        GPL-3.0+
Group:          Development/Libraries
Requires:       libgnutls-devel = %{version}
Requires:       libgnutls-openssl = %{version}

%description -n libgnutls-openssl-devel
Files needed for software development using gnutls.


%prep
%setup -q
cp %{SOURCE1001} .
echo %{_includedir}/%{name}/abstract.h

%build
touch ChangeLog

%reconfigure \
        --disable-static \
        --with-pic \
        --disable-rpath \
        --disable-silent-rules \
        --with-default-trust-store-dir=/etc/ssl/certs \
        --with-sysroot=/%{?_sysroot}

%__make %{?_smp_mflags}

# 17-ago-2011, Test suite passes in factory, just not
#in the build system due to some broken code requiring both networking
#and fixes.
#make check

%install
%make_install
rm -rf doc/examples/.deps doc/examples/.libs doc/examples/*.{o,lo,la} doc/examples/Makefile{,.in}
find doc/examples -perm -111 -exec rm {} \;
rm -rf %{buildroot}%{_datadir}/locale/en@{,bold}quot
# Do not package static libs and libtool files
rm -f %{buildroot}%{_libdir}/*.la
%find_lang libgnutls --all-name

%post -n libgnutls -p /sbin/ldconfig

%postun -n libgnutls -p /sbin/ldconfig

%post -n libgnutlsxx -p /sbin/ldconfig

%postun -n libgnutlsxx -p /sbin/ldconfig

%post -n libgnutls-openssl -p /sbin/ldconfig

%postun -n libgnutls-openssl -p /sbin/ldconfig

%files -f libgnutls.lang
%manifest %{name}.manifest
%defattr(-, root, root)
%license COPYING
%{_bindir}/*tool
%{_bindir}/crywrap
%{_bindir}/gnutls-cli
%{_bindir}/gnutls-cli-debug
%{_bindir}/gnutls-serv
%{_mandir}/man1/*

%files -n libgnutls
%manifest %{name}.manifest
%defattr(-,root,root)
%license COPYING
%{_libdir}/libgnutls.so.*

%files -n libgnutls-openssl
%manifest %{name}.manifest
%defattr(-,root,root)
%{_libdir}/libgnutls-openssl.so.*

%files -n libgnutlsxx
%manifest %{name}.manifest
%defattr(-,root,root)
%{_libdir}/libgnutlsxx.so.*

%files -n libgnutls-devel
%manifest %{name}.manifest
%defattr(-, root, root)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_mandir}/man3/*
%{_infodir}/*.*
%doc doc/examples doc/gnutls.html doc/*.png doc/gnutls.pdf doc/reference/html/*

%files -n libgnutlsxx-devel
%manifest %{name}.manifest
%defattr(-, root, root)
%{_libdir}/libgnutlsxx.so
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/gnutlsxx.h

%files -n libgnutls-openssl-devel
%manifest %{name}.manifest
%defattr(-, root, root)
%{_libdir}/libgnutls-openssl.so
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/openssl.h
