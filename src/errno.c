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
	#include "../config.h"
#else
	#define PACKAGE "errno"
#endif /* HAVE_CONFIG_H */


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <alloca.h>
#include <netdb.h>
#include <ctype.h>
#include <libintl.h>
#include <limits.h>
#include <popt.h>
#include <locale.h>
#include <assert.h>
#include <string.h>

#include "dict.h"

#include "gcc-compat.h"

#if	ENABLE_NLS
	#define	_(x)		gettext( x )
	#define	N_(x)		x
#else
	#define	_(x)		x
	#define	N_(x)		x
	#define	gettext( x )	x
#endif

#if	HAVE_LIBREADLINE
	#include <readline/readline.h>
	#include <readline/history.h>
#endif	/* !HAVE_LIBREADLINE */

static	char const *	me = PACKAGE;
static	unsigned	nonfatal;
static	unsigned	sw_l;
static	poptContext	optCon;
static	unsigned	choices;
static	dict_t const *	actions[10];	/* Space for up to 9 choices */
static	dict_t const * *	next_action = actions;

#define	CHOICE_E	(1 << 0)
#define	CHOICE_N	(1 << 1)
#define	CHOICE_S	(1 << 2)
#define	CHOICE_W	(1 << 3)
#define	CHOICE_X	(1 << 4)
#define	CHOICE_P	(1 << 5)

extern	dict_t const errdict;
extern	dict_t const netdict;
extern	dict_t const pamdict;
extern	dict_t const sigdict;
extern	dict_t const webdict;
extern	dict_t const x11dict;

/*
 *------------------------------------------------------------------------
 * popt setup
 *------------------------------------------------------------------------
 */

static	const	struct poptOption	optionsTable[] =	{
	{
		"pam",
		'p',
		POPT_ARG_NONE,
		NULL,
		'p',
		N_( "Treat N as a pam(8) error." ),
		NULL
	},
	{
		"signals",
		's',
		POPT_ARG_NONE,
		NULL,
		's',
		N_( "Treat N as a signal value." ),
		NULL
	},
	{
		"errors",
		'e',
		POPT_ARG_NONE,
		NULL,
		'e',
		N_( "Treat N as a errno value." ),
		NULL
	},
	{
		"list",
		'l',
		POPT_ARG_NONE,
		NULL,
		'l',
		N_( "List a table of all known values" ),
		NULL
	},
	{
		"network",
		'n',
		POPT_ARG_NONE,
		NULL,
		'n',
		N_( "decode network error values" ),
		NULL
	},
	{
		"X11",
		'x',
		POPT_ARG_NONE,
		NULL,
		'x',
		N_( "decode X11 return values" ),
		NULL
	},
	{
		"web",
		'w',
		POPT_ARG_NONE,
		NULL,
		'w',
		N_( "decode html web status values" ),
		NULL
	},
	POPT_AUTOHELP
	POPT_TABLEEND
};

/*
 *------------------------------------------------------------------------
 * usage: how to run this program
 *------------------------------------------------------------------------
 */

static	void _printf( 3, 4 )
usage(
	poptContext		optCon,
	int			e,
	char const * const	fmt,
	...
)
{
	poptPrintUsage( optCon, stderr, 0 );

	if( fmt )	{
		va_list		ap;
		fprintf( stderr, "%s: ", me );
		va_start( ap, fmt );
		vfprintf( stderr, fmt, ap );
		va_end( ap );

		if( e )	{
			fprintf( stderr, "; errno=%d (%s)", e, strerror( e ) );
		}

		fprintf( stderr, "\n" );
	}
}

static	int		_printf( 3, 4 )
snprintf_ellipsis(
	char	*	buf,
	size_t		size,
	char const *	fmt,
	...
)
{
	va_list		ap;
	size_t		needed;
	va_start( ap, fmt );
	needed = vsnprintf( buf, size, fmt, ap );
	va_end( ap );

	if( needed >= size )	{
		static	char const	ellipsis[] = "...";
		strcpy( buf + size - sizeof( ellipsis ), ellipsis );
	}

	return( needed );
}

