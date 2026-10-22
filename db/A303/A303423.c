/*
 * A303423  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 5 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) +12*a(n-2) +175*a(n-3) +283*a(n-4) -235*a(n-5)
 *            -144*a(n-6) +67*a(n-7) -135*a(n-8) +50*a(n-9) Initial values:
 *            a(1)=8, a(2)=128, a(3)=1988, a(4)=30897, a(5)=480953,
 *            a(6)=7486281, a(7)=116517744, a(8)=1813509273, a(9)=28226012078
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 0;                /* last index that has been computed */

/* first 9 terms (index 0 unused) */
static const seq_t initial[10] = {
    0,      /* 0 – unused */
    8,      /* 1 */
    128,    /* 2 */
    1988,   /* 3 */
    30897,  /* 4 */
    480953, /* 5 */
    7486281,/* 6 */
    116517744,/* 7 */
    1813509273,/* 8 */
    28226012078/* 9 */
};

/* Ensure cache has at least n+1 elements.  If it grows, newly allocated
   slots are zeroed. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* we use indexes 1..n inclusive */
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        abort();

    /* zero newly allocated part */
    if (new_cap > cache_cap) {
        size_t old = cache_cap;
        for (size_t i = old; i < new_cap; ++i)
            p[i] = 0;
    }

    cache = p;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* Small n handled by the initial table. */
    if (n <= 9)
        return initial[n];

    ensure_cache(n);

    if (cache_last == 0) {            /* first use – copy the known terms */
        for (size_t i = 1; i <= 9; ++i)
            cache[i] = initial[i];
        cache_last = 9;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t val = 0;
        /* Add positive contributions first */
        val += 14 * cache[i-1];
        val += 12 * cache[i-2];
        val += 175 * cache[i-3];
        val += 283 * cache[i-4];
        val += 67 * cache[i-7];
        val += 50 * cache[i-9];
        /* Subtract negative contributions */
        val -= 235 * cache[i-5];
        val -= 144 * cache[i-6];
        val -= 135 * cache[i-8];

        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
