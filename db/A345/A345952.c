/*
 * A345952  a(n) = 1 if the largest prime power divisor of n (A034699) is
 *          greater than the largest prime divisor of n (A006530).
 * https://oeis.org/A345952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the largest prime power divisor of n (A034699(n)) is
 *            greater than the largest prime divisor of n (A006530(n)), 0
 *            otherwise
 * Range:     u8 n <= 4351, u16 n <= 9923, u32 n <= 9923, u64 n <= 9923,
 *            u128 n <= 9923, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n < 2)
        return 0;

    /* factor n; at most 20 prime factors for 64‑bit n */
    uint64_t pr[20];
    uint64_t ex[20];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t max_prime = 0;
    seq_t max_power = 0;

    for (size_t i = 0; i < cnt; ++i) {
        const uint64_t p = pr[i];
        const size_t e = ex[i];

        seq_t pw = 1;
        for (size_t j = 0; j < e; ++j)
            pw *= (seq_t)p;

        if (pw > max_power) max_power = pw;
        const seq_t sp = (seq_t)p;
        if (sp > max_prime) max_prime = sp;
    }

    return (max_power > max_prime) ? 1 : 0;
}
