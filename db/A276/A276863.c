/*
 * A276863  First differences of the Beatty sequence A276854 for 1 + sqrt(5).
 * https://oeis.org/A276863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n*sqrt(5)) - floor((n-1)*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* sn holds n as a seq_t */
    seq_t sn = (seq_t)n;
    /* integer square root of 5*n^2 gives floor(n*sqrt(5)) */
    seq_t t = (seq_t)5 * sn * sn;
    seq_t cur = floor_sqrt(t);

    /* compute floor((n-1)*sqrt(5)) */
    seq_t prev;
    if (n == 0) {            /* should not occur (offset starts at 1), but guard anyway */
        prev = 0;
    } else {
        seq_t sn1 = sn - (seq_t)1;
        seq_t t1 = (seq_t)5 * sn1 * sn1;
        prev = floor_sqrt(t1);
    }

    return (seq_t)1 + (cur - prev);
}
