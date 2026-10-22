/*
 * A391724  a(n) = Sum_{k=0..floor(n/4)} 2^k * 3^(n-3*k) * binomial(n-3*k+6,6)
 *          * binomial(2*(n-3*k),2*k).
 * https://oeis.org/A391724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} 2^k * 3^(n-3k) * binomial(n-3k+6,6) * *
 *            binomial(2*(n-3k),2k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 52,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* local power function using binary exponentiation */
static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4ULL;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t t = n - 3ULL * k;                     /* n - 3k */
        seq_t pow2k = pow_uint((seq_t)2, k);        /* 2^k */
        seq_t pow3t = pow_uint((seq_t)3, t);        /* 3^(n-3k) */
        seq_t bin1  = binomial(t + 6ULL, 6ULL);     /* binomial(n-3k+6,6)  */
        seq_t bin2  = binomial(2ULL * t, 2ULL * k); /* binomial(2*(n-3k),2k) */
        seq_t term  = bin1 * bin2;
        term *= pow2k;
        term *= pow3t;
        sum += term;
    }
    return sum;
}
