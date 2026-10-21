/*
 * A286751  Positions of 1 in A286749; complement of A286750.
 * https://oeis.org/A286751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 2*floor(n*phi) - 2 with phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Offset is 1, but we guard against n==0 for robustness */
    if (n == 0)
        return 0;

    seq_t ns = (seq_t)n;
    /* Compute floor(n * sqrt(5)) as floor_sqrt(5 * n * n) */
    seq_t sq = (seq_t)5 * ns * ns;
    seq_t s  = floor_sqrt(sq);

    /* Parity correction: 2*floor((n+s)/2) = n + s - (n+s)&1 */
    seq_t parity = (ns + s) & 1;

    /* a(n) = 4*n - s - 2 + parity */
    seq_t result = (seq_t)4 * ns - s - (seq_t)2 + parity;
    return result;
}
