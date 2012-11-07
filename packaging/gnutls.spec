%define gnutls_sover 28
%define gnutlsxx_sover 28
%define gnutls_ossl_sover 27

Name:           gnutls
Version:        3.0.21
Release:        0
Summary:        The GNU Transport Layer Security Library
License:        LGPL-3.0+ and GPL-3.0+
Group:          Productivity/Networking/Security
Url:            http://www.gnutls.org/
Source0:        http://ftp.gnu.org/gnu/gnutls/%{name}-%{version}.tar.xz
Source1:        baselibs.conf
BuildRequires:  automake
BuildRequires:  gcc-c++
BuildRequires:  libidn-devel
BuildRequires:  pkgconfig(nettle) 
BuildRequires:  libtasn1-devel
BuildRequires:  libtool
BuildRequires:  p11-kit-devel >= 0.11
BuildRequires:  pkg-config
BuildRequires:  xz
BuildRequires:  zlib-devel
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.

%package -n libgnutls
Summary:        The GNU Transport Layer Security Library
License:        LGPL-3.0+
Group:          Productivity/Networking/Security

%description -n libgnutls
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.

%package -n libgnutlsxx
Summary:        The GNU Transport Layer Security Library
License:        LGPL-3.0+
Group:          Productivity/Networking/Security

%description -n libgnutlsxx
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.


%package -n libgnutls-openssl
Summary:        The GNU Transport Layer Security Library
License:        GPL-3.0+
Group:          Productivity/Networking/Security

%description -n libgnutls-openssl
The GnuTLS project aims to develop a library that provides a secure
layer over a reliable transport layer. Currently the GnuTLS library
implements the proposed standards of the IETF's TLS working group.


%package -n libgnutls-devel
Summary:        Development package for gnutls
License:        LGPL-3.0+
Group:          Development/Libraries/C and C++
Requires:       glibc-devel
Requires:       libgnutls = %{version}
Provides:       gnutls-devel = %{version}-%{release}

%description -n libgnutls-devel
Files needed for software development using gnutls.

%package -n libgnutlsxx-devel
Summary:        Development package for gnutls
License:        LGPL-3.0+
Group:          Development/Libraries/C and C++
Requires:       libgnutls-devel = %{version}
Requires:       libgnutlsxx = %{version}
Requires:       libstdc++-devel

%description -n libgnutlsxx-devel
Files needed for software development using gnutls.


%package -n libgnutls-openssl-devel
Summary:        Development package for gnutls
License:        GPL-3.0+
Group:          Development/Libraries/C and C++
Requires:       libgnutls-devel = %{version}
Requires:       libgnutls-openssl = %{version}

%description -n libgnutls-openssl-devel
Files needed for software development using gnutls.


%prep
%setup -q
echo %{_includedir}/%{name}/abstract.h

%build
autoreconf -if
%configure \
        --disable-static \
        --with-pic \
        --disable-rpath \
        --disable-silent-rules \
	--with-default-trust-store-dir=/etc/ssl/certs \
        --with-sysroot=/%{?_sysroot}
make %{?_smp_mflags}

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

%clean
rm -rf %{buildroot}

%post -n libgnutls -p /sbin/ldconfig

%postun -n libgnutls -p /sbin/ldconfig

%post -n libgnutlsxx -p /sbin/ldconfig

%postun -n libgnutlsxx -p /sbin/ldconfig

%post -n libgnutls-openssl -p /sbin/ldconfig

%postun -n libgnutls-openssl -p /sbin/ldconfig

%files -f libgnutls.lang
%defattr(-, root, root)
%doc COPYING
%{_bindir}/certtool
%{_bindir}/crywrap
%{_bindir}/gnutls-cli
%{_bindir}/gnutls-cli-debug
%{_bindir}/gnutls-serv
%{_bindir}/ocsptool
%{_bindir}/psktool
%{_bindir}/p11tool
%{_bindir}/srptool
%{_mandir}/man1/*

%files -n libgnutls
%defattr(-,root,root)
%{_libdir}/libgnutls.so.*

%files -n libgnutls-openssl
%defattr(-,root,root)
%{_libdir}/libgnutls-openssl.so.*

%files -n libgnutlsxx
%defattr(-,root,root)
%{_libdir}/libgnutlsxx.so.*

%files -n libgnutls-devel
%defattr(-, root, root)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/abstract.h
%{_includedir}/%{name}/crypto.h
%{_includedir}/%{name}/compat.h
%{_includedir}/%{name}/dtls.h
%{_includedir}/%{name}/gnutls.h
%{_includedir}/%{name}/openpgp.h
%{_includedir}/%{name}/ocsp.h
%{_includedir}/%{name}/pkcs11.h
%{_includedir}/%{name}/pkcs12.h
%{_includedir}/%{name}/x509.h
%{_libdir}/libgnutls.so
%{_libdir}/pkgconfig/gnutls.pc
%{_mandir}/man3/*
%{_infodir}/*.*
%doc doc/examples doc/gnutls.html doc/*.png doc/gnutls.pdf doc/reference/html/*

%files -n libgnutlsxx-devel
%defattr(-, root, root)
%{_libdir}/libgnutlsxx.so
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/gnutlsxx.h

%files -n libgnutls-openssl-devel
%defattr(-, root, root)
%{_libdir}/libgnutls-openssl.so
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/openssl.h

%changelog
