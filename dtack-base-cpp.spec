Summary: 	Helper library with common cpp functions.
Name:    	dtack-base-cpp
Version: 4.3.2
Release:	1%{?dist}.maxlab
License: 	GPL
URL:		http://www.dtack.com
Group:		System Environment/Libraries
Source: 	%{name}-%{version}.tar.gz
Provides:	libdtack.base.so()(64bit)
Requires(post):	/sbin/ldconfig

%description
Helper library with some cpp functions used in Tango devices

%package devel
Summary:	Header files and libraries needed for %{name} development
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}

%description devel
Header files and shared libraries for developing programs using %{name}.

%prep
%setup -q

%build
make

%install
[ -z %{buildroot} ] || rm -rf %{buildroot}

## install libs
read MAJOR MINOR REVISION <<< $(echo %{version} | sed 's/\./ /g')
pushd lib > /dev/null
for f in *.so; do
	install -D -m644 $f %{buildroot}%{_libdir}/$f
done
popd > /dev/null

## install bins
pushd bin > /dev/null
install -D -m755 test_version %{buildroot}%{_bindir}/lib-dtack-base-test-00-version
popd > /dev/null

## install headers
pushd include > /dev/null
for f in *.h; do
	install -D -m644 $f %{buildroot}%{_includedir}/%{name}/$f
done
popd > /dev/null

%clean
[ -z %{buildroot} ] || rm -rf %{buildroot}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

# main package includes libraries and binaries
%files
%defattr (-,root,root,755)
%{_libdir}/*.so
%{_bindir}/*

%files devel
%defattr(-,root,root,755)
%{_libdir}/*.so
%{_includedir}/%{name}/*

%changelog
* Fri Jul 19 2019 David Erb initial package
- initial package