static	void
print_de_all(
	int const		e,
	dict_entry_t const *	de
)
{
	char		number[ 31 ];
	char		title[ 21 ];
	assert( de /* No dict_entry to decode */ );

	if( e == INT_MIN )	{
		number[ 0 ] = '\0';
	} else	{
		snprintf_ellipsis(
			number,
			sizeof( number ),
			"%d",
			e
		);
	}

	snprintf_ellipsis(
		title,
		sizeof( title ),
		"%s",
		de->name ? de->name : _( "[UNKNOWN]" )
	);
	printf(
		"%s\t%-23s\t%s\n",
		number,
		title,
		de->desc ? de->desc : _( "[NO DESCRIPTION]" )
	);
}

static	dict_entry_t const *
name_to_de(
	dict_t const	*	dict,
	char const	*	name
)
{
	dict_entry_t const *	retval;
	retval = NULL;

	do	{
		dict_entry_t const *	de;

		if( !name )	{
			break;
		}

		for( de = dict->d; de < ( dict->d + dict->n ); ++de )	{
			if( de->name && !strcasecmp( name, de->name ) ) {
				retval = de;
				break;
			}
		}
	} while( 0 );

	return( retval );
}

static	dict_entry_t const *
value_to_de(
	dict_t const * const	dict,
	size_t			e	/* Always positive		 */
)
{
	dict_entry_t const *	retval;

	retval = NULL;
	do	{
		if( e < dict->n )	{
			retval = dict->d + e;
		}
	} while( 0 );

	return( retval );
}

static	void
explain_de(
	dict_t const * const	dict,
	dict_entry_t const *	de
)
{
	if( de ) do	{
		print_de_all(
			de ? de - dict->d : INT_MIN,
			de
		);
	} while( 0 );
}

/*
 *------------------------------------------------------------------------
 * explain_term: lookup errno and write its value, from any dictionary
 *------------------------------------------------------------------------
 */

static	void
explain_term(
	char const	*	name
)
{
	dict_t const * *	dict_addr;

	for( dict_addr = actions; *dict_addr; ++dict_addr )	{
		dict_entry_t const * const	de = name_to_de(
			*dict_addr,
			name
		);
		explain_de( *dict_addr, de );
	}
}

#if	!HAVE_LIBREADLINE
/*
 *------------------------------------------------------------------------
 * readline: simulate GNU readline() system if we don't have it
 *------------------------------------------------------------------------
 */

static	char *
readline(
	char const *	prompt
)
{
	static size_t const	len = BUFSIZ + 1;
	char	*	result;
	result = malloc( len );

	if( result )	{
		if( prompt && prompt[0] )	{
			printf( "%s", prompt );
			fflush( stdout );
		}

		if( !fgets( result, len, stdin ) )	{
			free( result );
			result = NULL;
		}
	}

	return( result );
}

void
add_history(
	char const * const	line	_unused
)
{
	/*NOTHING*/
}
#endif	/* !HAVE_LIBREADLINE */

/*
 *------------------------------------------------------------------------
 * get_last: find the length of the longest active dictionary
 *------------------------------------------------------------------------
 */

static	int
get_last(
	void
)
{
	size_t			last;
	dict_t const * *	dict_addr;

	last = 0;
	for( dict_addr = actions; *dict_addr; ++dict_addr )	{
		last = max( last, (*dict_addr)->n );
	}
	return( last );
}

static	char const *
de_to_name(
	dict_entry_t const * const	de
)
{
	char const *	s = de ? de->name : "";
	return( s ? s : "" );
}

static	void
do_list(
	void
)
{
	static char const	fmt[] = "\t%-*.*s";
	static size_t const	width = 23;
	char const		bars[] = "______________________________";
	size_t const		last = get_last();
	size_t			e;
	dict_t const * *	dict_addr;

	for( dict_addr = actions; *dict_addr; ++dict_addr )	{
		printf( fmt, (int) width, (int) width, (*dict_addr)->title );
	}
	printf( "\n" );
	for( dict_addr = actions; *dict_addr; ++dict_addr )	{
		size_t const	len = min(
			width - 1,
			strlen( (*dict_addr)->title )
		 );
		printf( fmt, (int) len, (int) len, bars );
	}
	printf( "\n" );
	printf( "\n" );
	for( e = 0; e < last; ++e )	{
		dict_entry_t const *	de;

		printf( "%d", (int) e );
		for( dict_addr = actions; *dict_addr; ++dict_addr )	{
			de = value_to_de( *dict_addr, e );
			printf( fmt, (int) width, (int) width, de_to_name( de ) );
		}
		printf( "\n" );
	}
}

/*
 *------------------------------------------------------------------------
 * main: central control logic
 *------------------------------------------------------------------------
 */

