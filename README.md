Ever had a program exit with the only explanation being "errno=22".  What does
that mean?  With this errno(1) tool, you can find out:

	$ errno 22
	22      EINVAL          Invalid argument

Built-in to errno(1) are the decodings for:

1.  System call errors from errno.
2.  TCP/IP socket status, aka network errors.
3.  X11 error codes.
4.  HTTP/1.0 network codes.
