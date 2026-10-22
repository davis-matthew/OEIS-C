/*
 * A395108  Number of points covered by a minimal level n H-Tree on a square
 *          lattice.
 * https://oeis.org/A395108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 1 + 3*4^k - 6*2^(k-1) (k ≥ 1); a(0) = 1 * a(2k+1) = 1 +
 *            6*4^k - 4*2^k (k ≥ 0) * (Derived from the closed‑form expression
 *            involving √2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases first */
    if (n == 0u) return (seq_t)1;

    if ((n & 1u) == 0u) {                    /* even n = 2k */
        idx_t k = n >> 1u;                   /* k = n/2 */
        if (k == 0u) return (seq_t)1;        /* corresponds to n=0 */

        /* 4^k = 2^(2k), 2^(k-1) is (k-1)th power of two */
        seq_t p4 = pow2((idx_t)(2ul * k));
        seq_t p2 = pow2((idx_t)(k - 1u));

        /* 1 + 3*4^k - 6*2^(k-1) */
        return (seq_t)1 + (seq_t)3 * p4 - (seq_t)6 * p2;
    } else {                                 /* odd n = 2k+1 */
        idx_t k = n >> 1u;                   /* k = floor(n/2) */

        seq_t p4 = pow2((idx_t)(2ul * k));
        seq_t p2 = pow2((idx_t)k);

        /* 1 + 6*4^k - 4*2^k */
        return (seq_t)1 + (seq_t)6 * p4 - (seq_t)4 * p2;
    }
}
