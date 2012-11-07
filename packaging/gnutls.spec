#sbs-git:slp/pkgs/l/libgnutls26 gnutls 2.12.0 334e00aa49812702098c8de319a48e4a1bac6f02
Name:           gnutls
Version:        2.12.0
Release:        1
License:        LGPL-2.0+
Summary:        A TLS protocol implementation
Url:            http://www.gnutls.org/
Group:          System/Libraries
Source0:        %{name}-%{version}.tar.gz
BuildRequires:  autoconf
BuildRequires:  automake,
BuildRequires:  gettext-tools
BuildRequires:  libgcrypt-devel >= 1.2.2
BuildRequires:  libtasn1-devel
BuildRequires:  libtool,
BuildRequires:  lzo-devel,
BuildRequires:  readline-devel,
BuildRequires:  zlib-devel,

Requires:       libgcrypt >= 1.2.2
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%package devel
Summary:        Development files for the %{name} package
Group:          Development/Libraries
Requires:       %{name} = %{version}
Requires:       libgcrypt-devel
Requires:       pkgconfig
Requires(post): /sbin/install-info
Requires(preun): /sbin/install-info

%description
GnuTLS is a project that aims to develop a library which provides a secure
layer, over a reliable transport layer. Currently the GnuTLS library implements
the proposed standards by the IETF's TLS working group.

%description devel
GnuTLS is a project that aims to develop a library which provides a secure
layer, over a reliable transport layer. Currently the GnuTLS library implements
the proposed standards by the IETF's TLS working group.
This package contains files needed for developing applications with
the GnuTLS library.

%prep
%setup -q

%build

rm -f doc/*.info* lib/po/libgnutls26.pot
if [ -e doc/gnutls.pdf.debbackup ] && [ ! -e doc/gnutls.pdf ] ; then
	mv doc/gnutls.pdf.debbackup doc/gnutls.pdf ;
fi

%configure  --enable-ld-version-script --disable-cxx --without-lzo \
            --cache-file=config.cache --with-libgcrypt \
            --with-included-libtasn1
make

%install
rm -fr %{buildroot}
%make_install



%remove_docs

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/libgnutls*.so.*
%{_datadir}/locale/*/LC_MESSAGES/libgnutls26.mo

%files devel
%{_includedir}/*
%{_libdir}/libgnutls*.so
%{_libdir}/pkgconfig/*.pc
