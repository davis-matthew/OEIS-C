/*
 * A245474  a(n) = smallest positive integer s such that s*n -
 *          floor(sqrt(s*n))^2 is a square.
 * https://oeis.org/A245474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer s such that s*n -
 *            floor(sqrt(s*n))^2 is a perfect square. (Direct search up to s =
 *            n.)
 * Range:     u8 n <= 10, u16 n <= 1011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    seq_t N = (seq_t)n;             /* convert index to seq_t */
    seq_t s;

    for (s = 1; s <= (seq_t)n; ++s) {
        seq_t prod = N * s;          /* s * n */
        seq_t t    = floor_sqrt(prod);           /* floor(sqrt(s*n)) */
        seq_t diff = prod - t * t;                /* remainder */
        seq_t k    = floor_sqrt(diff);            /* sqrt of remainder */
        if (k * k == diff)                       /* remainder is a square? */
            return s;                            /* minimal s found */
    }

    /* In theory we should always return inside the loop.
       Fallback to n (s = n) if something unexpected occurs. */
    return (seq_t)n;
}
