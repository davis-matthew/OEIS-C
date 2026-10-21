/*
 * A253226  Number of (n+1) X (3+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minimum minus antidiagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal maximum
 *          nondecreasing vertically.
 * https://oeis.org/A253226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46 + 1443*2^(n-5) + (13*n)/2 + (15*n^2)/2 for n > 4 a(1)
 *            = 125, a(2) = 306, a(3) = 494, a(4) = 914
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)125;
            case 2: return (seq_t)306;
            case 3: return (seq_t)494;
            case 4: return (seq_t)914;
        }
    }
    /* n > 4: apply the closed‑form expression */
    seq_t pow2n = pow2(n - 5);                     /* 2^(n-5) */
    seq_t term   = pow2n * (seq_t)1443;            /* 1443*2^(n-5) */
    seq_t nsq    = (seq_t)n * (seq_t)n;            /* n^2 in seq_t */
    seq_t poly   = (((seq_t)13 * (seq_t)n) + ((seq_t)15 * nsq)) >> 1; /* (13n+15n^2)/2 */
    return (seq_t)46 + term + poly;
}
