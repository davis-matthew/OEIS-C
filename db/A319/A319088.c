/*
 * A319088  a(n) = Sum_{k=1..n} k^2*tau_3(k), where tau_3 is A007425.
 * https://oeis.org/A319088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^2 * tau_3(k)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 739, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau_3(k) = prod_{p^e || k} binomial(e+2,2) */

#include "../utils/factor_u64.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    seq_t sum = 0;
    /* Temporary arrays for prime factors and exponents */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    for (idx_t k = 1; k <= n; ++k)
    {
        factor_u64((uint64_t)k, pr, ex, &cnt);

        seq_t tau = 1;
        for (size_t i = 0; i < cnt; ++i)
        {
            seq_t e = (seq_t)ex[i];
            seq_t comb = (e + 2) * (e + 1) / 2;   /* binomial(e+2,2) */
            tau *= comb;
        }

        seq_t kk = (seq_t)k * (seq_t)k;  /* k^2 in seq_t */
        sum += kk * tau;
    }

    return sum;
}
