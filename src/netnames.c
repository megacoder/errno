#include <unistd.h>
#include "dict.h"
#include "tcp_states.h"

static dict_entry_t const	tcp_names[] =	{
	[TCP_CLOSE      ] = "TCP_CLOSE",
	[TCP_CLOSE_WAIT ] = "TCP_CLOSE_WAIT",
	[TCP_CLOSING    ] = "TCP_CLOSING",
	[TCP_ESTABLISHED] = "TCP_ESTABLISHED",
	[TCP_FIN_WAIT1  ] = "TCP_FIN_WAIT1",
	[TCP_FIN_WAIT2  ] = "TCP_FIN_WAIT2",
	[TCP_LAST_ACK   ] = "TCP_LAST_ACK",
	[TCP_LISTEN     ] = "TCP_LISTEN",
	[TCP_MAX_STATES ] = "TCP_MAX_STATES",
	[TCP_SYN_RECV   ] = "TCP_SYN_RECV",
	[TCP_SYN_SENT   ] = "TCP_SYN_SENT",
	[TCP_TIME_WAIT  ] = "TCP_TIME_WAIT",
};
