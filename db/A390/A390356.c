/*
 * A390356  a(n) = A000045(A111035(n)).
 * https://oeis.org/A390356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000045(A111035(n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/fib.h"   /* fib(idx_t) */

/* ----- Modulo multiplication using repeated addition ----- */
static inline seq_t mod_mul(seq_t a, seq_t b, seq_t mod)
{
    /* a, b are guaranteed to be < mod */
    idx_t aa = (idx_t)a;
    idx_t bb = (idx_t)b;
    idx_t mm = (idx_t)mod;
    idx_t result = 0;
    while (bb) {
        if (bb & 1) {
            result += aa;
            if (result >= mm) result -= mm;
        }
        aa += aa;
        if (aa >= mm) aa -= mm;
        bb >>= 1;
    }
    return (seq_t)result;
}

/* ----- Fast doubling: compute F(n) and F(n+1) modulo mod ----- */
static void fib_pair_mod(idx_t n, idx_t mod, seq_t *fn, seq_t *fn1)
{
    if (mod == 0) {               /* Should never occur for n >= 1 */
        *fn  = 0;
        *fn1 = 0;
        return;
    }
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % (seq_t)mod;
        return;
    }

    /* Recurse to obtain (F(k), F(k+1)) for k = n >> 1 */
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);

    /* F(2k)   = F(k) * (2*F(k+1) – F(k))   */
    /* F(2k+1) = F(k)^2 + F(k+1)^2          */
    seq_t two_b  = mod_mul(b, (seq_t)2, (seq_t)mod);   /* 2*F(k+1) mod mod */
    seq_t temp   = two_b;
    if (temp < a) temp += (seq_t)mod;                 /* (2*b - a) mod mod */
    temp -= a;
    seq_t c = mod_mul(a, temp, (seq_t)mod);           /* F(2k)   */
    seq_t d = mod_mul(a, a, (seq_t)mod);
    d += mod_mul(b, b, (seq_t)mod);
    if (d >= (seq_t)mod) d -= (seq_t)mod;            /* F(2k+1) */

    if (n & 1) {
        *fn  = d;                                   /* F(2k+1) */
        *fn1 = (c + d) % (seq_t)mod;                 /* F(2k+2) */
    } else {
        *fn  = c;                                   /* F(2k)   */
        *fn1 = d;                                   /* F(2k+1) */
    }
}

/* ----- Test if k satisfies k | (F(k+2) – 1) ----- */
static inline bool satisfies(idx_t k)
{
    if (k == 1) return true;      /* 1 | (F_3 – 1) trivially true */
    seq_t rem, dummy;
    fib_pair_mod(k + 2, k, &rem, &dummy);  /* rem = F(k+2) mod k */
    return rem == (seq_t)1;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    /* Cache the indices k = A111035(i) for i >= 1 */
    static idx_t *idx_cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;       /* Next k to test */

    /* Grow the cache until it contains at least n entries */
    while (cache_len < n) {
        if (satisfies(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(idx_cache, new_cap * sizeof(idx_t));
                if (!tmp) abort();   /* allocation failure */
                idx_cache = tmp;
                cache_cap = new_cap;
            }
            idx_cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    /* Retrieve the n‑th index k and return the Fibonacci number at that position */
    idx_t k = idx_cache[n - 1];
    return fib(k);
}
