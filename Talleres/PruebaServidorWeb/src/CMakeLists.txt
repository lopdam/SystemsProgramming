include(CheckCSourceCompiles)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckVariableExists)
include(CheckTypeSize)
include(CheckLibraryExists)
include(CMakeDetermineCCompiler)
include(FindThreads)
include(FindPkgConfig)

include(LighttpdMacros)

add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES)

# default to ON
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "Default value for ``POSITION_INDEPENDENT_CODE`` of targets.")

option(WITH_XATTR "with xattr-support for the stat-cache [default: off]")
option(WITH_MYSQL "with mysql-support for mod_vhostdb_mysql [default: off]")
option(WITH_PGSQL "with postgres-support for mod_vhostdb_pgsql [default: off]")
option(WITH_DBI "with dbi-support for mod_vhostdb_dbi [default: off]")
option(WITH_OPENSSL "with openssl-support [default: off]")
option(WITH_WOLFSSL "with wolfSSL-support [default: off]")
option(WITH_PCRE "with regex support [default: on]" ON)
option(WITH_WEBDAV_PROPS "with property-support for mod_webdav [default: off]")
option(WITH_WEBDAV_LOCKS "locks in webdav [default: off]")
option(WITH_BZIP "with bzip2-support for mod_compress [default: off]")
option(WITH_ZLIB "with deflate-support for mod_compress [default: on]" ON)
option(WITH_KRB5 "with Kerberos5-support for mod_auth [default: off]")
option(WITH_LDAP "with LDAP-support for mod_auth mod_vhostdb_ldap [default: off]")
option(WITH_PAM "with PAM-support for mod_auth [default: off]")
option(WITH_LUA "with lua 5.1 for mod_magnet [default: off]")
# option(WITH_VALGRIND "with internal support for valgrind [default: off]")
option(WITH_FAM "fam/gamin for reducing number of stat() calls [default: off]")
option(WITH_GDBM "gdbm storage for mod_trigger_b4_dl [default: off]")
option(WITH_MEMCACHED "memcached storage for mod_trigger_b4_dl [default: off]")
option(WITH_LIBEV "libev support for fdevent handlers [default: off]")
option(WITH_LIBUNWIND "with libunwind to print backtraces in asserts [default: off]")
option(WITH_GEOIP "with GeoIP-support mod_geoip [default: off]")
option(WITH_MAXMINDDB "with MaxMind GeoIP2-support mod_maxminddb [default: off]")
option(WITH_SASL "with SASL-support for mod_authn_sasl [default: off]")

if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
	option(BUILD_EXTRA_WARNINGS "extra warnings")

	if(BUILD_EXTRA_WARNINGS)
		set(WARN_CFLAGS "-g -g2 -Wall -Wmissing-declarations -Wcast-align -Wsign-compare -Wnested-externs -Wpointer-arith -D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security")
		set(WARN_LDFLAGS "-Wl,--as-needed")
		# -Werror -Wbad-function-cast -Wmissing-prototypes
	endif()
endif()

option(BUILD_STATIC "build a static lighttpd with all modules added")

if(BUILD_STATIC)
	set(LIGHTTPD_STATIC 1)
elseif(APPLE)
	set(CMAKE_SHARED_MODULE_PREFIX "")
else()
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

if(WITH_LIBEV)
	find_package(LibEV REQUIRED)
	set(HAVE_LIBEV 1)
endif()

if(WITH_LIBUNWIND)
	pkg_check_modules(LIBUNWIND REQUIRED libunwind)
	set(HAVE_LIBUNWIND 1)
endif()

if(WITH_WEBDAV_PROPS)
	set(WITH_XML 1)
	set(WITH_SQLITE3 1)
endif()

if(WITH_WEBDAV_LOCKS)
	set(WITH_UUID 1)
endif()

