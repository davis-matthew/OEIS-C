/*
 * A327611  Number of length n reversible string structures that are not
 *          palindromic using exactly four different colors.
 * https://oeis.org/A327611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 10*a(n-2) - 60*a(n-3) + 145*a(n-4) +
 *            100*a(n-5) - 470*a(n-6) + 120*a(n-7) + 456*a(n-8) - 288*a(n-9)
 *            (n>9) Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=6,
 *            a(6)=37, a(7)=182, a(8)=876, a(9)=3920
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static seq_t cache[10001];   /* indices 0..10000; index 0 unused */
static idx_t max_cached = 0; /* highest n for which cache[n] is valid */

/* Copy the predefined first 9 terms into the cache.  Assumes n ≥ 1. */
static void init_cache_up_to(idx_t n)
{
    static const seq_t init_vals[10] = {
        0,         /* dummy for index 0 */
        0,         /* a(1) */
        0,         /* a(2) */
        0,         /* a(3) */
        1,         /* a(4) */
        6,         /* a(5) */
        37,        /* a(6) */
        182,       /* a(7) */
        876,       /* a(8) */
        3920       /* a(9) */
    };
    for (idx_t i = 1; i <= n; ++i)
        cache[i] = init_vals[i];
    max_cached = n;
}

/* Ensure that cache contains a(n) for all 1 ≤ k ≤ n.  Assumes n ≤ 10000. */
static void ensure_cache(idx_t n)
{
    if (n > 10000) {
        /* n is outside the fixed-size cache.  Compute a(n) directly.  */
        if (n <= 9) return;
        /* We could extend the cache or compute row by row; for n ≤ 10000 the
           loop below will eventually fill the cache, so just let it fall
           through. */
    }

    if (max_cached == 0) {
        /* First call: initialise up to min(n,9). */
        if (n <= 9) init_cache_up_to(n);
        else init_cache_up_to(9);
    }
    else if (n <= max_cached) {
        /* Already computed.  Nothing to do. */
        return;
    }

    /* Compute any missing terms, starting from max_cached+1. */
    for (idx_t k = max_cached + 1; k <= n && k <= 9; ++k) {
        /* For k ≤ 9, the value is already in init_vals; copy it. */
        static const seq_t init_vals[10] = {
            0, 0, 0, 0, 1, 6, 37, 182, 876, 3920
        };
        cache[k] = init_vals[k];
    }

    if (n > 9) {
        for (idx_t k = max_cached + 1; k <= n; ++k) {
            seq_t v = 8 * cache[k-1];
            v -= 10 * cache[k-2];
            v -= 60 * cache[k-3];
            v += 145 * cache[k-4];
            v += 100 * cache[k-5];
            v -= 470 * cache[k-6];
            v += 120 * cache[k-7];
            v += 456 * cache[k-8];
            v -= 288 * cache[k-9];
            cache[k] = v;
        }
    }

    if (n > max_cached) max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* offset starts at 1; undefined for n<1 */
    if (n > 10000) {
        /* For safety: fall back to computing directly if n > 10000.
           Since the problem only requires n up to about 10000, this
           branch is unlikely to be taken. */
        ensure_cache(10000);
        seq_t res = cache[10000];
        for (idx_t k = 10001; k <= n; ++k) {
            seq_t v = 8 * res;
            /* shift remaining terms; maintain a circular buffer of 9 values */
            /* For simplicity, recompute using array indices: */
            seq_t t1 = cache[k-1];
            seq_t t2 = cache[k-2];
            seq_t t3 = cache[k-3];
            seq_t t4 = cache[k-4];
            seq_t t5 = cache[k-5];
            seq_t t6 = cache[k-6];
            seq_t t7 = cache[k-7];
            seq_t t8 = cache[k-8];
            seq_t t9 = cache[k-9];
            v = 8 * t1 - 10 * t2 - 60 * t3 + 145 * t4 + 100 * t5
                - 470 * t6 + 120 * t7 + 456 * t8 - 288 * t9;
            res = v;
        }
        return res;
    }

    ensure_cache(n);
    return cache[n];
}
