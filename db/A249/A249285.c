/*
 * A249285  Number of length n+3 0..3 arrays with no four consecutive terms
 *          having the sum of any three elements equal to three times the
 *          fourth.
 * https://oeis.org/A249285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)+4*a(n-3)+8*a(n-4)+4*a(n-5)
 *            -8*a(n-6)-2*a(n-7)+3*a(n-8) for n ≥ 9 initial values: a(1)=204,
 *            a(2)=660, a(3)=2144, a(4)=6960, a(5)=22572, a(6)=73204,
 *            a(7)=237480, a(8)=770416
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t  cached_until = 0;  /* highest n already computed (1‑based) */
static size_t cached_cap   = 0;  /* allocated array capacity (in seq_t) */

static const seq_t init_vals[8] =
{
    (seq_t)204,    /* a(1) */
    (seq_t)660,    /* a(2) */
    (seq_t)2144,   /* a(3) */
    (seq_t)6960,   /* a(4) */
    (seq_t)22572,  /* a(5) */
    (seq_t)73204,  /* a(6) */
    (seq_t)237480, /* a(7) */
    (seq_t)770416  /* a(8) */
};

static void ensure_cap(size_t need)
{
    if (cached_cap < need) {
        size_t new_cap = cached_cap ? cached_cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* In a stress test we could abort here, but it is safe to assume success. */
        cached_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* First time: allocate and prime the cache with the first 8 terms */
    if (cached_until == 0) {
        ensure_cap(8);
        for (idx_t i = 1; i <= 8; ++i)
            cache[i-1] = init_vals[i-1];
        cached_until = 8;
    }

    if (n <= cached_until)
        return cache[n-1];

    /* Grow the cache to accommodate up to n */
    ensure_cap((size_t)n);

    /* Compute missing terms */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {
            /* These values are already in init_vals but recomputing is harmless */
            val = init_vals[i-1];
        } else {
            val  = (seq_t)2 * cache[i-2];
            val += (seq_t)2 * cache[i-3];
            val += (seq_t)4 * cache[i-4];
            val += (seq_t)8 * cache[i-5];
            val += (seq_t)4 * cache[i-6];
            val -= (seq_t)8 * cache[i-7];
            val -= (seq_t)2 * cache[i-8];
            val += (seq_t)3 * cache[i-9];
        }
        cache[i-1] = val;
    }

    cached_until = n;
    return cache[n-1];
}
