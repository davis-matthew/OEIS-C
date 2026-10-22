/*
 * A395482  Number of derangements of [n] with 3 descents.
 * https://oeis.org/A395482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 105*a(n-2) + 362*a(n-3) - 692*a(n-4) +
 *            696*a(n-5) - 288*a(n-6) for n > 9 with initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=12, a(6)=120, a(7)=896,
 *            a(8)=5544, a(9)=30384
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Internal helper: extend the cache up to at least (n+1) elements. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* Base values for n = 0..9 */
    static const seq_t base[10] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)12, (seq_t)120, (seq_t)896,
        (seq_t)5544, (seq_t)30384
    };

    /* Initialise cache if this is the first call */
    if (cache_len == 0) {
        size_t init_len = (needed < 10 ? needed : 10);
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < init_len; ++i) cache[i] = base[i];
        cache_len = init_len;
    }

    /* If we still don't have all base values, fill them now */
    if (cache_len < 10 && needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, 10 * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        for (size_t i = cache_len; i < 10; ++i) cache[i] = base[i];
        cache_len = 10;
    }

    /* Extend until we have enough for the requested n */
    if (needed <= cache_len) return;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    for (size_t i = cache_len; i < needed; ++i) {
        /* Positive part: 16*a[i-1] + 362*a[i-3] + 696*a[i-5] */
        seq_t pos = (seq_t)16 * cache[i - 1]
                  + (seq_t)362 * cache[i - 3]
                  + (seq_t)696 * cache[i - 5];
        /* Negative part: 105*a[i-2] + 692*a[i-4] + 288*a[i-6] */
        seq_t neg = (seq_t)105 * cache[i - 2]
                  + (seq_t)692 * cache[i - 4]
                  + (seq_t)288 * cache[i - 6];
        cache[i] = pos - neg;   /* pos > neg for all n ≥ 10 */
    }

    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)cache_len) return cache[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}
