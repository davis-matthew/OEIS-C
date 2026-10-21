/*
 * A266771  Molien series for invariants of finite Coxeter group D_8
 *          (bisected).
 * https://oeis.org/A266771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^(2n) in
 *            1/((1-t^8)^2*(1-t^2)*(1-t^4)*(1-t^6)*(1-t^10)*(1-t^12)*(1-t^14))
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via recurrence from the denominator coefficients */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len = 0;     /* number of cached terms (cache_len = last+1) */
static size_t cache_cap = 0;     /* allocated capacity of cache */

static seq_t *dh = NULL;         /* denominator coefficients for even powers 0..32 */
static int dh_initialized = 0;   /* flag to initialise dh once */

/* Build the denominator coefficients dh[0..32] */
static void init_dh(void)
{
    if (dh_initialized) return;
    static int64_t cur[65] = {0};
    static int64_t tmp[65] = {0};
    int deg = 0;

    cur[0] = 1; /* initial polynomial 1 */

    /* (1 - t^2) */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 2] -= cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 2;

    /* (1 - t^4) */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 4] -= cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 4;

    /* (1 - t^6) */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 6] -= cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 6;

    /* (1 - t^8)^2 = 1 - 2t^8 + t^16 */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 8] -= 2 * cur[j];
        tmp[j + 16] += cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 16;

    /* (1 - t^10) */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 10] -= cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 10;

    /* (1 - t^12) */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 12] -= cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 12;

    /* (1 - t^14) */
    for (int j = 0; j <= deg; ++j)
    {
        tmp[j]     += cur[j];
        tmp[j + 14] -= cur[j];
    }
    for (int i = 0; i < 65; ++i) { cur[i] = tmp[i]; tmp[i] = 0; }
    deg += 14;

    dh = (seq_t *)malloc(sizeof(seq_t) * 33);
    for (int k = 0; k <= 32; ++k)
        dh[k] = (seq_t)cur[2 * k];

    dh_initialized = 1;
}

/* Ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* Grow cache if necessary */
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap;
    while (new_cap <= n + 1) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    cache = new_arr;
    cache_cap = new_cap;

    /* Compute new terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) { cache[0] = (seq_t)1; continue; }
        seq_t sum = (seq_t)0;
        idx_t maxk = (i < 32) ? i : 32;
        for (idx_t k = 1; k <= maxk; ++k)
            sum += dh[k] * cache[i - k];
        cache[i] = -sum;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    init_dh();
    ensure_cache(n);
    return cache[n];
}
