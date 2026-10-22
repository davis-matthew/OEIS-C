/*
 * A310039  Coordination sequence Gal.3.7.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 6*a(n-3) - 5*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) + 3*a(n-7) - 5*a(n-8) + 6*a(n-9) - 5*a(n-10) +
 *            3*a(n-11) - a(n-12); a(0)=1, a(1)=3, a(2)=5, a(3)=9, a(4)=12,
 *            a(5)=15, a(6)=17, a(7)=19, a(8)=24, a(9)=29, a(10)=31, a(11)=33,
 *            a(12)=36, a(13)=39, a(14)=41
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Pre‑computed initial values (a(0) … a(14)). */
static const seq_t base_vals[15] = {
    1ULL, 3ULL, 5ULL, 9ULL, 12ULL, 15ULL, 17ULL, 19ULL,
    24ULL, 29ULL, 31ULL, 33ULL, 36ULL, 39ULL, 41ULL
};

static seq_t *cache = NULL;      /* dynamically enlarged cache of values   */
static size_t cache_cap = 0;     /* allocated capacity (in elements)      */
static size_t cache_len = 0;     /* first unused index (total stored)   */

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    /* Convert the index to a size_t; all meaningful indices are very
       small compared with SIZE_MAX, so truncation is safe. */
    size_t idx = (size_t)n;

    /* Return cached value if already available. */
    if (idx < cache_len)
        return cache[idx];

    /* Grow the cache so that it can hold all indices up to idx. */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap <= idx)
            new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure – give up */
        cache      = tmp;
        cache_cap  = new_cap;

#ifdef __cplusplus
        /* In C++ construct the newly allocated objects. */
        for (size_t i = cache_len; i < cache_cap; ++i)
            new (cache + i) seq_t();
#endif
    }

    /* Compute missing values up to idx (including). */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 15) {
            cache[i] = base_vals[i];
        } else {
            /* Compute the 12‑term linear recurrence. */
            seq_t pos =
                3 * cache[i - 1] + 6 * cache[i - 3] + 3 * cache[i - 5] +
                3 * cache[i - 7] + 6 * cache[i - 9] + 3 * cache[i - 11];
            seq_t neg =
                5 * cache[i - 2] + 5 * cache[i - 4] + 2 * cache[i - 6] +
                5 * cache[i - 8] + 5 * cache[i - 10] + cache[i - 12];
            cache[i] = pos - neg;
        }
    }

    cache_len = idx + 1;
    return cache[idx];
}
