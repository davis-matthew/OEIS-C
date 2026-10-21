/*
 * A288853  Triangle read by rows: T(n,k) is the number of surjective linear
 *          mappings from an n-dimensional vector space over F_2 onto a
 *          k-dimensional vector space, n>=0, 0<=k<=n.
 * https://oeis.org/A288853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the flattened triangle T(r,k) *
 *            where T(r,k) = ∏_{j=0}^{k-1} (2^r – 2^j). The index n maps to *
 *            a pair (r,k) with * r = ⌊(√(8n+1) – 1)/2⌋ * k = n – r(r+1)/2 *
 *            and a(n) = T(r,k). This reproduces the OEIS sequence A288853
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 26, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine the row r using the inverse of the triangular number
     * sequence.  The double-precision sqrt is accurate enough for
     * idx_t values up to at least 2^56. */
    long double x = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((x - 1.0L) / 2.0L);

    /* Correct any rounding error caused by floating point truncation. */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column index within row r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute the product T(r,k). */
    seq_t prod = 1;
    seq_t two_pow_r = pow2(r);
    for (idx_t j = 0; j < k; ++j) {
        seq_t term = two_pow_r - pow2(j);
        prod = prod * term;
    }
    return prod;
}
