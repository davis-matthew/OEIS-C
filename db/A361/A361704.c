/*
 * A361704  Constant term in the expansion of (1 + w^2 + x^2 + y^2 + z^2 +
 *          1/(w*x*y*z))^n.
 * https://oeis.org/A361704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A361704 - Constant term in (1 + w² + x² + y² + z² + 1/(wxyz))ⁿ
   Formula (OEIS):  a(n) = Σ_{k=0..⌊n/6⌋}
     (4k)!/(k!⁴) · C(6k,4k) · C(n,6k)
   The implementation uses the identity
     (4k)!/(k!⁴) = C(4k,k)·C(3k,k)·C(2k,k),
   and all binomial coefficients are obtained via the helper
   `binomial(idx_t n, idx_t k)` supplied by the build system.
   The function works for arbitrary‑precision seq_t as well as
   for 64‑bit seq_t in the range where the result fits.  The
   computation is O(n) and never overflows the intermediate
   64‑bit arithmetic for values that fit in seq_t. */

#include <stddef.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t, idx_t); */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;          /* k = 0 contributes 1 */
    if (n < 6) return sum;         /* only the first term is non‑zero */

    idx_t maxk = n / 6;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* (4k)!/(k!⁴) = C(4k,k)·C(3k,k)·C(2k,k) */
        seq_t term = binomial(4*k, k);
        term *= binomial(3*k, k);
        term *= binomial(2*k, k);

        /* remaining binomials */
        term *= binomial(6*k, 4*k);
        term *= binomial(n, 6*k);

        sum += term;
    }

    return sum;
}
