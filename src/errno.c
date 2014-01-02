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
#else
# define PACKAGE "errno"
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

#if	HAVE_LIBREADLINE
# include <readline/readline.h>
# include <readline/history.h>
#endif	/* !HAVE_LIBREADLINE */

static	char const *	me = PACKAGE;
static	unsigned	nonfatal;
static	unsigned	sw_l;
static	poptContext	optCon;
static	unsigned	choices;

#define	CHOICE_E	(1 << 0)
#define	CHOICE_N	(1 << 1)
#define	CHOICE_S	(1 << 2)
#define	CHOICE_X	(1 << 3)

extern	dict_t const errdict;
extern	dict_t const netdict;
extern	dict_t const sigdict;
extern	dict_t const x11dict;

/*
 *------------------------------------------------------------------------
 * popt setup
 *------------------------------------------------------------------------
 */

static	const	struct poptOption	optionsTable[] =	{
	{
		"signals",
		's',
		POPT_ARG_NONE,
		NULL,
		's',
		N_("Treat N as a signal value."),
		NULL
	},
	{
		"errors",
		'e',
		POPT_ARG_NONE,
		NULL,
		'e',
		N_("Treat N as a errno value."),
		NULL
	},
	{
		"list",
		'l',
		POPT_ARG_NONE,
		NULL,
		'l',
		N_("List a table of all known values"),
		NULL
	},
	{
		"network",
		'n',
		POPT_ARG_NONE,
		NULL,
		'n',
		N_("decode network error values"),
		NULL
	},
	{
		"X11",
		'x',
		POPT_ARG_NONE,
		NULL,
		'x',
		N_("decode X11 return values"),
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

static	void _printf(3,4)
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
	char *		buf,
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
report_dict_entry(
	int const		minus,	/* Originally had '-' prefix	 */
	int const		e,
	dict_entry_t const *	de
)
{
	char *		leadin;
	char		number[ 31 ];
	char		title[ 21 ];

	assert( de /* No dict_entry to decode */ );
	if( minus )	{
		leadin = "-";
	} else	{
		leadin = "";
	}
	if( e == INT_MIN )	{
		number[ 0 ] = '\0';
	} else	{
		snprintf_ellipsis(
			number,
			sizeof( number ),
			"%s%d",
			leadin,
			e
		);
	}
	snprintf_ellipsis(
		title,
		sizeof( title ),
		"%s%s",
		leadin,
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
dict_by_name(
	dict_t const *		dict,
	char const *		name
)
{
	static	dict_entry_t const	none = {
		NULL,
		NULL
	};
	dict_entry_t const *	retval;

	retval = NULL;
	do	{
		dict_entry_t const *	de;

		if( !name )	{
			break;
		}
		for( de = dict->d; de < (dict->d+dict->n); ++de )	{
			if( de->name && !strcasecmp( name, de->name ) ) {
				retval = de;
				break;
			}
		}
	} while( 0 );
	return( retval );
}

static	dict_entry_t const *
dict_by_value(
	dict_t const *		dict,
	int const		e	/* Always positive		 */
)
{
	static dict_entry_t const	none =	{
		NULL,
		NULL
	};
	dict_entry_t const *	retval;

	retval = &none;
	do	{
		if( e < dict->n )	{
			dict_entry_t const *	de = dict->d + e;

			if( de->name )	{
				retval = de;
				break;
			}
		}
	} while( 0 );
	return( retval );
}

static	int
explain_dict_term(
	dict_t const * const	dict,
	char const *		name
)
{
	int			retval;

	retval = 1;
	do	{
		dict_entry_t const *	de;
		char const *		desc;
		int			c;
		int			minus;

		de = NULL;
		if( (name[0] & 0xFF) == '-' )	{
			minus = 1;
			++name;
		} else	{
			minus = 0;
		}
		if( isdigit( name[0] ) )	{
			int		e;

			e = atoi( name );
			de = dict_by_value( dict, e );
			report_dict_entry(
				minus,
				e,
				de
			);
		} else	{
			de = dict_by_name( dict, name );
			report_dict_entry(
				minus,
				de ? de - dict->d : INT_MIN,
				de
			);
		}
		if( de )	{
			retval = 0;
		}
	} while( 0 );
	return( retval );
}

static	char const *
name_from_value(
	dict_t const * const	dict,
	size_t			e	/* Always positive		 */
)
{
	char const *	retval;

	retval = NULL;
	do	{
		if( e < dict->n )	{
			dict_entry_t const * const	de = dict->d + e;

			retval = de->name;
		}
	} while( 0 );
	return( retval );
}

static	char const *
value_from_name(
	dict_t const * const	dict,
	char const * const	name,
	size_t * const		ep
)
{
	char const *		retval;

	retval = NULL;
	do	{
		int		i;

		for( i = 0; i < dict->n; ++i )	{
			dict_entry_t const * const	de = dict->d + i;
			// char const * const	s = names[ i ];

			if( !strcasecmp( name, de->name ? : "<>" ) )	{
				retval = de->name;
				*ep = i;
				break;
			}
		}
	} while( 0 );
	return( retval );
}

/*
 * explain_term: lookup errno and write its value
 */

static	void
explain_term(
	char const *		name
)
{
	if( choices & CHOICE_E )	{
		explain_dict_term( &errdict, name );
	}
	if( choices & CHOICE_N )	{
		explain_dict_term( &netdict, name );
	}
	if( choices & CHOICE_S )	{
		explain_dict_term( &sigdict, name );
	}
	if( choices & CHOICE_X )	{
		explain_dict_term( &x11dict, name );
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
	char *		result;

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
 */

static	void
do_entry(
	dict_t const * const	dict,
	int const		e
)
{
	char const * const	name = name_from_value( dict, e );

	if( name )	{
		explain_dict_term( dict, name );
	}
}

static	int
get_last(
	void
)
{
	size_t		last;

	last = 0;
	if( choices & CHOICE_E )	{
		last = max( last, errdict.n );
	}
	if( choices & CHOICE_N )	{
		last = max( last, netdict.n );
	}
	if( choices & CHOICE_S )	{
		last = max( last, sigdict.n );
	}
	if( choices & CHOICE_X )	{
		last = max( last, x11dict.n );
	}
	return( last );
}

static	char const *
name_of(
	dict_entry_t const * const	de
)
{
	char const *	s = de ? de->name : NULL;
	return( s ? s : "" );
}

static	void
do_list(
	void
)
{
	char const	notta[] = "_______________";
	size_t const	last = get_last();
	size_t		e;

	for( e = 0; e < last; ++e )	{
		static const char	notta[] = "";
		dict_entry_t const *	de;

		printf( "%d", e );
		if( choices & CHOICE_E )	{
			de = dict_by_value( &errdict, e );
			printf( "\t%-15s",  name_of( de ) );
		}
		if( choices & CHOICE_N )	{
			de = dict_by_value( &netdict, e );
			printf( "\t%-15s",  name_of( de ) );
		}
		if( choices & CHOICE_S )	{
			de = dict_by_value( &sigdict, e );
			printf( "\t%-15s",  name_of( de ) );
		}
		if( choices & CHOICE_X )	{
			de = dict_by_value( &x11dict, e );
			printf( "\t%-15s",  name_of( de ) );
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
		char *		bp;
		int		c;

		/* Figure out our process name				*/
		me = argv[ 0 ];
		if( (bp = strrchr( me, '/' )) != NULL )	{
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
		while( (c = poptGetNextOpt( optCon )) >= 0 )	{
			switch( c )	{
			default:
				if( isalnum(c) | ispunct(c)
				)	{
					printf("Unknown switch -%c\n", c );
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
			case 's':
				choices |= CHOICE_S;
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
	if( !nonfatal ) do	{
		size_t const	last = get_last();
		size_t		e;

		if( sw_l )	{
			if( !choices )	{
				choices = ~0U;
			}
			do_list();
			break;
		}
		if( !choices )	{
			choices |= CHOICE_E;
		}
		/* Process command line arguments, if any	 */
		if(poptPeekArg( optCon ) )	{
			/* More arguments remaining		 */
			char const *	name;

			while( (name = poptGetArg( optCon )) != NULL ) {
				explain_term( name );
			}
		} else	{
			int const	interactive = isatty(fileno( stdin ) );
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
					_( "Internal prompt buffer too small" )
				);
				exit( 1 );
				/* NOTREACHED			 */
			}
			/* Get an process each stdin line	 */
			while(!feof( stdin ) )	{
				char * const	line = readline(
					interactive ? prompt: NULL
				);
				char *		name;
				char *		lp;

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
					( name = strtok( lp, " \t\n" )) != NULL;
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
