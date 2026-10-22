/*
 * A366024  Expansion of (1/x) * Series_Reversion( x*(1-x)*(1+x^5) ).
 * https://oeis.org/A366024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/5⌋} (-1)^k * binomial(n+k,n) *
 *            binomial(2n-5k,n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute binomial(n,k) for unsigned integers.
   Works for any seq_t that supports +, -, *, /, and integer truncation. */
static inline seq_t __binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t kmax = n / 5;
    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t t1 = __binom(n + k, n);
        seq_t t2 = __binom(2 * n - 5 * k, n);
        seq_t t = t1 * t2;
        if (k & 1U)  /* odd k => negative sign */
            sum -= t;
        else
            sum += t;
    }
    seq_t denom = (seq_t)n + 1;
    return sum / denom;
}
