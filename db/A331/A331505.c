/*
 * A331505  Number of labeled graphs with n nodes and floor(n/2) edges.
 * https://oeis.org/A331505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = binom(C(n,2), floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* below offset, undefined */

    /* Compute C(n,2) using seq_t to avoid overflow */
    seq_t tot_seq = (seq_t)n * (seq_t)(n - 1) / 2;
    idx_t tot = (idx_t)tot_seq;  /* tot fits in idx_t for n ≤ 10000 */
    idx_t k = n / 2;              /* floor(n/2) */

    return binom(tot, k);
}
