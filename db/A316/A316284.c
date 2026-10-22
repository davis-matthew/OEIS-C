/*
 * A316284  Number of n X 3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +a(n-2) +6*a(n-3) -23*a(n-4) -22*a(n-5)
 *            +7*a(n-6) +27*a(n-7) +24*a(n-8) -18*a(n-9) -10*a(n-10) for n>12
 *            a(1)=4, a(2)=21, a(3)=42, a(4)=125, a(5)=361, a(6)=987,
 *            a(7)=2840, a(8)=8177, a(9)=23078, a(10)=66092, a(11)=189934,
 *            a(12)=542549
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for 1 ≤ n ≤ 12 */
    static const seq_t base[13] = {
        (seq_t)0,
        (seq_t)4,   (seq_t)21,   (seq_t)42,   (seq_t)125,
        (seq_t)361, (seq_t)987,  (seq_t)2840, (seq_t)8177,
        (seq_t)23078,(seq_t)66092,(seq_t)189934,(seq_t)542549
    };

    if (n <= 12) return base[(size_t)n];

    /* Memoisation: cache values of a(1..cache_len) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* maximum index stored in cache */

    /* Grow cache if necessary */
    if (cache_len < 12) {
        size_t needed = 13; /* indices 0..12 */
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failed */
        cache = tmp;

        /* Initialise new entries with the base values */
        for (size_t i = 1; i <= 12; ++i) cache[i] = base[i];
        cache_len = 12;
    }

    if (cache_len < n) {
        size_t new_len = (size_t)n + 1;   /* indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failed */
        cache = tmp;
        /* No need to initialise beyond cache_len: values will be computed */
    }

    /* Compute missing terms using the order‑10 recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        size_t idx = (size_t)i;          /* cast for array indexing */

        /* Positive and negative contributions */
        seq_t pos = 3 * cache[idx-1] + cache[idx-2] + 6 * cache[idx-3]
                    + 7 * cache[idx-6] + 27 * cache[idx-7] + 24 * cache[idx-8];
        seq_t neg = 23 * cache[idx-4] + 22 * cache[idx-5]
                    + 18 * cache[idx-9] + 10 * cache[idx-10];

        cache[idx] = pos - neg;          /* pos ≥ neg for all n≥13 */
    }
    cache_len = n;                       /* cache newer values */

    return cache[(size_t)n];
}
