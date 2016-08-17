#include <unistd.h>
#include <security/pam_appl.h>

#include "dict.h"

static dict_entry_t const	names[] =	{
	[PAM_SUCCESS]           = {
		"PAM_SUCCESS",
		"SUCCESS"
	},
	[PAM_AUTH_ERR]          = {
		"PAM_AUTH_ERR",
		"Could not be authenticated"
	},
	[PAM_USER_UNKNOWN]      = {
		"PAM_USER_UNKNOWN",
		"No account for user"
	},
	[PAM_CRED_INSUFFICIENT] = {
		"PAM_CRED_INSUFFICIENT",
		"Cannot access auth data because of insuffienct creds"
	},
	[PAM_AUTHINFO_UNAVAIL]  = {
		"PAM_AUTHINFO_UNAVAIL",
		"Cannot retriev auth info"
	},
	[PAM_IGNORE]            = {
		"PAM_IGNORE",
		"Ignore underlying cred"
	},
	[PAM_SERVICE_ERR]       = {
		"PAM_SERVICE_ERR",
		"Conversation failed"
	},
	[PAM_CONV_ERR]          = {
		"PAM_CONV_ERR",
		"Error in underlying services"
	},
	[PAM_MAXTRIES]          = {
		"PAM_MAXTRIES",
		"Limit further retries"
	},
	[PAM_PERM_DENIED]       = {
		"PAM_PERM_DENIED",
		"No auth for activity"
	},
	[PAM_SYSTEM_ERR]        = {
		"PAM_SYSTEM_ERR",
		"System error"
	},
	[PAM_BUF_ERR]           = {
		"PAM_BUF_ERR",
		"Memory buffer error"
	},
};

dict_t	const	pamdict =	{
	"PAM Returns",
	names,
	sizeof(names)/sizeof(names[0])
};
