/*
 * A320696  Number of partitions of n with up to nine distinct kinds of 1.
 * https://oeis.org/A320696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..9} binom(9,k) * p2(n-k) where p2(m) = number of
 *            partitions of m with parts >= 2. Since p2(m) = p(m) - p(m-1), p2
 *            can be computed with a simple DP using only additions
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 96, u64 n <= 365,
 *            u128 n <= 1377, big any n
 * Helpers:   compute_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include only standard headers needed. */
#include <stdlib.h>
#include "../utils/compute_p2.h"

/*  Main function required by the specification.  */
seq_t a(idx_t n)
{
    /*  Cache for the p2 array.  */
    static seq_t *p2 = NULL;
    static idx_t p2_size = 0;

    /*  Recompute p2 if we need values beyond what we have.  */
    if (n + 1 > p2_size) {
        size_t new_size = (size_t)(n + 1);
        p2 = (seq_t *)realloc(p2, new_size * sizeof(seq_t));
        compute_p2(n, p2);
        p2_size = (idx_t)new_size;
    }

    /*  Precomputed binomial coefficients binom(9,k).  */
    static const int bin9[10] = {1, 9, 36, 84, 126,
                                 126, 84, 36, 9, 1};

    seq_t res = 0;
    for (idx_t k = 0; k <= 9; ++k) {
        if (n >= k) {
            seq_t term = p2[n - k];
            term = term * (seq_t)bin9[k];
            res += term;
        }
    }
    return res;
}
