#ifdef	HAVE_CONFIG_H
# include "../config.h"
#endif
#include <unistd.h>
#include <errno.h>
#include "dict.h"

#if	ENABLE_NLS
# define	_(x)		gettext( x )
# define	N_(x)		x
#else
# define	_(x)		x
# define	N_(x)		x
#endif

static dict_entry_t const 	names[ 134 ] =	{
	[ EWOULDBLOCK     ] =	{
	 "EWOULDBLOCK",
	 N_("Operation would block")
	},
	[ EPERM           ] =	{
	 "EPERM",
	 N_("Operation not permitted")
	},
	[ ENOENT          ] =	{
	 "ENOENT",
	 N_("No such file or directory")
	},
	[ ESRCH           ] =	{
	 "ESRCH",
	 N_("No such process")
	},
	[ EINTR           ] =	{
	 "EINTR",
	 N_("Interrupted system call")
	},
	[ EIO             ] =	{
	 "EIO",
	 N_("I/O error")
	},
	[ ENXIO           ] =	{
	 "ENXIO",
	 N_("No such device or address")
	},
	[ E2BIG           ] =	{
	 "E2BIG",
	 N_("Argument list too long")
	},
	[ ENOEXEC         ] =	{
	 "ENOEXEC",
	 N_("Exec format error")
	},
	[ EBADF           ] =	{
	 "EBADF",
	 N_("Bad file number")
	},
	[ ECHILD          ] =	{
	 "ECHILD",
	 N_("No child processes")
	},
	[ EAGAIN          ] =	{
	 "EAGAIN",
	 N_("Try again")
	},
	[ ENOMEM          ] =	{
	 "ENOMEM",
	 N_("Out of memory")
	},
	[ EACCES          ] =	{
	 "EACCES",
	 N_("Permission denied")
	},
	[ EFAULT          ] =	{
	 "EFAULT",
	 N_("Bad address")
	},
	[ ENOTBLK         ] =	{
	 "ENOTBLK",
	 N_("Block device required")
	},
	[ EBUSY           ] =	{
	 "EBUSY",
	 N_("Device or resource busy")
	},
	[ EEXIST          ] =	{
	 "EEXIST",
	 N_("File exists")
	},
	[ EXDEV           ] =	{
	 "EXDEV",
	 N_("Cross-device link")
	},
	[ ENODEV          ] =	{
	 "ENODEV",
	 N_("No such device")
	},
	[ ENOTDIR         ] =	{
	 "ENOTDIR",
	 N_("Not a directory")
	},
	[ EISDIR          ] =	{
	 "EISDIR",
	 N_("Is a directory")
	},
	[ EINVAL          ] =	{
	 "EINVAL",
	 N_("Invalid argument")
	},
	[ ENFILE          ] =	{
	 "ENFILE",
	 N_("File table overflow")
	},
	[ EMFILE          ] =	{
	 "EMFILE",
	 N_("Too many open files")
	},
	[ ENOTTY          ] =	{
	 "ENOTTY",
	 N_("Not a typewriter")
	},
	[ ETXTBSY         ] =	{
	 "ETXTBSY",
	 N_("Text file busy")
	},
	[ EFBIG           ] =	{
	 "EFBIG",
	 N_("File too large")
	},
	[ ENOSPC          ] =	{
	 "ENOSPC",
	 N_("No space left on device")
	},
	[ ESPIPE          ] =	{
	 "ESPIPE",
	 N_("Illegal seek")
	},
	[ EROFS           ] =	{
	 "EROFS",
	 N_("Read-only file system")
	},
	[ EMLINK          ] =	{
	 "EMLINK",
	 N_("Too many links")
	},
	[ EPIPE           ] =	{
	 "EPIPE",
	 N_("Broken pipe")
	},
	[ EDOM            ] =	{
	 "EDOM",
	 N_("Math argument out of domain of func")
	},
	[ ERANGE          ] =	{
	 "ERANGE",
	 N_("Math result not representable")
	},
	[ EDEADLK         ] =	{
	 "EDEADLK",
	 N_("Resource deadlock would occur")
	},
	[ ENAMETOOLONG    ] =	{
	 "ENAMETOOLONG",
	 N_("File name too long")
	},
	[ ENOLCK          ] =	{
	 "ENOLCK",
	 N_("No record locks available")
	},
	[ ENOSYS          ] =	{
	 "ENOSYS",
	 N_("Function not implemented")
	},
	[ ENOTEMPTY       ] =	{
	 "ENOTEMPTY",
	 N_("Directory not empty")
	},
	[ ELOOP           ] =	{
	 "ELOOP",
	 N_("Too many symbolic links encountered")
	},
	[ ENOMSG          ] =	{
	 "ENOMSG",
	 N_("No message of desired type")
	},
	[ EIDRM           ] =	{
	 "EIDRM",
	 N_("Identifier removed")
	},
	[ ECHRNG          ] =	{
	 "ECHRNG",
	 N_("Channel number out of range")
	},
	[ EL2NSYNC        ] =	{
	 "EL2NSYNC",
	 N_("Level 2 not synchronized")
	},
	[ EL3HLT          ] =	{
	 "EL3HLT",
	 N_("Level 3 halted")
	},
	[ EL3RST          ] =	{
	 "EL3RST",
	 N_("Level 3 reset")
	},
	[ ELNRNG          ] =	{
	 "ELNRNG",
	 N_("Link number out of range")
	},
	[ EUNATCH         ] =	{
	 "EUNATCH",
	 N_("Protocol driver not attached")
	},
	[ ENOCSI          ] =	{
	 "ENOCSI",
	 N_("No CSI structure available")
	},
	[ EL2HLT          ] =	{
	 "EL2HLT",
	 N_("Level 2 halted")
	},
	[ EBADE           ] =	{
	 "EBADE",
	 N_("Invalid exchange")
	},
	[ EBADR           ] =	{
	 "EBADR",
	 N_("Invalid request descriptor")
	},
	[ EXFULL          ] =	{
	 "EXFULL",
	 N_("Exchange full")
	},
	[ ENOANO          ] =	{
	 "ENOANO",
	 N_("No anode")
	},
	[ EBADRQC         ] =	{
	 "EBADRQC",
	 N_("Invalid request code")
	},
	[ EBADSLT         ] =	{
	 "EBADSLT",
	 N_("Invalid slot")
	},
	[ EBFONT          ] =	{
	 "EBFONT",
	 N_("Bad font file format")
	},
	[ ENOSTR          ] =	{
	 "ENOSTR",
	 N_("Device not a stream")
	},
	[ ENODATA         ] =	{
	 "ENODATA",
	 N_("No data available")
	},
	[ ETIME           ] =	{
	 "ETIME",
	 N_("Timer expired")
	},
	[ ENOSR           ] =	{
	 "ENOSR",
	 N_("Out of streams resources")
	},
	[ ENONET          ] =	{
	 "ENONET",
	 N_("Machine is not on the network")
	},
	[ ENOPKG          ] =	{
	 "ENOPKG",
	 N_("Package not installed")
	},
	[ EREMOTE         ] =	{
	 "EREMOTE",
	 N_("Object is remote")
	},
	[ ENOLINK         ] =	{
	 "ENOLINK",
	 N_("Link has been severed")
	},
	[ EADV            ] =	{
	 "EADV",
	 N_("Advertise error")
	},
	[ ESRMNT          ] =	{
	 "ESRMNT",
	 N_("Srmount error")
	},
	[ ECOMM           ] =	{
	 "ECOMM",
	 N_("Communication error on send")
	},
	[ EPROTO          ] =	{
	 "EPROTO",
	 N_("Protocol error")
	},
	[ EMULTIHOP       ] =	{
	 "EMULTIHOP",
	 N_("Multihop attempted")
	},
	[ EDOTDOT         ] =	{
	 "EDOTDOT",
	 N_("RFS specific error")
	},
	[ EBADMSG         ] =	{
	 "EBADMSG",
	 N_("Not a data message")
	},
	[ EOVERFLOW       ] =	{
	 "EOVERFLOW",
	 N_("Value too large for defined data type")
	},
	[ ENOTUNIQ        ] =	{
	 "ENOTUNIQ",
	 N_("Name not unique on network")
	},
	[ EBADFD          ] =	{
	 "EBADFD",
	 N_("File descriptor in bad state")
	},
	[ EREMCHG         ] =	{
	 "EREMCHG",
	 N_("Remote address changed")
	},
	[ ELIBACC         ] =	{
	 "ELIBACC",
	 N_("Can not access a needed shared library")
	},
	[ ELIBBAD         ] =	{
	 "ELIBBAD",
	 N_("Accessing a corrupted shared library")
	},
	[ ELIBSCN         ] =	{
	 "ELIBSCN",
	 N_(".lib section in a.out corrupted")
	},
	[ ELIBMAX         ] =	{
	 "ELIBMAX",
	 N_("Attempting to link in too many shared libraries")
	},
	[ ELIBEXEC        ] =	{
	 "ELIBEXEC",
	 N_("Cannot exec a shared library directly")
	},
	[ EILSEQ          ] =	{
	 "EILSEQ",
	 N_("Illegal byte sequence")
	},
	[ ERESTART        ] =	{
	 "ERESTART",
	 N_("Interrupted system call should be restarted")
	},
	[ ESTRPIPE        ] =	{
	 "ESTRPIPE",
	 N_("Streams pipe error")
	},
	[ EUSERS          ] =	{
	 "EUSERS",
	 N_("Too many users")
	},
	[ ENOTSOCK        ] =	{
	 "ENOTSOCK",
	 N_("Socket operation on non-socket")
	},
	[ EDESTADDRREQ    ] =	{
	 "EDESTADDRREQ",
	 N_("Destination address required")
	},
	[ EMSGSIZE        ] =	{
	 "EMSGSIZE",
	 N_("Message too long")
	},
	[ EPROTOTYPE      ] =	{
	 "EPROTOTYPE",
	 N_("Protocol wrong type for socket")
	},
	[ ENOPROTOOPT     ] =	{
	 "ENOPROTOOPT",
	 N_("Protocol not available")
	},
	[ EPROTONOSUPPORT ] =	{
	 "EPROTONOSUPPORT",
	 N_("Protocol not supported")
	},
	[ ESOCKTNOSUPPORT ] =	{
	 "ESOCKTNOSUPPORT",
	 N_("Socket type not supported")
	},
	[ EOPNOTSUPP      ] =	{
	 "EOPNOTSUPP",
	 N_("Operation not supported on transport endpoint")
	},
	[ EPFNOSUPPORT    ] =	{
	 "EPFNOSUPPORT",
	 N_("Protocol family not supported")
	},
	[ EAFNOSUPPORT    ] =	{
	 "EAFNOSUPPORT",
	 N_("Address family not supported by protocol")
	},
	[ EADDRINUSE      ] =	{
	 "EADDRINUSE",
	 N_("Address already in use")
	},
	[ EADDRNOTAVAIL   ] =	{
	 "EADDRNOTAVAIL",
	 N_("Cannot assign requested address")
	},
	[ ENETDOWN        ] =	{
	 "ENETDOWN",
	 N_("Network is down")
	},
	[ ENETUNREACH     ] =	{
	 "ENETUNREACH",
	 N_("Network is unreachable")
	},
	[ ENETRESET       ] =	{
	 "ENETRESET",
	 N_("Network dropped connection because of reset")
	},
	[ ECONNABORTED    ] =	{
	 "ECONNABORTED",
	 N_("Software caused connection abort")
	},
	[ ECONNRESET      ] =	{
	 "ECONNRESET",
	 N_("Connection reset by peer")
	},
	[ ENOBUFS         ] =	{
	 "ENOBUFS",
	 N_("No buffer space available")
	},
	[ EISCONN         ] =	{
	 "EISCONN",
	 N_("Transport endpoint is already connected")
	},
	[ ENOTCONN        ] =	{
	 "ENOTCONN",
	 N_("Transport endpoint is not connected")
	},
	[ ESHUTDOWN       ] =	{
	 "ESHUTDOWN",
	 N_("Cannot send after transport endpoint shutdown")
	},
	[ ETOOMANYREFS    ] =	{
	 "ETOOMANYREFS",
	 N_("Too many references: cannot splice")
	},
	[ ETIMEDOUT       ] =	{
	 "ETIMEDOUT",
	 N_("Connection timed out")
	},
	[ ECONNREFUSED    ] =	{
	 "ECONNREFUSED",
	 N_("Connection refused")
	},
	[ EHOSTDOWN       ] =	{
	 "EHOSTDOWN",
	 N_("Host is down")
	},
	[ EHOSTUNREACH    ] =	{
	 "EHOSTUNREACH",
	 N_("No route to host")
	},
	[ EALREADY        ] =	{
	 "EALREADY",
	 N_("Operation already in progress")
	},
	[ EINPROGRESS     ] =	{
	 "EINPROGRESS",
	 N_("Operation now in progress")
	},
	[ ESTALE          ] =	{
	 "ESTALE",
	 N_("Stale NFS file handle")
	},
	[ EUCLEAN         ] =	{
	 "EUCLEAN",
	 N_("Structure needs cleaning")
	},
	[ ENOTNAM         ] =	{
	 "ENOTNAM",
	 N_("Not a XENIX named type file")
	},
	[ ENAVAIL         ] =	{
	 "ENAVAIL",
	 N_("No XENIX semaphores available")
	},
	[ EISNAM          ] =	{
	 "EISNAM",
	 N_("Is a named type file")
	},
	[ EREMOTEIO       ] =	{
	 "EREMOTEIO",
	 N_("Remote I/O error")
	},
	[ EDQUOT          ] =	{
	 "EDQUOT",
	 N_("Quota exceeded")
	},
	[ ENOMEDIUM       ] =	{
	 "ENOMEDIUM",
	 N_("No medium found")
	},
	[ EMEDIUMTYPE     ] =	{
	 "EMEDIUMTYPE",
	 N_("Wrong medium type")
	},
	[ ECANCELED       ] =	{
	 "ECANCELED",
	 N_("Operation Canceled")
	},
	[ ENOKEY          ] =	{
	 "ENOKEY",
	 N_("Required key not available")
	},
	[ EKEYEXPIRED     ] =	{
	 "EKEYEXPIRED",
	 N_("Key has expired")
	},
	[ EKEYREVOKED     ] =	{
	 "EKEYREVOKED",
	 N_("Key has been revoked")
	},
	[ EKEYREJECTED    ] =	{
	 "EKEYREJECTED",
	 N_("Key was rejected by service")
	},
	[ EOWNERDEAD      ] =	{
	 "EOWNERDEAD",
	 N_("Owner died")
	},
	[ ENOTRECOVERABLE ] =	{
	 "ENOTRECOVERABLE",
	 N_("State not recoverable")
	},
	[ ERFKILL         ] =	{
	 "ERFKILL",
	 N_("Operation not possible due to RF-kill")
	},
	[ EHWPOISON       ] =	{
	 "EHWPOISON",
	 N_("Memory page has hardware error")
	},
};

dict_t const errdict =	{
	names,
	134
};
