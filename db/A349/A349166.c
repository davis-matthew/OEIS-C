/*
 * A349166  Numbers k such that sigma(k) and A003961(k) share a prime factor,
 *          where A003961(n) is fully multiplicative function with a(prime(k))
 *          = prime(k+1).
 * https://oeis.org/A349166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k ≥ 2 for which σ(k) and A003961(k) share
 *            a common prime factor. The algorithm uses a simple sieve of
 *            small prime factors up to a fixed limit (here 2 000 000) and
 *            checks each integer sequentially, caching the found values
 * Range:     u8 n <= 8, u16 n <= 966, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

#define INITIAL_LIMIT 2000000      /* enough for n ≤ 10000 */

static idx_t *spf   = NULL;   /* smallest prime factor for each integer */
static idx_t *primes = NULL;  /* list of primes up to INITIAL_LIMIT      */
static size_t prime_cnt = 0;   /* number of primes found                 */
static idx_t *nextPrimeFor = NULL; /* mapping from prime → next prime (0 if unknown) */

static seq_t *cache   = NULL; /* cached sequence values */
static idx_t cache_cnt = 0;
static idx_t cache_cap = 0;

static idx_t last_checked = 2;/* next integer to test          */

/* ------------------------------------------------------------------ */
/* Build sieve of small prime factors and prime list up to LIMIT.     */
/* ------------------------------------------------------------------ */
static void build_sieve(idx_t limit)
{
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* collect primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i) ++prime_cnt;

    primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i) primes[idx++] = i;

    /* nextPrime mapping for primes within the limit */
    nextPrimeFor = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p <= limit) nextPrimeFor[p] = primes[i + 1];
    }
}

/* ------------------------------------------------------------------ */
/* Compute σ(n) and A003961(n) for a given n using the sieve data.   */
/* ------------------------------------------------------------------ */
static bool has_common_prime(idx_t n)
{
    seq_t sigma    = (seq_t)1;
    seq_t a003961  = (seq_t)1;
    idx_t temp = n;

    while (temp > 1) {
        idx_t p = spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* p^(e+1) */
        seq_t p_pow = (seq_t)1;
        for (idx_t i = 0; i < e + 1; ++i) p_pow *= (seq_t)p;

        /* σ factor: (p^(e+1)-1)/(p-1) */
        seq_t sigma_factor = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= sigma_factor;

        /* A003961 factor: nextPrime(p)^e */
        idx_t np = nextPrimeFor[p];
        if (np == 0) np = next_prime(p);          /* prime beyond the sieve */
        seq_t np_pow = (seq_t)1;
        for (idx_t i = 0; i < e; ++i) np_pow *= (seq_t)np;
        a003961 *= np_pow;
    }

    seq_t g = gcd(sigma, a003961);
    return g != (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* Ensure cache has capacity for at least 'need' elements.            */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    cache_cap = (need > 16) ? need * 2 : 16;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* Public function: a(idx_t n)                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for index 0 */

    /* initialize sieve once */
    if (!spf) build_sieve(INITIAL_LIMIT);

    /* generate terms until we have the required number */
    while (cache_cnt < n) {
        idx_t k = last_checked++;
        if (has_common_prime(k)) {
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
