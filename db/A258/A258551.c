/*
 * A258551  Number of (n+1) X (5+1) 0..1 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A258551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 12, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A258551
   Formula (Empirical for n>13):
   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 4*a(n-5) + 6*a(n-6) - 4*a(n-7) + a(n-8)
   Initial terms:
   a(1)=480, a(2)=506, a(3)=1403, a(4)=2509, a(5)=5070, a(6)=8339,
   a(7)=12940, a(8)=18630, a(9)=26240, a(10)=35386, a(11)=46687,
   a(12)=59945, a(13)=76039
*/
#include <stddef.h>
#include <stdbool.h>

/* Utility for dynamic reallocation */
void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
        while (new_cap < needed) new_cap *= 2;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Return a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* highest index stored */
    static size_t cache_cap = 0;

    if (n == 0) return 0; /* undefined, but return 0 */

    /* initialise cache with first 13 terms */
    if (cache == NULL) {
        ensure_capacity(16, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[1] = (seq_t)480;
        cache[2] = (seq_t)506;
        cache[3] = (seq_t)1403;
        cache[4] = (seq_t)2509;
        cache[5] = (seq_t)5070;
        cache[6] = (seq_t)8339;
        cache[7] = (seq_t)12940;
        cache[8] = (seq_t)18630;
        cache[9] = (seq_t)26240;
        cache[10] = (seq_t)35386;
        cache[11] = (seq_t)46687;
        cache[12] = (seq_t)59945;
        cache[13] = (seq_t)76039;
        cache_size = 13;
    }

    /* return if already computed */
    if (n <= cache_size) return cache[n];

    /* extend cache up to n */
    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 13) {
            /* already initialised, skip */
            continue;
        } else {
            /* Compute using the recurrence */
            seq_t pos = 0, neg = 0;
            /* positive contributions */
            for (int j = 0; j < 4; ++j) pos += cache[i-1];
            for (int j = 0; j < 4; ++j) pos += cache[i-3];
            for (int j = 0; j < 6; ++j) pos += cache[i-6];
            pos += cache[i-8];
            /* negative contributions */
            for (int j = 0; j < 6; ++j) neg += cache[i-2];
            for (int j = 0; j < 4; ++j) neg += cache[i-5];
            for (int j = 0; j < 4; ++j) neg += cache[i-7];
            cache[i] = pos - neg;
        }
    }

    cache_size = n;
    return cache[n];
}
