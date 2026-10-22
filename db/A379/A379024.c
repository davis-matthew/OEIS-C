/*
 * A379024  Expansion of (1/x) * Series_Reversion( x * ((1 - x - x^2)/(1 +
 *          x))^4 ).
 * https://oeis.org/A379024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * sum_{k=0..n} C(4*n+k+3,k) * C(4*n+k+4,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              where C(a,b) is binomial(a,b) */

#include <stddef.h>
#include "../utils/binomial.h"

#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
/* C implementation – use 128‑bit intermediate to avoid overflow */
seq_t a(idx_t n)
{
    unsigned __int128 sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* compute binomials as seq_t (uint64_t or similar) */
        seq_t t1 = binomial(4 * n + k + 3, k);
        seq_t t2 = binomial(4 * n + k + 4, n - k);
        sum += (unsigned __int128)t1 * (unsigned __int128)t2;
    }
    return (seq_t)(sum / ((unsigned __int128)n + 1));
}
#else
/* C++ (or C) build with generic seq_t – rely on seq_mul for safe multiplication */
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t denom = (seq_t)n + 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binomial(4 * n + k + 3, k);
        seq_t t2 = binomial(4 * n + k + 4, n - k);
        seq_t prod = seq_mul(t1, t2);  /* big‑int multiplication if needed */
        sum += prod;
    }
    return sum / denom;
}
#endif
