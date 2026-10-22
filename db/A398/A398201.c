/*
 * A398201  Upper (4/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7), * with
 *            (a(0),...,a(6)) = (0,2,7,14,24,37,53)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed values   */
static idx_t  capacity = 0;      /* allocated size of cache         */
static idx_t  length   = 0;      /* number of values currently stored */

static void grow_cache_size(idx_t new_len)
{
    if (capacity > new_len) return;
    idx_t new_cap = capacity ? capacity : 8;
    while (new_cap <= new_len) new_cap <<= 1;          /* double until large enough */
    seq_t *tmp = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                                  /* abort on OOM */
    if (cache) {
        for (idx_t i = 0; i < length; ++i) tmp[i] = cache[i];
        free(cache);
    }
    cache    = tmp;
    capacity = new_cap;
}

static void init_base(void)
{
    static const seq_t init_vals[7] = {0, 2, 7, 14, 24, 37, 53};
    if (length >= 7) return;
    grow_cache_size(6);                        /* ensure room for indices 0..6 */
    for (idx_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
    if (length < 7) length = 7;
}

static void ensure_cache(idx_t n)
{
    if (n < length) return;
    init_base();
    grow_cache_size(n);
    for (idx_t i = length; i <= n; ++i) {
        seq_t val = cache[i - 1] * 2;          /* 2*a(i-1) */
        val -= cache[i - 2];                  /* - a(i-2) */
        val += cache[i - 5];                  /* + a(i-5) */
        val -= cache[i - 6] * 2;              /* - 2*a(i-6) */
        val += cache[i - 7];                  /* + a(i-7) */
        cache[i] = val;
    }
    length = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 7) {
        static const seq_t init_vals[7] = {0, 2, 7, 14, 24, 37, 53};
        return init_vals[n];
    }
    ensure_cache(n);
    return cache[n];
}
