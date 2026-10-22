/*
 * A344878  a(n) is the least common multiple of numbers (2^(1+e2))-1 and
 *          those in the set (p_i^e_i)-1, when the odd part of n = Product
 *          (p_i^e_i), and e2 is the 2-adic valuation of n.
 * https://oeis.org/A344878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM of all numbers (p^(e + [p==2])) - 1, where n = ∏ p^e
 *            runs over its prime factorization
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_gcd.h"

/* compute lcm of two seq_t values */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = seq_gcd(a, b);
    return (a / g) * b;
}

/* OEIS A344878 */
seq_t a(idx_t n)
{
    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t ans = 1;  /* LCM of empty set is 1 */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* exponent increased by one if prime is 2 */
        seq_t exp = (seq_t)(e + (p == 2));
        seq_t val = big_pow((seq_t)p, exp) - (seq_t)1;
        ans = lcm_seq(ans, val);
    }
    return ans;
}
