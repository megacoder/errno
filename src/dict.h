#ifndef	DICT_H
# define DICT_H

# include <sys/types.h>

typedef	struct	{
	char const * const name;
	char const * const desc;
} dict_entry_t;

typedef	struct	{
	char const * const		title;
	dict_entry_t const * const	d;
	size_t				n;
} dict_t;

#endif	/* DICT_H */
