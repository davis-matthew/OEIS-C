/*
 * A393454  Integers k, not divisible by any of their digits, such that Sum_{j
 *          = digits of k} k/j is an integer different from k.
 * https://oeis.org/A393454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th integer k (n ≥ 1) that
   – contains no zero digit, and no non‑zero digit divides k,
   – the sum of k/j over its decimal digits j is an integer different
     from k.  The sequence is computed lazily and cached for future calls. */

#include <stdbool.h>
#include <stdlib.h>

/* LCM of 1…9: 2520 */
static const int DEN = 2520;
/* 2520/d for d = 1…9 (index 0 unused) */
static const int NUM[10] = {
    0,     /* 0 unused */
    2520, /* 2520/1 */
    1260, /* 2520/2 */
     840, /* 2520/3 */
     630, /* 2520/4 */
     504, /* 2520/5 */
     420, /* 2520/6 */
     360, /* 2520/7 */
     315, /* 2520/8 */
     280  /* 2520/9 */
};

/* Test whether a candidate number k satisfies the conditions. */
static bool ok (idx_t k)
{
    int sum_num = 0;               /* Σ 2520/d over the digits of k  */
    idx_t t = k;

    /* Examine each decimal digit. */
    while (t > 0) {
        int d = (int)(t % 10);
        t /= 10;
        if (d == 0) return false;                    /* zero digit forbidden */
        if ((k % (idx_t)d) == 0) return false;        /* digit divides k  */
        sum_num += NUM[d];
    }

    if (sum_num == DEN) return false;                /* sum equals k   */
    /* Check that k * sum_num / DEN is an integer.
       Using modular arithmetic to avoid overflow.             */
    return ((k % DEN) * (idx_t)sum_num) % DEN == 0;
}

/* Cached list of found terms.  The array grows lazily. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;   /* start searching from 1 */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache_capacity (size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a (idx_t n)
{
    /* We want at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        idx_t k = next_candidate;
        while (!ok(k)) ++k;           /* find next qualifying integer */

        /* Store k in the cache. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)k;

        next_candidate = k + 1;       /* next search starts after k   */
    }
    return cache[n - 1];
}
