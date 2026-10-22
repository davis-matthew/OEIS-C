/*
 * A359103  a(n) = Sum_{d|n} d * (n/d)^d.
 * https://oeis.org/A359103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * (n/d)^d (offset 1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 53, u64 n <= 113,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Local power function: base^exp with seq_t arithmetic (unsigned overflow is
 * defined).  Uses exponentiation by squaring. */
static inline seq_t seq_pow_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;    /* undefined for n = 0, return 0 as a safe default */

    seq_t sum = (seq_t)0;
    /* Enumerate divisors up to sqrt(n). */
    for (idx_t d = (idx_t)1; (uint64_t)d * (uint64_t)d <= (uint64_t)n; ++d)
    {
        if (n % d == (idx_t)0)
        {
            /* d is a divisor. n/d is the co‑divisor. */
            uint64_t co = n / d;

            /* term for divisor d:  d * (n/d)^d */
            seq_t pow1 = seq_pow_local((seq_t)co, d);
            sum += (seq_t)d * pow1;

            /* If divisor pair is distinct, add the complementary term. */
            if ((uint64_t)d != co)
            {
                /* co is the complementary divisor; n / co == d. */
                seq_t pow2 = seq_pow_local((seq_t)d, co);
                sum += (seq_t)co * pow2;
            }
        }
    }
    return sum;
}
