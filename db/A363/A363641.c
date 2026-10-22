/*
 * A363641  Expansion of Sum_{k>0} x^(2*k)/(1 - k*x^k)^2.
 * https://oeis.org/A363641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(d-2) * (d-1)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple integer square‑root (returns floor(sqrt(x))) */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    while (r * r <= x) ++r;
    return r - 1;
}

/* Modular exponentiation (base^exp) using repeated squaring.  Works for
   unsigned seq_t and idx_t types.  In the 64‑bit build overflows may
   occur when the result does not fit; that is acceptable because the
   value itself will not fit in seq_t as well (per requirement 8). */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) corresponds to n == 1 */
    if (n < 1) return 0;

    seq_t sum = 0;
    idx_t limit = isqrt_u64(n);

    for (idx_t i = 1; i <= limit; ++i)
    {
        if (n % i == 0)
        {
            /* divisor d = i */
            idx_t d = i;
            if (d >= 2)
            {
                seq_t base = (seq_t)n / (seq_t)d; /* = n/d */
                idx_t exp  = d - 2;
                seq_t powv = pow_seq(base, exp);
                sum += powv * (seq_t)(d - 1);
            }

            /* paired divisor d2 = n / i */
            idx_t d2 = n / i;
            if (d2 != d && d2 >= 2)
            {
                seq_t base = (seq_t)i;          /* since n/d2 == i */
                idx_t exp  = d2 - 2;
                seq_t powv = pow_seq(base, exp);
                sum += powv * (seq_t)(d2 - 1);
            }
        }
    }
    return sum;
}
