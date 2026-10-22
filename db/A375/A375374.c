/*
 * A375374  Let X be the sequence of power towers built of 2's and 3's, sorted
 *          first by their height and then colexicographically: 2, 3, 2^2,
 *          3^2, 2^3, 3^3, 2^2^2, 3^2^2, etc. Sequence gives the permutation
 *          of indices which reorders X by magnitude.
 * https://oeis.org/A375374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1) = 2*a(n-1)+1, a(2n) = a(2n-1)+1 (for n >= 7) Initial
 *            values: a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=4, a(6)=7, a(7)=6,
 *            a(8)=8, a(9)=11, a(10)=9, a(11)=12, a(12)=10
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache of computed values */
static size_t cache_capacity = 0;/* number of slots allocated in cache */

/* Ensure cache has space for indices up to n (inclusive). */
static void _ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);           /* include index 0 */
    if (needed <= cache_capacity) return;

    /* at least 13 slots: indices 1..12 are guaranteed to have known values */
    if (needed < 13) needed = 13;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();  /* out‑of‑memory: terminate */
    cache = tmp;
    cache_capacity = needed;

    /* initialise base values 1..12 (only if within current capacity) */
    if (cache_capacity > 1) cache[1] = (seq_t)1;          /* a(1) */
    if (cache_capacity > 2) cache[2] = (seq_t)2;          /* a(2) */
    if (cache_capacity > 3) cache[3] = (seq_t)3;          /* a(3) */
    if (cache_capacity > 4) cache[4] = (seq_t)5;          /* a(4) */
    if (cache_capacity > 5) cache[5] = (seq_t)4;          /* a(5) */
    if (cache_capacity > 6) cache[6] = (seq_t)7;          /* a(6) */
    if (cache_capacity > 7) cache[7] = (seq_t)6;          /* a(7) */
    if (cache_capacity > 8) cache[8] = (seq_t)8;          /* a(8) */
    if (cache_capacity > 9) cache[9] = (seq_t)11;         /* a(9) */
    if (cache_capacity > 10) cache[10] = (seq_t)9;        /* a(10) */
    if (cache_capacity > 11) cache[11] = (seq_t)12;       /* a(11) */
    if (cache_capacity > 12) cache[12] = (seq_t)10;       /* a(12) */
}

/* Compute all values up to n (inclusive) using the recurrence. */
static void _compute_up_to(idx_t n)
{
    for (idx_t k = 7; 2 * k - 1 <= n; ++k) {
        seq_t val = 2 * cache[k - 1] + (seq_t)1;
        cache[2 * k - 1] = val;
        if (2 * k <= n) cache[2 * k] = val + (seq_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but handle gracefully */

    _ensure_capacity(n);
    _compute_up_to(n);

    /* For indices <=12 the values were set directly in _ensure_capacity. */
    /* For larger indices, _compute_up_to has filled them. */
    return cache[n];
}
