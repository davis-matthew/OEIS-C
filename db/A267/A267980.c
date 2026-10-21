/*
 * A267980  a(n) = Catalan(n)^2*(4n + 1).
 * https://oeis.org/A267980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n)^2 * (4*n + 1) (OEIS A267980)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute the Catalan number for n. */
    seq_t c = catalan(n);

    /* a(n) = c² · (4n + 1). Calculate it using seq_t arithmetic. */
    seq_t res = c * c;
    res *= (seq_t)(4ULL * n + 1ULL);
    return res;
}
