/*
 * A345261  a(n) = Sum_{d|n} d * rad(d).
 * https://oeis.org/A345261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} d * rad(d)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 7840, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the multiplicative definition:
   a(p^e) = (p^(e+2)-1)/(p-1) - p
   and then a(n) = Π a(p_i^e_i) over all prime factors of n. */
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    /* Compute (p^(e+2)-1)/(p-1) - p */
    seq_t pow = (seq_t)1;
    for (uint64_t i = 0; i < e + 2; ++i)
        pow = pow * (seq_t)p;

    seq_t numerator   = pow - (seq_t)1;
    seq_t denominator = (seq_t)(p - 1);
    seq_t quotient    = numerator / denominator;   /* integer division */
    return quotient - (seq_t)p;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined but guard to avoid UB */
        return (seq_t)0;

    /* Factor n into prime powers using the provided utility */
    uint64_t pri[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, pri, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t ap = a_prime_power(pri[i], exps[i]);
        result = result * ap;
    }
    return result;
}
