/*
 * A378541  Practical numbers whose sum of divisors is even.
 * https://oeis.org/A378541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 7767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A378541.
 * The function searches for practical numbers whose sum of divisors
 * is even, excluding numbers that are perfect squares or twice a square.
 * It caches all terms found so far for repeated calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t*, size_t, size_t); */

/* Helper to compute sigma(p^exp) for a prime p. */
static seq_t sigma_prime_power(uint64_t p, uint64_t exp)
{
    seq_t sum = 1;
    seq_t pw   = 1;
    for (uint64_t i = 0; i < exp; ++i) {
        pw *= (seq_t)p;
        sum += pw;
    }
    return sum;
}

/* Integer square root for 64‑bit unsigned integers. */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n;
    uint64_t y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Test whether n is a perfect square or twice a perfect square. */
static bool is_square_or_twice_square(uint64_t n)
{
    uint64_t r = isqrt_u64(n);
    if (r * r == n) return true;
    if ((n & 1ULL) == 0) {
        uint64_t half = n >> 1;
        uint64_t s    = isqrt_u64(half);
        if (s * s == half) return true;
    }
    return false;
}

/* Check practicalness of n (n > 1). */
static bool is_practical(uint64_t n)
{
    if (n <= 1) return false;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* factor 2 */
    if ((n & 1ULL) == 0) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0) { n >>= 1; ++e; }
        primes[cnt] = 2; exps[cnt] = e; ++cnt;
    }

    /* factor odd primes by trial division */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[cnt] = p; exps[cnt] = e; ++cnt;
        }
    }
    if (n > 1) { primes[cnt] = n; exps[cnt] = 1; ++cnt; }

    if (cnt == 0) return false;
    if (primes[0] != 2) return false;             /* 2 must be the smallest prime */

    seq_t sigma_prod = sigma_prime_power(primes[0], exps[0]);  /* sigma(2^e) */
    for (size_t i = 1; i < cnt; ++i) {
        if ((seq_t)primes[i] > sigma_prod + (seq_t)1) return false;
        seq_t sp = sigma_prime_power(primes[i], exps[i]);
        sigma_prod = sigma_prod * sp;
    }
    return true;
}

/* Compute sigma(n) for any unsigned integer n. */
static seq_t sigma_of(uint64_t n)
{
    seq_t res = 1;
    uint64_t temp = n;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((temp & 1ULL) == 0) { temp >>= 1; ++e; }
        res *= sigma_prime_power(2, e);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            res *= sigma_prime_power(p, e);
        }
    }
    if (temp > 1) { res *= sigma_prime_power(temp, 1); }

    return res;
}

/* ---------- Cache for the sequence ---------- */
static seq_t *cache      = NULL;   /* dynamic array of found terms */
static idx_t  cache_len  = 0;      /* number of terms stored */
static idx_t  cache_cap  = 0;      /* capacity of the array */
static uint64_t next_candidate = 1;  /* next integer to test */

/* Ensure that the cache can hold at least 'need' terms. */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap < need) new_cap <<= 1;
    seq_t *newp = seq_realloc(cache, (size_t)cache_cap, (size_t)new_cap);
    if (!newp) abort();
    cache = newp;
    cache_cap = new_cap;
}

/* ---------- External function required by the kernel ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence offset starts at 1 */

    ensure_cache(n);
    while (cache_len < n) {
        uint64_t cand = next_candidate++;

        /* skip candidates that are squares or twice squares */
        if (is_square_or_twice_square(cand)) continue;

        /* test practicalness */
        if (!is_practical(cand)) continue;

        /* test evenness of sigma */
        seq_t sigma = sigma_of(cand);
        if ((sigma & (seq_t)1) == (seq_t)0) {
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