int
main(
	int		argc,
	char const * *	argv
)
{
	do	{
		char	*	bp;
		int		c;
		/* Figure out our process name				*/
		me = argv[ 0 ];

		if( ( bp = strrchr( me, '/' ) ) != NULL )	{
			me = bp + 1;
		}

#if	ENABLE_NLS
		setlocale( LC_ALL, "" );
		bindtextdomain( PACKAGE, LOCALEDIR );
		textdomain( PACKAGE );
#endif	/* ENABLE_NLS */
		/* Process command line switches via popt		 */
		optCon = poptGetContext(
				 NULL,
				 argc,
				 argv,
				 optionsTable,
				 0
			 );
		poptSetOtherOptionHelp( optCon, "<id/number> .." );

		while( ( c = poptGetNextOpt( optCon ) ) >= 0 )	{
			switch( c )	{
				default:
					if( isalnum( c ) | ispunct( c )
					  )	{
						printf( "Unknown switch -%c\n", c );
					} else	{
						printf(
							"Unknown switch c=0x%02X\n",
							c
						);
					}

					break;

				case 'e':
					choices |= CHOICE_E;
					break;

				case 'l':
					++sw_l;
					break;

				case 'n':
					choices |= CHOICE_N;
					break;

				case 'p':
					choices |= CHOICE_P;
					break;

				case 's':
					choices |= CHOICE_S;
					break;

				case 'w':
					choices |= CHOICE_W;
					break;

				case 'x':
					choices |= CHOICE_X;
					break;
			}
		}

		if( c < -1 )	{
			usage(
				optCon,
				0,
				"%s: %s\n",
				poptBadOption( optCon, POPT_BADOPTION_NOALIAS ),
				poptStrerror( c )
			);
			exit( 1 );
		}
	} while( 0 );
	/* Push selection actions onto the action stack			 */
	if( ! choices )	{
		choices |= ~0U;	/* Default to all tables		 */
	}
	if( choices & CHOICE_E )	{
		*next_action++ = &errdict;
	}
	if( choices & CHOICE_N )	{
		*next_action++ = &netdict;
	}
	if( choices & CHOICE_P )	{
		*next_action++ = &pamdict;
	}
	if( choices & CHOICE_S )	{
		*next_action++ = &sigdict;
	}
	if( choices & CHOICE_X )	{
		*next_action++ = &x11dict;
	}
	if( choices & CHOICE_W )	{
		*next_action++ = &webdict;
	}
	*next_action = NULL;
	/*								 */
	if( !nonfatal ) do	{
			if( sw_l )	{
				do_list();
				break;
			}

			if( !choices )	{
				choices |= CHOICE_E;
			}

			/* Process command line arguments, if any	 */
			if( poptPeekArg( optCon ) )	{
				/* More arguments remaining		 */
				char const *	name;

				while( ( name = poptGetArg( optCon ) ) != NULL ) {
					explain_term( name );
				}
			} else	{
				int const	interactive = isatty( fileno( stdin ) );
				char		prompt[ BUFSIZ + 1 ];
				size_t		needed;
				/* Build prompt, even if we don't need it */
				needed = snprintf(
						 prompt,
						 sizeof( prompt ),
						 "%s> ",
						 me
					 );
				prompt[ DIM( prompt ) - 1 ] = '\0';

				if( needed >= sizeof( prompt ) )	{
					fprintf(
						stderr,
						"%s: %s\n",
						me,
						_( "Internal prompt buffer too small" )
					);
					exit( 1 );
					/* NOTREACHED			 */
				}

				/* Get an process each stdin line	 */
				while( !feof( stdin ) )	{
					char * const	line = readline(
								       interactive ? prompt : NULL
							       );
					char	*	name;
					char	*	lp;

					/* Recognize EOF		 */
					if( !line )	{
						if( interactive )	{
							printf( "\n[EOF]\n" );
						}

						break;
					}

					if( interactive )	{
						add_history( line );
					}

					/* Drop comments		 */
					lp = strchr( line, '#' );

					if( lp )	{
						*lp = '\0';
					}

					/* Explain remaining tokens	 */
					for(
						lp = line;
						( name = strtok( lp, " \t\n" ) ) != NULL;
						lp = NULL
					)	{
						explain_term( name );
					}

					/* Discard the line, we're done	 */
					free( line );
				}
			}
		} while( 0 );

	return( nonfatal ? 1 : 0 );
}
