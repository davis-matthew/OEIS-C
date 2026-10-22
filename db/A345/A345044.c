/*
 * A345044  If n = Product p(k)^e(k) then a(n) = LCM (p(k)^e(k) - 1).
 * https://oeis.org/A345044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM_{p^e || n} (p^e - 1)
 * Range:     u8 n <= 256, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Euclidean GCD for arbitrary seq_t */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* LCM using GCD */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd_seq(a, b);
    return a / g * b;
}

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n <= (idx_t)1)
        return (seq_t)1;

    /* Factor n using the provided utility */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t pow = big_pow((seq_t)primes[i], (seq_t)exps[i]);
        seq_t term = pow - (seq_t)1;
        result = lcm_seq(result, term);
    }
    return result;
}
