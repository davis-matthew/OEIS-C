/*
 * A253742  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          ne-sw antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A253742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 31*a(n-2) + 51*a(n-3) - 40*a(n-4) + 12*a(n-5)
 *            (valid for n ≥ 7); initial terms: a(1)=81, a(2)=450, a(3)=1998,
 *            a(4)=7803, a(5)=28107, a(6)=95940
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based cache, cache[0] unused */
static idx_t cache_cap = 0;     /* current allocated size */

/* compute a(n) for n ≥ 1, memoised */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* behaviour for n < 1 irrelevant */

    /* Ensure cache large enough */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure – fallback to 0 */
        /* initialise newly allocated slots to 0 */
        for (idx_t i = cache_cap; i <= n; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* fill cache lazily */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= cache_cap && cache[i] != (seq_t)0) continue; /* already computed */
        if (i == 1)            cache[i] = (seq_t)81;
        else if (i == 2)       cache[i] = (seq_t)450;
        else if (i == 3)       cache[i] = (seq_t)1998;
        else if (i == 4)       cache[i] = (seq_t)7803;
        else if (i == 5)       cache[i] = (seq_t)28107;
        else if (i == 6)       cache[i] = (seq_t)95940;
        else {                 /* recurrence for i ≥ 7 */
            seq_t t1 = (seq_t)9  * cache[i-1];
            seq_t t2 = (seq_t)31 * cache[i-2];
            seq_t t3 = (seq_t)51 * cache[i-3];
            seq_t t4 = (seq_t)40 * cache[i-4];
            seq_t t5 = (seq_t)12 * cache[i-5];
            seq_t pos = t1 + t3 + t5;          /* all positive parts */
            seq_t neg = t2 + t4;              /* all negative parts */
            cache[i] = pos - neg;             /* result is positive for n≥7 */
        }
    }
    return cache[n];
}
