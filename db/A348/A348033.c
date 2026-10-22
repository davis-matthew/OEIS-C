/*
 * A348033  Number of unitary divisors d of n such that sigma(d)*d is equal to
 *          n.
 * https://oeis.org/A348033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} [ gcd(d, n/d)=1 && σ(d)*d = n ], where σ is the
 *            sum–of–divisors function
 * Range:     u8 n <= 1215, u16 n <= 9079, u32 n <= 9079, u64 n <= 9079,
 *            u128 n <= 9079, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Factorise a 64‑bit integer into its distinct prime powers.
   primes[idx] and exps[idx] receive the prime factor and its exponent.
   cnt is set to the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t *primes, unsigned *exps, size_t *cnt)
{
    size_t i = 0;
    if (n % 2ULL == 0ULL) {
        unsigned e = 0;
        while (n % 2ULL == 0ULL) {
            n /= 2ULL;
            ++e;
        }
        primes[i] = 2ULL;
        exps[i]   = e;
        ++i;
    }
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            unsigned e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            primes[i] = p;
            exps[i]   = e;
            ++i;
        }
    }
    if (n > 1ULL) {
        primes[i] = n;
        exps[i]   = 1;
        ++i;
    }
    *cnt = i;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n=1 is a special case */
    if (n == 1) return (seq_t)1;

    uint64_t primes[16];
    unsigned exps[16];
    size_t cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    /* Precompute t[i] = σ(p_i^e_i) * p_i^e_i for each distinct prime factor. */
    seq_t t[16];
    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^e */
        seq_t pow = 1;
        for (unsigned k = 0; k < exps[i]; ++k) pow *= (seq_t)primes[i];
        /* Compute σ(p^e) = 1 + p + p^2 + ... + p^e */
        seq_t sigma = 1;
        seq_t term  = 1;
        for (unsigned k = 1; k <= exps[i]; ++k) {
            term *= (seq_t)primes[i];
            sigma += term;
        }
        t[i] = sigma * pow; /* product equals σ(p^e) * p^e */
    }

    seq_t target = (seq_t)n;
    seq_t count  = 0;

    /* Enumerate all subsets of the distinct prime powers (unitary divisors). */
    uint32_t mask_limit = 1U << (uint32_t)cnt;
    for (uint32_t mask = 0; mask < mask_limit; ++mask) {
        seq_t prod = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1U << (uint32_t)i))
                prod *= t[i];
        }
        if (prod == target)
            ++count;
    }

    return count;
}
