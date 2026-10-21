/*
 * A294287  Sum of the cubes of the parts in the partitions of n into two
 *          distinct parts.
 * https://oeis.org/A294287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)^2 (if n is odd) or * a(n) = binomial(n,2)^2
 *            - n^3/8 (if n is even)
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* binomial(n,2) is defined to be 0 when n < 2 */
    seq_t c = binomial(n, 2);
    seq_t res = c * c;

    if ((n & 1) == 0) {          /* even n */
        seq_t nseq = (seq_t)n;
        seq_t cube = nseq * nseq * nseq;   /* n^3  */
        cube /= 8;                 /* exact for even n */
        res -= cube;
    }
    return res;
}
