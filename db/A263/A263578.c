/*
 * A263578  Positive values of k such that A014286(k) is divisible by k.
 * https://oeis.org/A263578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th positive integer k such that the weighted
 * Fibonacci sum A014286(k) = Σ_{i=1}^k i·Fib(i) is divisible by k.
 *
 * Since A014286(k) = k·Fib(k+2) – Fib(k+3) + 2, the divisibility test
 * reduces to Fib(k+3) ≡ 2 (mod k).  The code searches for k in
 * increasing order and caches the found values so that repeated calls
 * can return any term in O(1) after the first computation.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of seq_t and idx_t – supplied by the build. */

/* Compute Fibonacci(n) modulo mod.  n and mod are 64‑bit values.
 * Uses iterative fast doubling; all arithmetic is performed in
 * unsigned __int128 to avoid overflow when mod ≥ 2^32. */
static inline uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    uint64_t a = 0;       /* F(0) */
    uint64_t b = 1;       /* F(1) */

    /* Process bits of n from most significant to least. */
    int shift = 63;
    while ((n >> shift) == 0) /* skip leading zeros */
        --shift;
    for (; shift >= 0; --shift)
    {
        /* doubling step: compute F(2k), F(2k+1) from F(k),F(k+1) */
        uint64_t two_b = (b << 1) % mod;
        uint64_t temp  = (two_b + mod - a) % mod;              /* 2*b - a (mod) */
        uint64_t c = (unsigned __int128)a * temp % mod;        /* F(2k)  */
        uint64_t d = ( (unsigned __int128)a * a % mod
                     + (unsigned __int128)b * b % mod ) % mod; /* F(2k+1) */
        if ( (n >> shift) & 1ULL ) {
            /* odd n: F(2k+1), F(2k+2) */
            a = d;
            b = (c + d) % mod;
        } else {
            /* even n: F(2k),   F(2k+1) */
            a = c;
            b = d;
        }
    }
    return a;   /* F(n) mod mod */
}

/* Static cache of found k. */
static seq_t *cache   = NULL;   /* array of seq_t values */
static idx_t   cap     = 0;     /* current allocated capacity */
static idx_t   size    = 0;     /* number of entries stored */
static idx_t   next_k  = 1;     /* next integer to test   */

/* Ensure the cache has at least 'need' capacity. */
static void ensure_cache(idx_t need)
{
    if (need <= cap)
        return;
    idx_t new_cap = cap ? cap << 1 : 32;
    if (new_cap < need)
        new_cap = need;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = p;
    cap   = new_cap;
}

/* Return the nth term of the sequence, 1‑based.  For n < 1 the
 * function returns 0 (undefined value). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* behaviour before offset irrelevant */

    /* Generate terms until we have at least n entries. */
    while (size < n) {
        /* Find next k such that Fib(k+3) ≡ 2 (mod k). */
        for (;;) {
            uint64_t k = (uint64_t)next_k;
            /* fib_mod_uint64 requires 64‑bit mod; k fits in that. */
            uint64_t fk = fib_mod_uint64(k + 3, k);
            if (fk == 2 % k) {
                /* Store k in cache. */
                ensure_cache(size + 1);
                cache[size++] = (seq_t)k;
                ++next_k;
                break;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
