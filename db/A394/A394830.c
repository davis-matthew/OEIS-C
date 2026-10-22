/*
 * A394830  Number of divisors d of n that do not divide n/d - 1.
 * https://oeis.org/A394830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(n) - A163671(n) (number of divisors of n minus
 *            the number of divisors d that divide n/d – 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* uint64_t, size_t */
#include "../utils/factor_u64.h"

/* Count the divisors d of n for which (n/d - 1) is divisible by d.
   The prime factorization of n is given in arrays 'pr' (primes) and 'ex' (exponents). */
static void
count_bad_divisors(uint64_t  n,
                   const uint64_t *pr,
                   const uint64_t *ex,
                   size_t        pf_cnt,
                   size_t        idx,
                   uint64_t      cur,
                   seq_t        *bad)
{
    if (idx == pf_cnt) {
        uint64_t d = cur;
        uint64_t t = n / d;                       /* n/d  */
        /* (t - 1) % d == 0  ⇔  d divides n/d - 1  */
        if ((t - 1) % d == 0)
            (*bad)++;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        count_bad_divisors(n, pr, ex, pf_cnt, idx + 1, cur * mult, bad);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for 0 and negative indices, return 0 safely */
        return 0;

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* Total number of divisors: product of (exponent + 1) */
    seq_t total = 1;
    for (size_t i = 0; i < pf_cnt; ++i)
        total *= (seq_t)(exps[i] + 1);

    /* Count bad divisors */
    seq_t bad = 0;
    count_bad_divisors((uint64_t)n, primes, exps, pf_cnt, 0, 1ULL, &bad);

    return total - bad;
}
