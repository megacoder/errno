#include <unistd.h>
#include "dict.h"
#include "tcp_states.h"

static dict_entry_t const	names[] =	{
	[TCP_ESTABLISHED]   = {    "TCP_ESTABLISHED", "ESTABLISHED" },
	[TCP_SYN_SENT]      = {       "TCP_SYN_SENT", "SYN_SENT" },
	[TCP_SYN_RECV]      = {       "TCP_SYN_RECV", "SYN_RECV" },
	[TCP_FIN_WAIT1]     = {      "TCP_FIN_WAIT1", "FIN_WAIT1" },
	[TCP_FIN_WAIT2]     = {      "TCP_FIN_WAIT2", "FIN_WAIT2" },
	[TCP_TIME_WAIT]     = {      "TCP_TIME_WAIT", "TIME_WAIT" },
	[TCP_CLOSE]         = {          "TCP_CLOSE", "CLOSE" },
	[TCP_CLOSE_WAIT]    = {     "TCP_CLOSE_WAIT", "CLOSE_WAIT" },
	[TCP_LAST_ACK]      = {       "TCP_LAST_ACK", "LAST_ACK" },
	[TCP_LISTEN]        = {         "TCP_LISTEN", "LISTEN" },
	[TCP_CLOSING]       = {        "TCP_CLOSING", "CLOSING" },
	[TCP_MAX_STATES]    = {     "TCP_MAX_STATES", "MAX_STATES" },
};

dict_t	const	netdict =	{
	names,
	sizeof(names)/sizeof(names[0])
};
