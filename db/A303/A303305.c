/*
 * A303305  Generalized 17-gonal (or heptadecagonal) numbers: m*(15*m - 13)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(15*n + 26)/8 if n even (n+1)*(15*n - 11)/8 if n odd
 * Range:     u8 n <= 11, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;            /* convert index to seq_t for calculations */
    if ((n & 1ULL) == 0ULL) {        /* even n */
        return sn * (((seq_t)15 * sn) + (seq_t)26) / 8;
    } else {                         /* odd n */
        return (sn + 1U) * (((seq_t)15 * sn) - (seq_t)11) / 8;
    }
}
