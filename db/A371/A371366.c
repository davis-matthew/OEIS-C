/*
 * A371366  Expansion of (1/x) * Series_Reversion( x * (1-5*x)^2 / (1-4*x) ).
 * https://oeis.org/A371366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} 4^(n-k) * binomial(2*n+k+1,k) *
 *            binomial(2*n,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* denominator (n+1) */
    seq_t denom = (seq_t)n + 1ULL;

    /* special case n == 0 */
    if (n == 0)
        return 1;

    /* choose implementation based on size of seq_t */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 64‑bit (or smaller) build – use 128‑bit intermediate to avoid
           overflow in the intermediate products.  The final result fits
           into seq_t by construction, because the sequence is guaranteed
           to be divisible by (n+1). */

        unsigned __int128 sum   = 0;
        unsigned __int128 pow4  = 1;

        /* compute 4^n as 128‑bit value */
        for (idx_t i = 0; i < n; ++i)
            pow4 *= 4U;

        for (idx_t k = 0; k <= n; ++k) {
            /* term = 4^(n-k) * binomial(2*n+k+1,k) * binomial(2*n,n-k) */
            unsigned __int128 term = pow4;
            term *= (unsigned __int128)binomial(2 * n + k + 1, k);
            term *= (unsigned __int128)binomial(2 * n, n - k);

            sum += term;

            /* prepare next power of 4 */
            pow4 /= 4U;
        }

        /* final division – exact */
        return (seq_t)(sum / (unsigned __int128)denom);
    } else {
        /* seq_t is larger (128‑bit or arbitrary precision) – use
           normal seq_t arithmetic.  The intermediate sized values
           are handled by the underlying big‑integer implementation. */

        seq_t sum  = 0;
        seq_t pow4 = pow_uint(4, n);   /* 4^n in seq_t */

        for (idx_t k = 0; k <= n; ++k) {
            seq_t term = pow4;
            term *= binomial(2 * n + k + 1, k);
            term *= binomial(2 * n, n - k);

            sum += term;

            /* next power of 4 (exact division by 4) */
            pow4 /= 4;
        }

        return sum / denom;
    }
}
