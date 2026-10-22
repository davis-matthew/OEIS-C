/*
 * A382057  Z-sequence for the Riordan triangle A125166.
 * https://oeis.org/A382057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 12, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   (8 + 19*x + 18*x^2 + 6*x^3)/((1 + x)*(1 + 6*x + 6*x^2))
   gives
      a(0) =  8
      a(1) = -37
      a(2) = 181
      a(3) = -865
      a(n) = -7*a(n-1) -12*a(n-2) - 6*a(n-3)   for n ≥ 4   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *buf = NULL;     /* array of a(k) */
    static idx_t  cap = 0;        /* current size of buf   */

    /* If we have not yet computed up to n, expand the cache */
    if (n >= cap) {
        idx_t newcap = (cap == 0) ? 4 : cap;
        while (newcap <= n)
            newcap <<= 1;                     /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return 0;                  /* should never happen in tests */
        buf = tmp;

        /* If this is the very first allocation, initialise the first four terms */
        if (cap == 0) {
            buf[0] = 8;
            if (newcap > 1) buf[1] = 19 - 7 * buf[0];
            if (newcap > 2) buf[2] = 18 - 7 * buf[1] - 12 * buf[0];
            if (newcap > 3) buf[3] = 6  - 7 * buf[2] - 12 * buf[1] - 6 * buf[0];
        }

        /* Compute any remaining values up to newcap-1 */
        for (idx_t i = cap; i < newcap; ++i) {
            if (i < 4) continue;             /* base terms already set */
            buf[i] = -7 * buf[i-1]
                   -12 * buf[i-2]
                   - 6 * buf[i-3];
        }
        cap = newcap;
    }
    return buf[n];
}
