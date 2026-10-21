/*
 * A265390  a(n) = lcm_{d|n} tau(d) * Sum_{d|n} 1/tau(d), where tau(d)
 *          represents the number of divisors of d (A000005(d)).
 * https://oeis.org/A265390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = lcm_{d|n} tau(d) * Sum_{d|n} 1/tau(d)
 * Range:     u8 n <= 47, u16 n <= 1023, u32 n <= 9458, u64 n <= 9458,
 *            u128 n <= 9458, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is multiplicative:
 *   a(p^e) = sum_{k=1}^{e+1} L / k,
 *   where L = lcm{1,2,…,e+1}.
 * The answer for any n is the product of a(p^e) over its prime‑power
 * factors.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the term for a prime power p^e (e ≥ 1). */
static seq_t prime_power_term(uint64_t e)
{
    uint64_t m = e + 1;                 /* number of divisors of p^e */
    seq_t L = 1;                        /* L = lcm{1,…,m} */
    for (uint64_t k = 2; k <= m; ++k) {
        seq_t g = gcd(L, (seq_t)k);
        L = L / g * (seq_t)k;           /* L ← LCM(L, k) */
    }
    seq_t sum = 0;
    for (uint64_t k = 1; k <= m; ++k)
        sum += L / (seq_t)k;            /* k divides L exactly */
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0; safe guard */
    if (n == 1)
        return 1;                     /* a(1) = 1 */

    uint64_t rem = (uint64_t)n;
    seq_t result = 1;

    while (rem > 1) {
        /* Trial division to find the smallest prime factor of rem. */
        uint64_t p = 2;
        while (p * p <= rem && rem % p != 0)
            ++p;
        if (p * p > rem)                /* rem itself is prime */
            p = rem;

        uint64_t exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
        }

        seq_t cp = prime_power_term(exp);
        result *= cp;                   /* multiplicative property */
    }

    return result;
}
