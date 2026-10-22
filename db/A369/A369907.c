/*
 * A369907  a(n) = n * Sum_{p|n, p prime} omega(n/p) / p.
 * https://oeis.org/A369907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{p|n} omega(n/p) / p Computed by prime
 *            factorization over the distinct prime divisors of n. a(p^k) =
 *            p^(k-1)
 * Range:     u8 n <= 119, u16 n <= 9652, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Factorise n into primes and their exponents.
   primes[0..cnt-1] and exps[0..cnt-1] contain the result. */
static void factorize_u64(uint64_t n, uint64_t *primes, unsigned *exps, size_t *cnt)
{
    *cnt = 0;
    if (n < 2)
        return;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        unsigned e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        primes[*cnt] = 2ULL;
        exps[*cnt]   = e;
        ++(*cnt);
    }

    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            unsigned e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exps[*cnt]   = e;
            ++(*cnt);
        }
    }

    /* remaining prime factor, if any */
    if (n > 1ULL) {
        primes[*cnt] = n;
        exps[*cnt]   = 1U;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)                       /* a(1) = 0 */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    unsigned exps[64];
    size_t cnt;

    factorize_u64(nn, primes, exps, &cnt);

    unsigned omega_n = (unsigned)cnt;   /* distinct prime factors of n */
    unsigned long long result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = exps[i];

        /* omega(n/p) : the distinct factor count of n/p */
        unsigned omega_m = (e == 1 ? omega_n - 1 : omega_n);
        if (omega_m == 0)
            continue;   /* term would be zero */

        unsigned long long term = (nn / p) * (unsigned long long)omega_m;
        result += term;
    }

    return (seq_t)result;
}
