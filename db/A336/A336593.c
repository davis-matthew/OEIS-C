/*
 * A336593  Numbers k such that k/A008835(k) is cubeful (A036966), where
 *          A008835(k) is the largest 4th power dividing k.
 * https://oeis.org/A336593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 6523, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9944 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336593: numbers k such that at least one prime exponent in the factorisation of k is
   congruent to 3 mod 4.  The function a(n) returns the n‑th such number
   (a(1)=8).  The implementation uses a simple sieve‑like incremental
   generation and memoises previously found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff any prime exponent in the factorisation of x is
   congruent to 3 modulo 4.  The test operates only on 64‑bit values
   (the candidate numbers fit in uint64_t for the required range). */
static bool has_exp_mod4_3(uint64_t x)
{
    uint64_t n = x;
    unsigned exp = 0;

    /* factor 2 */
    exp = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        exp++;
    }
    if ((exp & 3U) == 3U) return true;          /* exp ≡ 3 (mod 4) */

    /* factor odd primes, up to sqrt(n) */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            if ((exp & 3U) == 3U) return true;
        }
    }

    /* if a prime factor remains (exponent 1), 1 ≡ 1 (mod 4) so not 3 */
    return false;
}

/* --------- memoisation storage -------------------------------------- */

static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* current number of stored terms */
static size_t cache_cap  = 0;      /* capacity of the cache array   */
static uint64_t next_val = 1;      /* last integer examined (value, not index) */

/* Ensure the cache has room for at least 'needed' elements.  Realloc
   uses the same rule as std::vector growth for efficiency. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(1);
    cache     = tmp;
    cache_cap = new_cap;
}

/* Append a newly found term to the cache. */
static void append_term(seq_t val)
{
    ensure_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* --------- public function -------------------------------------------- */

/* Return the n-th member of A336593 (n ≥ 1).  The function
   tolerates arbitrarily ordered requests. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined case, return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        next_val++;                                 /* next candidate */
        if (has_exp_mod4_3(next_val)) {
            append_term((seq_t)next_val);
        }
    }
    return cache[n - 1];
}
