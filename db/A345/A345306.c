/*
 * A345306  a(n) = n * Sum_{p|n, p prime} lcm(p,n/p) / p.
 * https://oeis.org/A345306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{p|n, p prime} lcm(p,n/p) / p
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Factorise n into distinct primes and their exponents.
   primes[i] holds the i‑th prime factor and exps[i] its exponent.
   cnt will contain the number of distinct prime factors. */
static void factorize(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        int e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        primes[*cnt] = 2;
        exps[*cnt]   = e;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[*cnt] = d;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }

    /* remaining prime factor, if any */
    if (n > 1) {
        primes[*cnt] = n;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;              /* a(1) = 0, a(0) undefined in OEIS */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;

    factorize(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        seq_t term;
        if (e == 1) {
            /* term = n^2 / p = (n/p) * n */
            seq_t factor = (seq_t)(nn / p);
            term = factor * (seq_t)nn;
        } else {
            /* term = n^2 / p^2 = (n/p) * (n/p) */
            seq_t factor = (seq_t)(nn / p);
            term = factor * factor;
        }
        sum += term;
    }

    return sum;
}
