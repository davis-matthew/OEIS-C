/*
 * A363696  Expansion of Sum_{k>0} (1/(1-x^k)^6 - 1).
 * https://oeis.org/A363696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} binomial(d+5,5)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 214, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle n = 0 (not expected, but safe) */
    if (n == 0) return 0;

    /* factor n: collect prime powers */
    idx_t primes[20];
    unsigned exp[20];
    size_t pcnt = 0;

    idx_t m = n;

    /* factor 2 */
    unsigned e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e) { primes[pcnt] = 2; exp[pcnt] = e; ++pcnt; }

    /* factor odd primes */
    idx_t p = 3;
    for (; p * p <= m; p += 2) {
        e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (e) { primes[pcnt] = p; exp[pcnt] = e; ++pcnt; }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exp[pcnt] = 1;
        ++pcnt;
    }

    /* generate all divisors */
    idx_t divs[128];
    size_t divcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        idx_t pval = primes[i];
        unsigned e = exp[i];
        size_t oldcnt = divcnt;
        idx_t mult = 1;
        for (unsigned k = 1; k <= e; ++k) {
            mult *= pval;
            for (size_t j = 0; j < oldcnt; ++j) {
                divs[divcnt++] = divs[j] * mult;
            }
        }
    }

    /* compute sum of binomials */
    seq_t sum = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        seq_t binom = 1;
        for (int j = 1; j <= 5; ++j) {
            binom *= (seq_t)(d + j);
            binom /= (seq_t)j;
        }
        sum += binom;
    }

    return sum;
}