check_include_files(sys/devpoll.h HAVE_SYS_DEVPOLL_H)
check_include_files(sys/epoll.h HAVE_SYS_EPOLL_H)
set(CMAKE_REQUIRED_FLAGS "-include sys/types.h")
check_include_files(sys/event.h HAVE_SYS_EVENT_H)
set(CMAKE_REQUIRED_FLAGS)
check_include_files(sys/mman.h HAVE_SYS_MMAN_H)
check_include_files(sys/poll.h HAVE_SYS_POLL_H)
check_include_files(sys/port.h HAVE_SYS_PORT_H)
check_include_files(sys/prctl.h HAVE_SYS_PRCTL_H)
check_include_files(sys/resource.h HAVE_SYS_RESOURCE_H)
check_include_files(sys/sendfile.h HAVE_SYS_SENDFILE_H)
check_include_files(sys/select.h HAVE_SYS_SELECT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(sys/uio.h HAVE_SYS_UIO_H)
check_include_files(sys/un.h HAVE_SYS_UN_H)
check_include_files(sys/wait.h HAVE_SYS_WAIT_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(pthread.h HAVE_PTHREAD_H)
check_include_files(getopt.h HAVE_GETOPT_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(poll.h HAVE_POLL_H)
check_include_files(pwd.h HAVE_PWD_H)
check_include_files(stddef.h HAVE_STDDEF_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(syslog.h HAVE_SYSLOG_H)

# check for fastcgi lib, for the tests only
check_include_files(fastcgi.h HAVE_FASTCGI_H)
check_include_files(fastcgi/fastcgi.h HAVE_FASTCGI_FASTCGI_H)

# will be needed for auth
check_include_files(crypt.h HAVE_CRYPT_H)
# check if we need libcrypt for crypt_r()
check_library_exists(crypt crypt_r "" HAVE_LIBCRYPT_CRYPT_R)
if(HAVE_LIBCRYPT_CRYPT_R)
	set(HAVE_CRYPT_R 1)
	set(HAVE_LIBCRYPT 1)
else()
	check_library_exists(crypt crypt "" HAVE_LIBCRYPT)
endif()
check_function_exists(crypt_r HAVE_CRYPT_R)
check_function_exists(crypt HAVE_CRYPT)

check_include_files(sys/inotify.h HAVE_SYS_INOTIFY_H)
if(HAVE_SYS_INOTIFY_H)
	check_function_exists(inotify_init HAVE_INOTIFY_INIT)
endif()

set(CMAKE_EXTRA_INCLUDE_FILES sys/socket.h)
check_type_size(socklen_t HAVE_SOCKLEN_T)
set(CMAKE_EXTRA_INCLUDE_FILES)

check_include_files(sys/random.h HAVE_SYS_RANDOM_H)
set(CMAKE_EXTRA_INCLUDE_FILES sys/random.h)
check_function_exists(getentropy HAVE_GETENTROPY)
set(CMAKE_EXTRA_INCLUDE_FILES)

check_include_files(linux/random.h HAVE_LINUX_RANDOM_H)
set(CMAKE_EXTRA_INCLUDE_FILES linux/random.h)
check_function_exists(getrandom HAVE_GETRANDOM)
set(CMAKE_EXTRA_INCLUDE_FILES)

check_type_size(long SIZEOF_LONG)
check_type_size(off_t SIZEOF_OFF_T)

check_function_exists(arc4random_buf HAVE_ARC4RANDOM_BUF)
check_function_exists(chroot HAVE_CHROOT)
check_function_exists(epoll_ctl HAVE_EPOLL_CTL)
check_function_exists(fork HAVE_FORK)
check_function_exists(getloadavg HAVE_GETLOADAVG)
check_function_exists(getrlimit HAVE_GETRLIMIT)
check_function_exists(getuid HAVE_GETUID)
check_function_exists(gmtime_r HAVE_GMTIME_R)
check_function_exists(inet_ntop HAVE_INET_NTOP)
check_function_exists(jrand48 HAVE_JRAND48)
check_function_exists(kqueue HAVE_KQUEUE)
check_function_exists(localtime_r HAVE_LOCALTIME_R)
check_function_exists(lstat HAVE_LSTAT)
check_function_exists(madvise HAVE_MADVISE)
check_function_exists(memcpy HAVE_MEMCPY)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(pathconf HAVE_PATHCONF)
check_function_exists(pipe2 HAVE_PIPE2)
check_function_exists(poll HAVE_POLL)
check_function_exists(port_create HAVE_PORT_CREATE)
check_function_exists(prctl HAVE_PRCTL)
check_function_exists(pread HAVE_PREAD)
check_function_exists(posix_fadvise HAVE_POSIX_FADVISE)
check_function_exists(select HAVE_SELECT)
check_function_exists(sendfile HAVE_SENDFILE)
check_function_exists(send_file HAVE_SEND_FILE)
check_function_exists(sendfile64 HAVE_SENDFILE64)
check_function_exists(sendfilev HAVE_SENDFILEV)
check_function_exists(sigaction HAVE_SIGACTION)
check_function_exists(signal HAVE_SIGNAL)
check_function_exists(sigtimedwait HAVE_SIGTIMEDWAIT)
check_function_exists(srandom HAVE_SRANDOM)
check_function_exists(strptime HAVE_STRPTIME)
check_function_exists(syslog HAVE_SYSLOG)
check_function_exists(writev HAVE_WRITEV)
check_function_exists(inet_aton HAVE_INET_ATON)
check_function_exists(issetugid HAVE_ISSETUGID)
check_function_exists(inet_pton HAVE_INET_PTON)
check_function_exists(memset_s HAVE_MEMSET_S)
check_function_exists(explicit_bzero HAVE_EXPLICIT_BZERO)
check_function_exists(explicit_memset HAVE_EXPLICIT_MEMSET)
check_symbol_exists(clock_gettime "time.h" HAVE_CLOCK_GETTIME)
if (NOT HAVE_CLOCK_GETTIME)
	check_library_exists(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)
endif()
check_library_exists(elftc elftc_copyfile "libelftc.h" HAVE_ELFTC_COPYFILE)
check_c_source_compiles("
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>

	int main() {
		struct sockaddr_in6 s; struct in6_addr t=in6addr_any; int i=AF_INET6; s; t.s6_addr[0] = 0;
		return 0;
	}" HAVE_IPV6)
check_c_source_compiles("
	__attribute__((weak)) void __dummy(void *x) { }
	int main() {
		void *x;
		__dummy(x);
	}
	" HAVE_WEAK_SYMBOLS)
check_c_source_compiles("
	#include <time.h>
	int main(void) {
		struct tm t;
		t.tm_gmtoff = 0;
		return 0;
	}
	" HAVE_STRUCT_TM_GMTOFF)

## refactor me
macro(XCONFIG _package _include_DIR _link_DIR _link_FLAGS _cflags)
# reset the variables at the beginning
	set(${_include_DIR})
	set(${_link_DIR})
	set(${_link_FLAGS})
	set(${_cflags})

	find_program(${_package}CONFIG_EXECUTABLE NAMES ${_package} PATHS /usr/local/bin )

	# if pkg-config has been found
	if(${_package}CONFIG_EXECUTABLE)
		set(XCONFIG_EXECUTABLE "${${_package}CONFIG_EXECUTABLE}")
		message(STATUS "found ${_package}: ${XCONFIG_EXECUTABLE}")

		exec_program(${XCONFIG_EXECUTABLE} ARGS --libs OUTPUT_VARIABLE __link_FLAGS)
		string(REPLACE "\n" "" ${_link_FLAGS} ${__link_FLAGS})
		exec_program(${XCONFIG_EXECUTABLE} ARGS --cflags OUTPUT_VARIABLE __cflags)
		string(REPLACE "\n" "" ${_cflags} ${__cflags})
	else()
		message(STATUS "found ${_package}: no")
	endif()
endmacro(XCONFIG _package _include_DIR _link_DIR _link_FLAGS _cflags)

if(WITH_XATTR)
	check_include_files("sys/types.h;attr/attributes.h" HAVE_ATTR_ATTRIBUTES_H)
	if(HAVE_ATTR_ATTRIBUTES_H)
		check_library_exists(attr attr_get "" HAVE_XATTR)
	endif()
else()
	unset(HAVE_ATTR_ATTRIBUTES_H)
	unset(HAVE_XATTR)
endif()

if(WITH_MYSQL)
	xconfig(mysql_config MYSQL_INCDIR MYSQL_LIBDIR MYSQL_LDFLAGS MYSQL_CFLAGS)

	set(CMAKE_REQUIRED_INCLUDES /usr/include/mysql)
	check_include_files(mysql.h HAVE_MYSQL_H)
	set(CMAKE_REQUIRED_INCLUDES)
	if(HAVE_MYSQL_H)
		check_library_exists(mysqlclient mysql_real_connect "" HAVE_MYSQL)
	endif()
else()
	unset(HAVE_MYSQL_H)
	unset(HAVE_MYSQL)
endif()

if(WITH_PGSQL)
	xconfig(pg_config PGSQL_INCDIR PGSQL_LIBDIR PGSQL_LDFLAGS PGSQL_CFLAGS)

	check_include_files(libpq-fe.h HAVE_PGSQL_H)
	if(HAVE_PGSQL_H)
		check_library_exists(pq PQsetdbLogin "" HAVE_PGSQL)
	endif()
else()
	unset(HAVE_PGSQL_H)
	unset(HAVE_PGSQL)
endif()

if(WITH_DBI)
	check_include_files(dbi/dbi.h HAVE_DBI_H)
	if(HAVE_DBI_H)
		check_library_exists(dbi dbi_conn_connect "" HAVE_DBI)
	endif()
else()
	unset(HAVE_DBI_H)
	unset(HAVE_DBI)
endif()

set(CRYPTO_LIBRARY "")

if(WITH_OPENSSL)
	if(APPLE)
		set(CMAKE_REQUIRED_INCLUDES /opt/local/include)
	endif()
	check_include_files(openssl/ssl.h HAVE_OPENSSL_SSL_H)
	if(APPLE)
		set(CMAKE_REQUIRED_INCLUDES)
	endif()
	if(HAVE_OPENSSL_SSL_H)
		check_library_exists(crypto BIO_f_base64 "" HAVE_LIBCRYPTO)
		if(HAVE_LIBCRYPTO)
			set(CRYPTO_LIBRARY crypto)
			check_library_exists(ssl SSL_new "" HAVE_LIBSSL)
		endif()
	endif()
else()
	unset(HAVE_OPENSSL_SSL_H)
	unset(HAVE_LIBSSL)
endif()

if(WITH_WOLFSSL)
	if(NOT ${WITH_WOLFSSL} EQUAL "")
		find_path(WOLFSSL_INCLUDE_DIR wolfssl/ssl.h ${WITH_WOLFSSL}/include)
	elseif(APPLE)
		find_path(WOLFSSL_INCLUDE_DIR wolfssl/ssl.h /opt/local/include)
	else()
		find_path(WOLFSSL_INCLUDE_DIR wolfssl/ssl.h /usr/local/include)
	endif()

	if(NOT ${WITH_WOLFSSL} EQUAL "")
		find_library(WOLFSSL_LIBRARY
			NAMES wolfssl
			PATHS ${WITH_WOLFSSL}/lib
		)
	else()
		find_library(WOLFSSL_LIBRARY
			NAMES wolfssl
			PATHS /usr/local/lib /opt/local/lib /usr/lib
		)
	endif()

	if(WOLFSSL_INCLUDE_DIR AND WOLFSSL_LIBRARY)
		set(CMAKE_REQUIRED_INCLUDES ${WOLFSSL_INCLUDE_DIR})
		check_include_files(wolfssl/ssl.h HAVE_WOLFSSL_SSL_H)

		check_c_source_compiles("
		#include <wolfssl/options.h>
		#if !defined(HAVE_LIGHTY) && !defined(OPENSSL_ALL)
		#error HAVE_LIGHTY macro not defined
		#endif
		int main() { return 0; }
		" CHECK_HAVE_LIGHTY)
		if (NOT CHECK_HAVE_LIGHTY)
			message(FATAL_ERROR "wolfssl must be built with ./configure --enable-lighty")
		endif()
		unset(CHECK_HAVE_LIGHTY)

		set(CMAKE_REQUIRED_LIBRARIES ${WOLFSSL_LIBRARY})
		check_library_exists(${WOLFSSL_LIBRARY} wolfSSL_Init "" HAVE_LIBCRYPTO)
		if(HAVE_LIBCRYPTO)
			set(CRYPTO_LIBRARY ${WOLFSSL_LIBRARY})
			add_definitions(-DHAVE_WOLFSSL_SSL_H)
		endif()
		set(CMAKE_REQUIRED_INCLUDES)
		set(CMAKE_REQUIRED_LIBRARIES)
		include_directories(${WOLFSSL_INCLUDE_DIR} ${WOLFSSL_INCLUDE_DIR}/wolfssl)
	endif()
	unset(WOLFSSL_LIBRARY)
	unset(WOLFSSL_INCLUDE_DIR)
else()
	unset(HAVE_WOLFSSL_SSL_H)
endif()

if(WITH_OPENSSL AND WITH_WOLFSSL)
	message(FATAL_ERROR "lighttpd should not be built with both --with-openssl and --with-wolfssl")
endif()

if(WITH_PCRE)
	## if we have pcre-config, use it
	xconfig(pcre-config PCRE_INCDIR PCRE_LIBDIR PCRE_LDFLAGS PCRE_CFLAGS)
	if(PCRE_LDFLAGS OR PCRE_CFLAGS)
		message(STATUS "found pcre at: LDFLAGS: ${PCRE_LDFLAGS} CFLAGS: ${PCRE_CFLAGS}")

		if(NOT PCRE_CFLAGS STREQUAL "\n")
			## if it is empty we'll get newline returned
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PCRE_CFLAGS}")
		endif()

		set(HAVE_PCRE_H 1)
		set(HAVE_LIBPCRE 1)
	else()
		if(NOT WIN32)
			check_include_files(pcre.h HAVE_PCRE_H)
			check_library_exists(pcre pcre_exec "" HAVE_LIBPCRE)
			set(PCRE_LDFLAGS -lpcre)
		else()
			find_path(PCRE_INCLUDE_DIR pcre.h
			/usr/local/include
			/usr/include
			)

			set(PCRE_NAMES pcre)
			find_library(PCRE_LIBRARY
			NAMES ${PCRE_NAMES}
			PATHS /usr/lib /usr/local/lib
			)

			if(PCRE_INCLUDE_DIR AND PCRE_LIBRARY)
				set(CMAKE_REQUIRED_INCLUDES ${PCRE_INCLUDE_DIR})
				set(CMAKE_REQUIRED_LIBRARIES ${PCRE_LIBRARY})
				check_include_files(pcre.h HAVE_PCRE_H)
				check_library_exists(pcre pcre_exec "" HAVE_LIBPCRE)
				set(CMAKE_REQUIRED_INCLUDES)
				set(CMAKE_REQUIRED_LIBRARIES)
				include_directories(${PCRE_INCLUDE_DIR})
			endif()
		endif()
	endif()

	if(NOT HAVE_PCRE_H)
		message(FATAL_ERROR "pcre.h couldn't be found")
	endif()
	if(NOT HAVE_LIBPCRE)
		message(FATAL_ERROR "libpcre couldn't be found")
	endif()
else()
	unset(HAVE_PCRE_H)
	unset(HAVE_LIBPCRE)
endif()

if(WITH_SASL)
	check_include_files(sasl/sasl.h HAVE_SASL_SASL_H)
	if(HAVE_SASL_SASL_H)
		check_library_exists(sasl2 sasl_server_init "" HAVE_SASL)
	endif()
else()
	unset(HAVE_SASL_SASL_H)
	unset(HAVE_SASL)
endif()


if(WITH_XML)
	xconfig(xml2-config XML2_INCDIR XML2_LIBDIR XML2_LDFLAGS XML2_CFLAGS)
	if(XML2_LDFLAGS OR XML2_CFLAGS)
		message(STATUS "found xml2 at: LDFLAGS: ${XML2_LDFLAGS} CFLAGS: ${XML2_CFLAGS}")

		## if it is empty we'll get newline returned
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${XML2_CFLAGS}")

		check_include_files(libxml/tree.h HAVE_LIBXML_H)

		set(CMAKE_REQUIRED_FLAGS ${XML2_LDFLAGS})
		check_library_exists(xml2 xmlParseChunk "" HAVE_LIBXML)
		set(CMAKE_REQUIRED_FLAGS)
	else()
		check_include_files(libxml.h HAVE_LIBXML_H)
		check_library_exists(xml2 xmlParseChunk "" HAVE_LIBXML)
	endif()

	if(NOT HAVE_LIBXML_H)
		message(FATAL_ERROR "libxml/tree.h couldn't be found")
	endif()
	if(NOT HAVE_LIBXML)
		message(FATAL_ERROR "libxml2 couldn't be found")
	endif()
else()
	unset(HAVE_LIBXML_H)
	unset(HAVE_LIBXML)
endif()

if(WITH_SQLITE3)
	check_include_files(sqlite3.h HAVE_SQLITE3_H)
	check_library_exists(sqlite3 sqlite3_reset "" HAVE_SQLITE3)
else()
	unset(HAVE_SQLITE3_H)
	unset(HAVE_SQLITE3)
endif()

if(WITH_UUID)
	check_include_files(uuid/uuid.h HAVE_UUID_UUID_H)
	check_library_exists(uuid uuid_generate "" NEED_LIBUUID)
	if(NOT NEED_LIBUUID)
		check_function_exists(uuid_generate HAVE_LIBUUID)
	else()
		set(HAVE_LIBUUID 1)
	endif()
else()
	unset(HAVE_UUID_UUID_H)
	unset(NEED_LIBUUID)
	unset(HAVE_LIBUUID)
endif()

if(WITH_ZLIB)
	if(NOT WIN32)
		check_include_files(zlib.h HAVE_ZLIB_H)
		check_library_exists(z deflate "" HAVE_LIBZ)
		set(ZLIB_LIBRARY z)
	else()
		find_path(ZLIB_INCLUDE_DIR zlib.h
			/usr/local/include
			/usr/include
		)

		set(ZLIB_NAMES z zlib zdll)
			find_library(ZLIB_LIBRARY
			NAMES ${ZLIB_NAMES}
			PATHS /usr/lib /usr/local/lib
		)

		if(ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
			set(CMAKE_REQUIRED_INCLUDES ${ZLIB_INCLUDE_DIR})
			set(CMAKE_REQUIRED_LIBRARIES ${ZLIB_LIBRARY})
			get_filename_component(ZLIB_NAME ${ZLIB_LIBRARY} NAME)
			check_include_files(zlib.h HAVE_ZLIB_H)
			check_library_exists(${ZLIB_NAME} deflate "" HAVE_LIBZ)
			set(CMAKE_REQUIRED_INCLUDES)
			set(CMAKE_REQUIRED_LIBRARIES)
			include_directories(${ZLIB_INCLUDE_DIR})
		endif()
	endif()
else()
	unset(HAVE_ZLIB_H)
	unset(HAVE_LIBZ)
	unset(ZLIB_INCLUDE_DIR)
	unset(ZLIB_LIBRARY)
endif()

if(WITH_BZIP)
	check_include_files(bzlib.h HAVE_BZLIB_H)
	check_library_exists(bz2 BZ2_bzCompress "" HAVE_LIBBZ2)
else()
	unset(HAVE_BZLIB_H)
	unset(HAVE_LIBBZ2)
endif()

if(WITH_LDAP)
	check_include_files(ldap.h HAVE_LDAP_H)
	check_library_exists(ldap ldap_bind "" HAVE_LIBLDAP)
	check_include_files(lber.h HAVE_LBER_H)
	check_library_exists(lber ber_printf "" HAVE_LIBLBER)
else()
	unset(HAVE_LDAP_H)
	unset(HAVE_LIBLDAP)
	unset(HAVE_LBER_H)
	unset(HAVE_LIBLBER)
endif()

if(WITH_PAM)
	check_include_files(security/pam_appl.h HAVE_SECURITY_PAM_APPL_H)
	check_library_exists(pam pam_start "" HAVE_PAM)
else()
	unset(HAVE_SECURITY_PAM_APPL_H)
	unset(HAVE_PAM)
endif()

if(WITH_LUA)
	pkg_search_module(LUA REQUIRED lua5.3 lua-5.3 lua5.2 lua-5.2 lua5.1 lua-5.1 lua)
	message(STATUS "found lua at: INCDIR: ${LUA_INCLUDE_DIRS} LIBDIR: ${LUA_LIBRARY_DIRS} LDFLAGS: ${LUA_LDFLAGS} CFLAGS: ${LUA_CFLAGS}")
	set(HAVE_LIBLUA 1 "Have liblua")
	set(HAVE_LUA_H  1 "Have liblua header")
else()
	unset(HAVE_LIBLUA)
	unset(HAVE_LUA_H)
endif()

if(WITH_FAM)
	check_include_files(fam.h HAVE_FAM_H)
	check_library_exists(fam FAMOpen2 "" HAVE_LIBFAM)
	if(HAVE_LIBFAM)
		set(CMAKE_REQUIRED_LIBRARIES fam)
		check_function_exists(FAMNoExists HAVE_FAMNOEXISTS)
	endif()
else()
	unset(HAVE_FAM_H)
	unset(HAVE_LIBFAM)
	unset(HAVE_FAMNOEXISTS)
endif()

if(WITH_GDBM)
	check_include_files(gdbm.h HAVE_GDBM_H)
	check_library_exists(gdbm gdbm_open "" HAVE_GDBM)
else()
	unset(HAVE_GDBM_H)
	unset(HAVE_GDBM)
endif()

if(WITH_MEMCACHED)
	check_include_files(libmemcached/memcached.h HAVE_LIBMEMCACHED_MEMCACHED_H)
	check_library_exists(memcached memcached "" HAVE_LIBMEMCACHED)
	if(HAVE_LIBMEMCACHED_MEMCACHED_H AND HAVE_LIBMEMCACHED)
		set(USE_MEMCACHED 1)
	else()
		message(FATAL_ERROR "didn't find libmemcached")
	endif()
endif()

if(WITH_GEOIP)
	check_library_exists(geoip GeoIP_country_name_by_addr "" HAVE_GEOIP)
endif()

if(WITH_MAXMINDDB)
	check_library_exists(maxminddb MMDB_open "" HAVE_MAXMINDDB)
endif()

if(NOT BUILD_STATIC)
	check_include_files(dlfcn.h HAVE_DLFCN_H)
else()
	unset(HAVE_DLFCN_H)
endif()

if(HAVE_DLFCN_H)
	check_library_exists(dl dlopen "" HAVE_LIBDL)
else()
	unset(HAVE_LIBDL)
endif()

set(LIGHTTPD_VERSION_ID 10400)
set(PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")

if(NOT SBINDIR)
	set(SBINDIR "sbin")
endif()

if(NOT LIGHTTPD_MODULES_DIR)
	set(LIGHTTPD_MODULES_DIR "lib${LIB_SUFFIX}/lighttpd")
endif()

if(NOT WIN32)
	set(LIGHTTPD_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/${LIGHTTPD_MODULES_DIR}")
else()
	## We use relative path in windows
	set(LIGHTTPD_LIBRARY_DIR "lib")
endif()

## Write out config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_definitions(-DHAVE_CONFIG_H)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(COMMON_SRC
	base64.c buffer.c burl.c log.c
	http_header.c http_kv.c keyvalue.c chunk.c
	http_chunk.c stream.c fdevent.c gw_backend.c
	stat_cache.c plugin.c joblist.c etag.c array.c
	data_string.c data_array.c
	data_integer.c algo_sha1.c md5.c
	vector.c
	fdevent_select.c fdevent_libev.c
	fdevent_poll.c fdevent_linux_sysepoll.c
	fdevent_solaris_devpoll.c fdevent_solaris_port.c
	fdevent_freebsd_kqueue.c
	data_config.c
	crc32.c
	connections-glue.c
	configfile-glue.c
	http-header-glue.c
	http_auth.c
	http_vhostdb.c
	request.c
	sock_addr.c
	splaytree.c
	rand.c
	safe_memclear.c
)

if(WIN32)
	message(STATUS "Adding local getopt implementation.")
	set(COMMON_SRC ${COMMON_SRC} xgetopt.c)
endif()

add_executable(lemon lemon.c)

## Build parsers by using lemon...
lemon_parser(configparser.y)
lemon_parser(mod_ssi_exprparser.y)

set(L_INSTALL_TARGETS)

add_executable(lighttpd-angel lighttpd-angel.c)
set(L_INSTALL_TARGETS ${L_INSTALL_TARGETS} lighttpd-angel)
add_target_properties(lighttpd-angel COMPILE_FLAGS "-DSBIN_DIR=\\\\\"${CMAKE_INSTALL_PREFIX}/${SBINDIR}\\\\\"")

add_executable(lighttpd
	server.c
	response.c
	connections.c
	inet_ntop_cache.c
	network.c
	network_write.c
	configfile.c
	configparser.c
	${COMMON_SRC}
)
set(L_INSTALL_TARGETS ${L_INSTALL_TARGETS} lighttpd)

add_and_install_library(mod_access mod_access.c)
add_and_install_library(mod_accesslog mod_accesslog.c)
add_and_install_library(mod_alias mod_alias.c)
add_and_install_library(mod_auth "mod_auth.c")
add_and_install_library(mod_authn_file "mod_authn_file.c")
if(NOT WIN32)
	add_and_install_library(mod_cgi mod_cgi.c)
endif()
add_and_install_library(mod_compress mod_compress.c)
add_and_install_library(mod_deflate mod_deflate.c)
add_and_install_library(mod_dirlisting mod_dirlisting.c)
add_and_install_library(mod_evasive mod_evasive.c)
add_and_install_library(mod_evhost mod_evhost.c)
add_and_install_library(mod_expire mod_expire.c)
add_and_install_library(mod_extforward mod_extforward.c)
add_and_install_library(mod_fastcgi mod_fastcgi.c)
add_and_install_library(mod_flv_streaming mod_flv_streaming.c)
add_and_install_library(mod_indexfile mod_indexfile.c)
add_and_install_library(mod_proxy mod_proxy.c)
add_and_install_library(mod_redirect mod_redirect.c)
add_and_install_library(mod_rewrite mod_rewrite.c)
add_and_install_library(mod_rrdtool mod_rrdtool.c)
add_and_install_library(mod_scgi mod_scgi.c)
add_and_install_library(mod_secdownload mod_secdownload.c)
add_and_install_library(mod_setenv mod_setenv.c)
add_and_install_library(mod_simple_vhost mod_simple_vhost.c)
add_and_install_library(mod_sockproxy mod_sockproxy.c)
add_and_install_library(mod_ssi "mod_ssi_exprparser.c;mod_ssi_expr.c;mod_ssi.c")
add_and_install_library(mod_staticfile mod_staticfile.c)
add_and_install_library(mod_status mod_status.c)
add_and_install_library(mod_uploadprogress mod_uploadprogress.c)
add_and_install_library(mod_userdir mod_userdir.c)
add_and_install_library(mod_usertrack mod_usertrack.c)
add_and_install_library(mod_vhostdb mod_vhostdb.c)
add_and_install_library(mod_webdav mod_webdav.c)
add_and_install_library(mod_wstunnel mod_wstunnel.c)

add_executable(test_array
	t/test_array.c
	array.c
	data_array.c
	data_integer.c
	data_string.c
	buffer.c
)
add_test(NAME test_array COMMAND test_array)

add_executable(test_buffer
	t/test_buffer.c
	buffer.c
)
add_test(NAME test_buffer COMMAND test_buffer)

add_executable(test_burl
	t/test_burl.c
	burl.c
	buffer.c
	base64.c
)
add_test(NAME test_burl COMMAND test_burl)

add_executable(test_base64
	t/test_base64.c
	buffer.c
	base64.c
)
add_test(NAME test_base64 COMMAND test_base64)

add_executable(test_configfile
	t/test_configfile.c
	buffer.c
	array.c
	data_config.c
	data_integer.c
	data_string.c
	http_header.c
	http_kv.c
	vector.c
	log.c
	sock_addr.c
)
add_test(NAME test_configfile COMMAND test_configfile)

add_executable(test_keyvalue
	t/test_keyvalue.c
	burl.c
	buffer.c
	base64.c
	array.c
	data_integer.c
	data_string.c
	log.c
)
add_test(NAME test_keyvalue COMMAND test_keyvalue)

add_executable(test_mod_access
	t/test_mod_access.c
	configfile-glue.c
	buffer.c
	array.c
	data_config.c
	data_integer.c
	data_string.c
	http_header.c
	http_kv.c
	vector.c
	log.c
	sock_addr.c
)
add_test(NAME test_mod_access COMMAND test_mod_access)

add_executable(test_mod_evhost
	t/test_mod_evhost.c
	configfile-glue.c
	buffer.c
	array.c
	data_config.c
	data_integer.c
	data_string.c
	http_header.c
	http_kv.c
	vector.c
	log.c
	sock_addr.c
)
add_test(NAME test_mod_evhost COMMAND test_mod_evhost)

add_executable(test_mod_simple_vhost
	t/test_mod_simple_vhost.c
	configfile-glue.c
	buffer.c
	array.c
	data_config.c
	data_integer.c
	data_string.c
	http_header.c
	http_kv.c
	vector.c
	log.c
	sock_addr.c
)
add_test(NAME test_mod_simple_vhost COMMAND test_mod_simple_vhost)

add_executable(test_request
	t/test_request.c
	request.c
	buffer.c
	array.c
	data_integer.c
	data_string.c
	http_header.c
	http_kv.c
	log.c
	sock_addr.c
)
add_test(NAME test_request COMMAND test_request)

if(HAVE_PCRE_H)
	target_link_libraries(lighttpd ${PCRE_LDFLAGS})
	add_target_properties(lighttpd COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(mod_rewrite ${PCRE_LDFLAGS})
	add_target_properties(mod_rewrite COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(mod_dirlisting ${PCRE_LDFLAGS})
	add_target_properties(mod_dirlisting COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(mod_redirect ${PCRE_LDFLAGS})
	add_target_properties(mod_redirect COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(test_configfile ${PCRE_LDFLAGS})
	add_target_properties(test_configfile COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(test_keyvalue ${PCRE_LDFLAGS})
	add_target_properties(test_keyvalue COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(test_mod_access ${PCRE_LDFLAGS})
	add_target_properties(test_mod_access COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(test_mod_evhost ${PCRE_LDFLAGS})
	add_target_properties(test_mod_evhost COMPILE_FLAGS ${PCRE_CFLAGS})
	target_link_libraries(test_mod_simple_vhost ${PCRE_LDFLAGS})
	add_target_properties(test_mod_simple_vhost COMPILE_FLAGS ${PCRE_CFLAGS})
endif()

if(WITH_PCRE AND (WITH_MEMCACHED OR WITH_GDBM))
	add_and_install_library(mod_trigger_b4_dl mod_trigger_b4_dl.c)
	target_link_libraries(mod_trigger_b4_dl ${PCRE_LDFLAGS})
	add_target_properties(mod_trigger_b4_dl COMPILE_FLAGS ${PCRE_CFLAGS})
endif()

if(WITH_LUA)
	add_and_install_library(mod_magnet "mod_magnet.c;mod_magnet_cache.c")
	target_link_libraries(mod_magnet ${LUA_LDFLAGS})
	add_target_properties(mod_magnet COMPILE_FLAGS ${LUA_CFLAGS})

	add_and_install_library(mod_cml "mod_cml.c;mod_cml_lua.c;mod_cml_funcs.c")
	target_link_libraries(mod_cml ${LUA_LDFLAGS})
	add_target_properties(mod_cml COMPILE_FLAGS ${LUA_CFLAGS})
	if(WITH_MEMCACHED)
		target_link_libraries(mod_cml memcached)
	endif()
endif()

if(WITH_GEOIP)
	add_and_install_library(mod_geoip mod_geoip.c)
	target_link_libraries(mod_geoip GeoIP)
endif()

if(WITH_MAXMINDDB)
	add_and_install_library(mod_maxminddb mod_maxminddb.c)
	target_link_libraries(mod_maxminddb maxminddb)
endif()

if(HAVE_MYSQL_H AND HAVE_MYSQL)
	add_and_install_library(mod_mysql_vhost "mod_mysql_vhost.c")
	target_link_libraries(mod_mysql_vhost mysqlclient)
	add_and_install_library(mod_vhostdb_mysql "mod_vhostdb_mysql.c")
	target_link_libraries(mod_vhostdb_mysql mysqlclient)
	include_directories(/usr/include/mysql)

	add_and_install_library(mod_authn_mysql "mod_authn_mysql.c")
	set(L_MOD_AUTHN_MYSQL)
	if(HAVE_LIBCRYPT)
		set(L_MOD_AUTHN_MYSQL ${L_MOD_AUTHN_MYSQL} crypt)
	endif()
	target_link_libraries(mod_authn_mysql ${L_MOD_AUTHN_MYSQL} mysqlclient)
endif()

if(HAVE_PGSQL_H AND HAVE_PGSQL)
	add_and_install_library(mod_vhostdb_pgsql "mod_vhostdb_pgsql.c")
	target_link_libraries(mod_vhostdb_pgsql pq)
endif()

if(HAVE_DBI_H AND HAVE_DBI)
	add_and_install_library(mod_vhostdb_dbi "mod_vhostdb_dbi.c")
	target_link_libraries(mod_vhostdb_dbi dbi)
endif()

set(L_MOD_WEBDAV)
if(HAVE_SQLITE3_H)
	set(L_MOD_WEBDAV ${L_MOD_WEBDAV} sqlite3)
endif()
if(HAVE_LIBXML_H)
	target_link_libraries(mod_webdav ${XML2_LDFLAGS})
endif()
if(HAVE_UUID_UUID_H)
	if(NEED_LIBUUID)
		set(L_MOD_WEBDAV ${L_MOD_WEBDAV} uuid)
	endif()
endif()

target_link_libraries(mod_webdav ${L_MOD_WEBDAV})

set(L_MOD_AUTHN_FILE)
if(HAVE_LIBCRYPT)
	set(L_MOD_AUTHN_FILE ${L_MOD_AUTHN_FILE} crypt)
endif()
target_link_libraries(mod_authn_file ${L_MOD_AUTHN_FILE})

if(WITH_KRB5)
	check_library_exists(krb5 krb5_init_context "" HAVE_KRB5)
	add_and_install_library(mod_authn_gssapi "mod_authn_gssapi.c")
	set(L_MOD_AUTHN_GSSAPI ${L_MOD_AUTHN_GSSAPI} krb5 gssapi_krb5)
	target_link_libraries(mod_authn_gssapi ${L_MOD_AUTHN_GSSAPI})
endif()

if(WITH_LDAP)
	set(L_MOD_AUTHN_LDAP ${L_MOD_AUTHN_LDAP} ldap lber)
	add_and_install_library(mod_authn_ldap "mod_authn_ldap.c")
	target_link_libraries(mod_authn_ldap ${L_MOD_AUTHN_LDAP})
	add_and_install_library(mod_vhostdb_ldap "mod_vhostdb_ldap.c")
	target_link_libraries(mod_vhostdb_ldap ${L_MOD_AUTHN_LDAP})
endif()

if(WITH_PAM)
	add_and_install_library(mod_authn_pam "mod_authn_pam.c")
	set(L_MOD_AUTHN_PAM ${L_MOD_AUTHN_PAM} pam)
	target_link_libraries(mod_authn_pam ${L_MOD_AUTHN_PAM})
endif()

if(WITH_SASL)
	add_and_install_library(mod_authn_sasl "mod_authn_sasl.c")
	set(L_MOD_AUTHN_SASL ${L_MOD_AUTHN_SASL} sasl2)
	target_link_libraries(mod_authn_sasl ${L_MOD_AUTHN_SASL})
endif()

if(HAVE_ZLIB_H)
	if(HAVE_BZLIB_H)
		target_link_libraries(mod_compress ${ZLIB_LIBRARY} bz2)
		target_link_libraries(mod_deflate ${ZLIB_LIBRARY} bz2)
	else()
		target_link_libraries(mod_compress ${ZLIB_LIBRARY})
		target_link_libraries(mod_deflate ${ZLIB_LIBRARY})
	endif()
endif()

if(HAVE_LIBFAM)
	target_link_libraries(lighttpd fam)
endif()

if(HAVE_GDBM_H)
	target_link_libraries(mod_trigger_b4_dl gdbm)
endif()

if(WITH_MEMCACHED)
	target_link_libraries(mod_trigger_b4_dl memcached)
endif()

if(HAVE_XATTR)
	target_link_libraries(lighttpd attr)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -g -Wshadow -W -pedantic ${WARN_CFLAGS}")
	set(CMAKE_C_FLAGS_RELEASE        "${CMAKE_C_FLAGS_RELEASE}     -O2")
	set(CMAKE_C_FLAGS_DEBUG          "${CMAKE_C_FLAGS_DEBUG}       -O0")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_WITHDEBINFO} -O2")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${WARN_LDFLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${WARN_LDFLAGS}")
endif()

if((NOT APPLE) OR CMAKE_C_COMPILER_ID MATCHES "GNU")
	add_target_properties(lighttpd LINK_FLAGS "-Wl,-export-dynamic")
endif()

set_target_properties(lighttpd PROPERTIES CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

if(WIN32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNVALGRIND")
	add_target_properties(lighttpd COMPILE_FLAGS "-DLI_DECLARE_EXPORTS")
	target_link_libraries(lighttpd ws2_32)
	target_link_libraries(mod_proxy ws2_32)
	target_link_libraries(mod_fcgi ws2_32)
	target_link_libraries(mod_scgi ws2_32)
	target_link_libraries(mod_ssi ws2_32)

	if(MINGW)
		target_link_libraries(lighttpd msvcr70)
		add_target_properties(lighttpd LINK_FLAGS "-Wl,-subsystem,console")
	endif()
endif()

if(NOT BUILD_STATIC)
	if(HAVE_LIBDL)
		target_link_libraries(lighttpd dl)
	endif()
endif()

if(NOT ${CRYPTO_LIBRARY} EQUAL "")
	if(NOT WITH_WOLFSSL)
		target_link_libraries(lighttpd ssl)
	endif()
	target_link_libraries(lighttpd ${CRYPTO_LIBRARY})
	add_and_install_library(mod_openssl "mod_openssl.c")
	if(NOT WITH_WOLFSSL)
		set(L_MOD_OPENSSL ${L_MOD_OPENSSL} ssl)
	endif()
	set(L_MOD_OPENSSL ${L_MOD_OPENSSL} ${CRYPTO_LIBRARY})
	target_link_libraries(mod_openssl ${L_MOD_OPENSSL})
	target_link_libraries(mod_auth ${CRYPTO_LIBRARY})
	set(L_MOD_AUTHN_FILE ${L_MOD_AUTHN_FILE} ${CRYPTO_LIBRARY})
	target_link_libraries(mod_authn_file ${L_MOD_AUTHN_FILE})
	target_link_libraries(mod_secdownload ${CRYPTO_LIBRARY})
	target_link_libraries(mod_wstunnel ${CRYPTO_LIBRARY})
endif()

if(WITH_LIBEV)
	target_link_libraries(lighttpd ${LIBEV_LDFLAGS})
	add_target_properties(lighttpd COMPILE_FLAGS ${LIBEV_CFLAGS})
endif()

if(WITH_LIBUNWIND)
	target_link_libraries(lighttpd ${LIBUNWIND_LDFLAGS})
	add_target_properties(lighttpd COMPILE_FLAGS ${LIBUNWIND_CFLAGS})

	target_link_libraries(test_array ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_array COMPILE_FLAGS ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_buffer ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_buffer COMPILE_FLAGS ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_burl ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_burl COMPILE_FLAGS ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_base64 ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_base64 COMPILE_FLAGS ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_configfile ${PCRE_LDFLAGS} ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_configfile COMPILE_FLAGS ${PCRE_CFLAGS} ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_keyvalue ${PCRE_LDFLAGS} ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_keyvalue COMPILE_FLAGS ${PCRE_CFLAGS} ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_mod_access ${PCRE_LDFLAGS} ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_mod_access COMPILE_FLAGS ${PCRE_CFLAGS} ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_mod_evhost ${PCRE_LDFLAGS} ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_mod_evhost COMPILE_FLAGS ${PCRE_CFLAGS} ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_mod_simple_vhost ${PCRE_LDFLAGS} ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_mod_simple_vhost COMPILE_FLAGS ${PCRE_CFLAGS} ${LIBUNWIND_CFLAGS})
	target_link_libraries(test_request ${LIBUNWIND_LDFLAGS})
	add_target_properties(test_request COMPILE_FLAGS ${LIBUNWIND_CFLAGS})
endif()

if(NOT WIN32)
install(TARGETS ${L_INSTALL_TARGETS}
	RUNTIME DESTINATION ${SBINDIR}
	LIBRARY DESTINATION ${LIGHTTPD_MODULES_DIR}
	ARCHIVE DESTINATION ${LIGHTTPD_MODULES_DIR}/static)
else()
## HACK to make win32 to install our libraries in desired directory..
install(TARGETS lighttpd
	RUNTIME DESTINATION ${SBINDIR}
	ARCHIVE DESTINATION lib/static)
list(REMOVE_ITEM L_INSTALL_TARGETS lighttpd)
install(TARGETS ${L_INSTALL_TARGETS}
	RUNTIME DESTINATION ${SBINDIR}/lib
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib/static)
endif()
