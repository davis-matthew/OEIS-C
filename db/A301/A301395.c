/*
 * A301395  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A301395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 3*a(n-1) + 2*a(n-2) - a(n-3) + 3*a(n-4) + 3*a(n-5) -8*a(n-6)
 *         -13*a(n-7) -13*a(n-8) -7*a(n-9) -8*a(n-10) -7*a(n-11) -a(n-12)
 *         +a(n-13) -2*a(n-14) +a(n-15) ,  n ≥ 16
 *  Initial values: a(1)=0, a(2)=2, a(3)=3, a(4)=13, a(5)=44,
 *                  a(6)=156, a(7)=554, a(8)=1963, a(9)=6964,
 *                  a(10)=24699, a(11)=87617, a(12)=310815,
 *                  a(13)=1102602, a(14)=3911438, a(15)=13875683 */

#include <stdlib.h>

static const seq_t init_vals[15] = {
    0,      /* a(1) */
    2,      /* a(2) */
    3,      /* a(3) */
    13,     /* a(4) */
    44,     /* a(5) */
    156,    /* a(6) */
    554,    /* a(7) */
    1963,   /* a(8) */
    6964,   /* a(9) */
    24699,  /* a(10) */
    87617,  /* a(11) */
    310815, /* a(12) */
    1102602,/* a(13) */
    3911438,/* a(14) */
    13875683/* a(15) */
};

static const int coeff[15] = {
    3,  2, -1,  3,  3, -8, -13, -13, -7, -8, -7, -1,  1, -2,  1
};

static seq_t *cache = NULL;   /* 1‑based cache, cache[0] unused */
static idx_t cache_len = 0;   /* largest index presently stored */

/* Allocate or enlarge the cache to hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    size_t new_len = (size_t)(n + 1); /* indices 0 … n   */
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;

    /* Initialize newly added entries to zero (will be overwritten as needed). */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = 0;

    cache_len = n;
}

/* Initialize the cache with the first 15 known terms. */
static void init_cache(void)
{
    if (cache_len > 0)
        return; /* already initialized */

    const idx_t init_len = 15; /* number of base terms */
    cache = (seq_t *)malloc((size_t)(init_len + 1) * sizeof(seq_t));
    if (!cache)
        abort();

    cache[0] = 0; /* unused */
    for (idx_t i = 1; i <= init_len; ++i)
        cache[i] = init_vals[i - 1];

    cache_len = init_len;
}

/* Return a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset=1, safe for callers */

    init_cache();

    if (n <= cache_len)
        return cache[n];

    /* Grow cache and compute new terms up to n. */
    idx_t old_len = cache_len;
    ensure_cache(n);

    for (idx_t i = old_len + 1; i <= n; ++i) {
        if (i <= 15) {
            cache[i] = init_vals[i - 1];
            continue;
        }

        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 0; k < 15; ++k) {
            idx_t idx = i - 1 - (idx_t)k; /* indices i-1 down to i-15 */
            if (coeff[k] >= 0)
                pos_sum += cache[idx] * (seq_t)coeff[k];
            else
                neg_sum += cache[idx] * (seq_t)(-coeff[k]);
        }
        cache[i] = pos_sum - neg_sum;
    }

    return cache[n];
}
