/*
 * A290804  One of the two successive approximations up to 7^n for the 7-adic
 *          integer sqrt(-3). These are the numbers congruent to 5 mod 7
 *          (except for the initial 0).
 * https://oeis.org/A290804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=5, a(n) = a(n-1)+2*(a(n-1)^2+3) mod 7^n for n>1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following implementation uses 128‑bit arithmetic for
   integer seq_t types up to 64 bits, which suffices for all
   n where the result still fits in seq_t.  For larger seq_t
   types it falls back to ordinary big‑integer arithmetic. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;        /* number of stored values */
    static idx_t next_index = 0;        /* next index to compute */

    /* state of the current modulus power of 7 */
    static unsigned __int128 mod128 = 7;     /* 7^(next_index-1) for 64‑bit seq_t */
    static seq_t mod_big = (seq_t)7;         /* 7^(next_index-1) for big seq_t */

    /* flag for small seq_t */
    const bool use128 = sizeof(seq_t) <= 8U;

    if (cache == NULL) {                 /* first call initialisation */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)5;
        cache_len = 2;
        next_index = 2;
        /* mod128 and mod_big are already initialised to 7 = 7^(next_index-1) */
        return (n <= 1) ? cache[n] : 0;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* enlarge cache */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* reallocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing entries */
    for (idx_t i = next_index; i <= n; ++i) {
        if (use128) {
            mod128 *= (unsigned __int128)7;   /* 7^i */
            unsigned __int128 mod = mod128;

            unsigned __int128 prev = (unsigned __int128)cache[i - 1];
            unsigned __int128 sq = prev * prev;   /* prev^2 */
            sq = (sq + (unsigned __int128)3) * (unsigned __int128)2;  /* 2*(prev^2+3) */
            sq %= mod;                            /* mod 7^i */

            unsigned __int128 val = (prev + sq) % mod;
            cache[i] = (seq_t)val;
        } else {
            mod_big *= (seq_t)7;                /* 7^i */
            seq_t mod = mod_big;

            seq_t prev = cache[i - 1];
            seq_t sq = (prev * prev) + (seq_t)3; /* prev^2 + 3 */
            sq = sq * (seq_t)2;                 /* 2*(prev^2+3) */
            sq %= mod;

            seq_t val = (prev + sq) % mod;
            cache[i] = val;
        }
    }
    cache_len = (size_t)n + 1;
    next_index = n + 1;
    return cache[n];
}
