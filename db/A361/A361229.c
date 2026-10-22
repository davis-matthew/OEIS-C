/*
 * A361229  G.f. A(x) satisfies A(x) = 1 + x^4 * (A(x) / (1 - x))^2.
 * https://oeis.org/A361229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A361229
 *
 * Formula: a(n) = Sum_{k=0..⌊n/4⌋} binomial(n-2*k-1, n-4*k) * catalan(k)
 *          with a(0) = 1, a(1) = a(2) = a(3) = 0.
 */

#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t, idx_t); */
#include "../utils/catalan.h"    /* provides: seq_t catalan(idx_t); */

seq_t a(idx_t n)
{
    /* handle the trivial initial values */
    if (n == 0) return (seq_t)1;
    if (n < 4)  return (seq_t)0;

    seq_t sum = 0;
    idx_t maxk = n >> 2;               /* floor(n/4) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t a_idx = n - 2 * k - 1;   /* a = n-2k-1   */
        idx_t b_idx = n - 4 * k;       /* b = n-4k     */

        /* binomial(a,b) is defined only for a ≥ b.  For a < b the term is zero. */
        if (a_idx < b_idx) continue;

        seq_t term = binomial(a_idx, b_idx);
        term = term * catalan(k);     /* multiply by Catalan number */
        sum += term;
    }

    return sum;
}
