/*
 * A248491  Number of length 2+5 0..n arrays with some three disjoint pairs in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-8) -
 *            2*a(n-9) + a(n-10) - 2*a(n-11) + a(n-12), a(1)=22, a(2)=183,
 *            a(3)=844, a(4)=2605, a(5)=6306, a(6)=13147, a(7)=24208,
 *            a(8)=40449, a(9)=64030, a(10)=96391, a(11)=139212, a(12)=195013
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 153, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */
#include <stdint.h>    /* for NULL */

/* static cache to memoize computed values */
static seq_t *cache = NULL;      /* cache[1] = a(1) ... */
static size_t cache_size = 0;    /* allocated elements (index 0..size-1) */
static size_t highest = 0;       /* highest index computed so far */

/* initial known values of the sequence */
static const seq_t init_vals[12] = {
    22, 183, 844, 2605, 6306, 13147,
    24208, 40449, 64030, 96391, 139212, 195013
};

/* allocate or enlarge cache to at least 'need' elements +1 (index 0 unused) */
static void ensure_cache(size_t need)
{
    if (cache_size >= need + 1) return;
    size_t new_size = need + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;
}

/* initialise cache with the first 12 terms */
static void init_cache(void)
{
    if (!cache) {
        cache_size = 13;   /* 0..12, we ignore 0 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < cache_size; ++i) cache[i] = 0;
        for (size_t i = 0; i < 12; ++i) cache[i + 1] = init_vals[i];
        highest = 12;
    }
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of range, should not happen */

    size_t idx = (size_t)n;          /* safe as n <= 10000 for tests */

    if (!cache) init_cache();

    /* if already computed, return cached value */
    if (idx <= highest) return cache[idx];

    /* extend cache if necessary */
    ensure_cache(idx);

    /* compute missing terms using the recurrence */
    for (size_t i = highest + 1; i <= idx; ++i) {
        /* positive contributions */
        seq_t pos = 0;
        seq_t tmp;

        tmp = cache[i - 1];
        tmp *= 2;
        pos += tmp;

        tmp = cache[i - 3];
        tmp *= 2;
        pos += tmp;

        tmp = cache[i - 8];
        tmp *= 3;
        pos += tmp;

        pos += cache[i - 10];
        pos += cache[i - 12];

        /* negative contributions */
        seq_t neg = 0;
        neg += cache[i - 2];

        tmp = cache[i - 4];
        tmp *= 3;
        neg += tmp;

        tmp = cache[i - 9];
        tmp *= 2;
        neg += tmp;

        tmp = cache[i - 11];
        tmp *= 2;
        neg += tmp;

        /* result = pos - neg (pos >= neg for all valid n) */
        cache[i] = pos - neg;
    }

    highest = idx;
    return cache[idx];
}
