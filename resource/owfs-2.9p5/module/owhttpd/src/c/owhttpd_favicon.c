/*
$Id$
 * http.c for owhttpd (1-wire web server)
 * By Paul Alfille 2003, using libow
 * offshoot of the owfs ( 1wire file system )
 *
 * GPL license ( Gnu Public Lincense )
 *
 * Based on chttpd. copyright(c) 0x7d0 greg olszewski <noop@nwonknu.org>
 *
 */

#include "owhttpd.h"

// #include <libgen.h>  /* for dirname() */

/* ------------ Protoypes ---------------- */
static const char favicon[] = {
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00, 0x01, 0x00,
	0x18, 0x00, 0x28, 0x03,
	0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x12, 0x0b,
	0x00, 0x00, 0x12, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xff,
	0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff,
	0xff, 0x00, 0xff, 0xff,
	0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
	0x00, 0xff, 0xff, 0x00,
	0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc0, 0xff,
	0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0xc0, 0xff, 0xff,
	0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0x00,
	0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x00,
	0x00, 0x00, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0xc0, 0xff, 0xff,
	0x00, 0x00, 0xc0, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0x00,
	0x00, 0xc0, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0xc0, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0xc0, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x80, 0xc0,
	0xff, 0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0xc0,
	0x00, 0x00, 0x80, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x80, 0xc0,
	0xff, 0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0x00, 0x00,
	0xc0, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x80, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0,
	0xff, 0xff, 0x00, 0x00,
	0xc0, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0x00, 0x00,
	0xc0, 0x00, 0x00, 0x80,
	0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff, 0xff, 0x00, 0x00, 0x80, 0x00, 0x00, 0xc0, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0x00, 0x00,
	0xc0, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xc0,
	0xff, 0xff, 0xc0, 0xff,
	0xff, 0xc0, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xc0, 0xff, 0xff, 0x00, 0x00,
	0xc0, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0x00, 0x00, 0xc0, 0xc0,
	0xff, 0xff, 0x00, 0x00,
	0x00, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff,
	0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff,
	0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xc0,
	0xff, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff,
	0xff, 0x00, 0xff, 0xff,
	0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff,
	0x00, 0xff, 0xff, 0x00,
	0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00,
	0xff, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00,
};

void Favicon(FILE * out)
{
	int i;
	HTTPstart(out, "200 OK", ct_icon);
	for (i = 0; i < 894; ++i) {
		fprintf(out, "%c", favicon[i]);
	}
	HTTPfoot(out);
}
