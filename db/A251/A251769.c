/*
 * A251769  Numbers n such that the sum of the heptagonal numbers H(n), H(n+1)
 *          and H(n+2) is equal to the sum of three consecutive squares.
 * https://oeis.org/A251769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1442*a(n-2) – 1442*a(n-3) – a(n-4) + a(n-5)
 *            Initial values: a(1)=17, a(2)=239, a(3)=25529, a(4)=345647,
 *            a(5)=36813809
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    /* Already up to date? */
    if (cache_len >= n) return;

    /* Allocate or grow the cache array. */
    if (!cache) {
        /* First allocation: at least the first five terms. */
        cache_cap = (n > 5 ? n : 5);
        cache = (seq_t *)malloc((size_t)(cache_cap + 1) * sizeof(seq_t));
        if (!cache) return; /* allocation failed */
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)239;
        cache[3] = (seq_t)25529;
        cache[4] = (seq_t)345647;
        cache[5] = (seq_t)36813809;
        cache_len = 5;
    } else if (cache_cap < n) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n + 5;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
        if (!tmp) return; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1];
        seq_t v2 = (seq_t)1442 * cache[i - 2];
        seq_t v3 = (seq_t)1442 * cache[i - 3];
        seq_t v4 = cache[i - 4];
        seq_t v5 = cache[i - 5];
        cache[i] = v1 + v2 - v3 - v4 + v5;
    }
    if (n > cache_len) cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1; return 0 */
    ensure_cache(n);
    return cache[n];
}
