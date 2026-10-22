/*
 * A328639  Dirichlet g.f.: zeta(2*s) / (zeta(s) * zeta(s-2)).
 * https://oeis.org/A328639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product_{p^e||n} (-1)^e * (p^2 + 1) (multiplicative, with
 *            a(1) = 1)
 * Range:     i8 n <= 9, i16 n <= 165, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* small helper to find the smallest prime factor of n */
static inline uint64_t _smallest_prime_factor(uint64_t n) {
    if (n % 2ULL == 0ULL)
        return 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL)
            return d;
    }
    return n;  /* n is prime */
}

/* Computes a single unaligned a(n) using prime factorization */
static seq_t _a_single(idx_t n) {
    if (n == 1ULL)
        return (seq_t)1;
    seq_t res = (seq_t)1;
    uint64_t m = (uint64_t)n;
    while (m > 1ULL) {
        uint64_t p = _smallest_prime_factor(m);
        size_t e = 0;
        while (m % p == 0ULL) {
            m /= p;
            ++e;
        }
        seq_t term = (seq_t)p * (seq_t)p + (seq_t)1;
        if (e & 1U)
            res = -res * term;
        else
            res = res * term;
    }
    return res;
}

/* Cached dynamic array */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;      /* allocated size */
static idx_t  a_cache_max = 0;      /* highest index computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void _ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;   /* index n can be stored at cache[n] */
    if (needed > a_cache_cap) {
        size_t new_cap = needed > a_cache_cap * 2U ? needed : a_cache_cap * 2U;
        seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort(); /* allocation failure */
        a_cache = new_cache;
        a_cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0ULL)   /* offset starts at 1; return 0 for safety */
        return (seq_t)0;

    _ensure_cache(n);

    if (a_cache_max == 0) {          /* first call; set a(1)=1 */
        a_cache[1] = (seq_t)1;
        a_cache_max = 1;
    }

    if (n <= a_cache_max)
        return a_cache[n];

    /* compute missing values up to n */
    for (idx_t i = a_cache_max + 1ULL; i <= n; ++i) {
        a_cache[i] = _a_single(i);
    }
    a_cache_max = n;
    return a_cache[n];
}
