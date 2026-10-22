/*
 * A342256  Numbers k such that gcd(k, Phi_k(a)) > 1 for some a, where Phi_k
 *          is the k-th cyclotomic polynomial.
 * https://oeis.org/A342256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A342256 – numbers k such that gcd(k,Phi_k(a))>1 for some a.
 *  Equivalent to the union over primes p of { d*p^e : e>=1, d | (p-1) }.
 *  We generate k sequentially, test the condition, and cache the found terms.
 *  The function a(idx_t n) returns the n-th term (a(1)=2).
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 2;  /* next integer to test */

/* test whether k satisfies the condition
 * k must be >= 2; it is assumed we never call this with k < 2.
 */
static bool is_term(uint64_t k)
{
    /* factor_u64 requires arrays for primes and exponents. Use fixed size. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    factor_u64(k, primes, exps, &cnt);
    if (cnt == 0)         /* shouldn't happen for k>=2 */
        return false;

    uint64_t p = primes[cnt-1];          /* largest prime factor */
    uint64_t d = k;

    /* divide k by p until no longer divisible: this removes p^e */
    while (d % p == 0)
        d /= p;

    /* now d must divide p-1 */
    return ((p - 1) % d) == 0;
}

/* sequence element function */
seq_t a(idx_t n)
{
    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed; return 0 as fallback */
    }

    size_t want = (size_t)n;
    while (cache_len < want) {
        if (is_term(next_candidate)) {
            if (cache_len == cache_cap) {
                /* grow cache */
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) break; /* allocation failed; stop */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
        /* small optimization: avoid overflow when next_candidate wraps around */
        if (next_candidate == 0)
            break;
    }

    if (cache_len >= want)
        return cache[want - 1];
    else
        return 0;  /* in case of allocation failure, return 0 */
}
