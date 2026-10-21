/*
 * A276116  Least k such that A001222(k*n) = A001222(k*n+1), or -1 if no such
 *          k exists.
 * https://oeis.org/A276116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that Ω(k*n) = Ω(k*n+1), return -1 if none
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Ω(m) = total number of prime factors of m counted with multiplicity. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/factor_u64.h"

static seq_t omega_uint64(uint64_t m)
{
    /* factor_u64 fills pr, ex arrays with prime factors and exponents */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += (seq_t)ex[i];
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;                /* offset starts at 1 */

    idx_t k = 1;
    while (1)
    {
        /* prevent overflow of k*n */
        if ((uint64_t)k > UINT64_MAX / (uint64_t)n)
            return (seq_t)-1;

        uint64_t m1 = (uint64_t)k * (uint64_t)n;
        uint64_t m2 = m1 + 1;

        if (omega_uint64(m1) == omega_uint64(m2))
            return (seq_t)k;

        ++k;
    }
}
