/*
 * A255002  Coefficients of recurrence for rows and columns of A250544 and
 *          rows of A250691.
 * https://oeis.org/A255002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of recurrence for rows and columns of A250544 and rows of A250691.
   a(0) .. a(16) are the non‑zero coefficients of the recurrence; a(n) = 0 for n>16. */

#include <stdint.h>

static const int64_t coeffs[17] = {
    -1,      /* a(0) */
     30,     /* a(1) */
    -415,    /* a(2) */
     3514,   /* a(3) */
    -20386,  /* a(4) */
     85924,  /* a(5) */
    -272198, /* a(6) */
     661180, /* a(7) */
    -1244717,/* a(8) */
     1822478,/* a(9) */
    -2068955,/* a(10) */
     1802474,/* a(11) */
    -1181760,/* a(12) */
     563888, /* a(13) */
    -184752, /* a(14) */
     37152,  /* a(15) */
    -3456    /* a(16) */
};

seq_t a(idx_t n)
{
    if (n < 17) {
        return (seq_t)coeffs[n];
    }
    return 0;
}
