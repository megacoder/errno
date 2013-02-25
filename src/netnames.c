/*
 *------------------------------------------------------------------------
 * vim: ts=8 sw=8
 *------------------------------------------------------------------------
 * Author:   tf135c (James Reynolds)
 * Filename: errno.c
 * Created:  2007-03-12 15:52:32
 *------------------------------------------------------------------------
 */

#ifdef	HAVE_CONFIG_H
#  include "../config.h"
#endif /* HAVE_CONFIG_H */


#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include "dict.h"

#include "gcc-compat.h"

#if	ENABLE_NLS
# define	_(x)		gettext( x )
# define	N_(x)		x
#else
# define	_(x)		x
# define	N_(x)		x
# define	gettext( x )	x
#endif

static dict_entry_t const	names[] =	{
	[ HOST_NOT_FOUND ] =	{
	 "HOST_NOT_FOUND",
	 N_("Authoritative answer host not found.")
	},
	[ TRY_AGAIN ] =	{
	 "TRY_AGAIN",
	 N_("Temporary error on authoritative name server. Try again.")
	},
	[ NO_RECOVERY ] =	{
	 "NO_RECOVERY",
	 N_("Non-recoverable DNS error.")
	},
	[ NO_DATA ] =	{
	 "NO_DATA",
	 N_("Valid name but no IP address found.")
	},
};

dict_t const	netdict =	{
	names,
	DIM( names )
};
