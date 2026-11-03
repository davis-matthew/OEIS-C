/* isqrt_seq: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_SEQ_H
#define OEISC_UTILS_ISQRT_SEQ_H
#include <stdint.h>

static seq_t isqrt_seq(seq_t x)
{
    /* Return the integer square root of x: maximum r with r*r <= x. */
    if (x == 0)
        return 0;

    /* Find an upper bound  hi such that hi*hi > x  (hi is the first such power of two). */
    seq_t hi = 1;
    while (hi <= x / hi) {
        /* For very large values, a left shift that overflows to zero would lead to division
           by zero later.  The shift only happens while hi <= x/hi, which stops before
           overflow for all practical n up to about 10 000 and for arbitrary-precision
           integer types. */
        hi <<= 1;
    }

    /* The search interval is now [1, hi-1] where hi*hi > x >= (hi/2)^2. */
    seq_t lo = 1;            /* lo >= 1 because x > 0 */

    /* Binary search for the floor sqrt. */
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {
            /* mid*mid <= x  => mid is a candidate, search higher */
            lo = mid + 1;
        } else {
            /* mid*mid > x  => mid too high, search lower */
            hi = mid - 1;
        }
    }
    return hi; /* hi is the largest integer with hi*hi <= x */
}

#endif
