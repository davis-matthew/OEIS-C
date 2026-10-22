/*
 * A392168  The Dedekind psi function value of the smallest square divisible
 *          by n.
 * https://oeis.org/A392168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p+1)·p^(e - (1 - e mod 2))
 * Range:     u8 n <= 13, u16 n <= 185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(p^e) = (p+1)·p^(e-1) if e even, and (p+1)·p^e if e odd */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* compute p^e for an unsigned integer exponent, result in seq_t */
static seq_t pow_uint_uint(uint64_t p, uint64_t e)
{
    seq_t result = 1;
    seq_t base = (seq_t)p;
    while (e)
    {
        if (e & 1ULL)
            result *= base;
        e >>= 1ULL;
        if (e)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t e_adj = e;
        if ((e & 1ULL) == 0)          /* e even -> subtract 1 */
            e_adj -= 1ULL;

        seq_t p_pow = pow_uint_uint(p, e_adj);
        seq_t term  = (seq_t)(p + 1ULL) * p_pow;
        res *= term;
    }
    return res;
}
