/*
 * A361737  Diagonal of rational function 1/(1 - (x + y + z + x^2*y*z)).
 * https://oeis.org/A361737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3*k)!/(k!^3) * binomial(k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, mul_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mul_exact.h"

/* Helper that computes (value * mult)/denom exactly.
   Works in both C and C++ builds. */
static inline seq_t mul_div(seq_t value, seq_t mult, seq_t denom)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    return (seq_t)(((unsigned __int128)value * mult) / denom);
#else
    return (value * mult) / denom;
#endif
}

/* Return the n-th term of OEIS sequence A361737.  */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t M    = 1;                     /* M(k) = (3k)!/(k!^3), so M(0)=1 */

    idx_t start = (n + 1) / 2;           /* first k with a non‑zero binomial */

    for (idx_t k = 0; k <= n; ++k) {
        if (k >= start) {
            seq_t bin = binomial(k, n - k);
            if (bin)  /* binomial may be zero for k < ceil(n/2) */
                sum += mul_exact(M, bin);
        }

        /* prepare M for the next k (if any) */
        if (k < n) {
            seq_t t = (seq_t)(k + 1);    /* (k+1) */
            seq_t a1 = (seq_t)(3 * k + 1);
            seq_t a2 = (seq_t)(3 * k + 2);
            seq_t a3 = (seq_t)(3 * k + 3);

            M = mul_div(M, a1, t);
            M = mul_div(M, a2, t);
            M = mul_div(M, a3, t);
        }
    }

    return sum;
}
