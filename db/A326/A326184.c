/*
 * A326184  a(n) = sigma(n) - A057521(n), where A057521 gives the powerful
 *          part of n, and sigma gives the sum of divisors of n.
 * https://oeis.org/A326184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - A057521(n) where sigma(n) is the sum of
 *            divisors of n and A057521(n) is the largest powerful divisor of
 *            n (p^e if e ≥ 2, otherwise 1)
 * Range:     u8 n <= 16, u16 n <= 1603, u32 n <= 9963, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1 */

    /* Factorise n */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum of divisors */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t pseq = (seq_t)p;
        /* term = (p^(e+1) - 1) / (p - 1) */
        seq_t term = (big_pow(pseq, (seq_t)(e + 1)) - 1) / (pseq - 1);
        sigma *= term;
    }

    /* Powerful part: product of p^e for exponents e ≥ 2 */
    seq_t powerful = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e >= 2) {
            powerful *= big_pow((seq_t)p, (seq_t)e);
        }
    }

    return sigma - powerful;
}
