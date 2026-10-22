/*
 * A306295  Maximal number of coalescent histories among non-matching pairs
 *          consisting of a caterpillar gene tree and a caterpillar species
 *          tree with n+2 leaves.
 * https://oeis.org/A306295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+1) - C(floor((n+1)/2)) * C(ceil((n+1)/2)), where C(k)
 *            is Catalan number
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is irrelevant to the OEIS offset */
    if (n < 1) return 0;

    idx_t k = n + 1;           // k = n+1
    idx_t half = k / 2;        // floor((n+1)/2)
    idx_t ceil = k - half;     // ceil((n+1)/2)

    seq_t c_k    = catalan(k);
    seq_t c_half = catalan(half);
    seq_t c_ceil = catalan(ceil);

    return c_k - c_half * c_ceil;
}
