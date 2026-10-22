/*
 * A314108  Coordination sequence Gal.6.198.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) - a(n‑2) - a(n‑4) + 2*a(n‑5) - a(n‑6) - a(n‑8) +
 *            2*a(n‑9) - a(n‑10) for n > 10; initial values: a(0)=1, a(1)=5,
 *            a(2)=11, a(3)=16, a(4)=21, a(5)=26, a(6)=30, a(7)=34, a(8)=39,
 *            a(9)=44, a(10)=49
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial terms for n = 0 … 10.  They are stored in a read‑only
       array so that the values are available at compile time and are
       never modified by the algorithm. */
    static const seq_t init[11] = {
        1, 5, 11, 16, 21, 26, 30, 34, 39, 44, 49
    };

    /* Cached values.  Cache[0]..Cache[max_computed] hold consecutive
       terms of the sequence.  The array is grown as necessary. */
    static seq_t *cache      = NULL;      /* pointer to cached values   */
    static idx_t max_computed = 0;        /* largest index that has been cached */
    static size_t capacity    = 0;        /* allocated array size        */

    /* Allocate and initialise the cache on first call. */
    if (!cache) {
        capacity = 64;                                           /* initial size */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        for (size_t i = 0; i < capacity; ++i) cache[i] = 0;

        /* copy the 11 known initial terms */
        for (size_t i = 0; i < 11; ++i)
            cache[i] = init[i];

        max_computed = 10;
    }

    /* Resize the array if the required index does not fit. */
    if (n + 1 > capacity) {
        size_t new_cap = capacity;
        while (n + 1 > new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            for (size_t i = capacity; i < new_cap; ++i)
                cache[i] = 0;
            capacity = new_cap;
        } else
            return 0;   /* allocation failed – return zero as a safe fallback */
    }

    /* The value is already cached. */
    if (n <= max_computed)
        return cache[n];

    /* Compute the sequence forward from max_computed+1 up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term;
        if (i <= 10)                     /* safety check – should not happen */
            term = init[i];
        else {
            /* Positives: 2*a(i-1) + 2*a(i-5) + 2*a(i-9) */
            /* Negatives: a(i-2) + a(i-4) + a(i-6) + a(i-8) + a(i-10) */
            seq_t pos = 2 * cache[i - 1] + 2 * cache[i - 5] + 2 * cache[i - 9];
            seq_t neg = cache[i - 2] + cache[i - 4] + cache[i - 6]
                      + cache[i - 8] + cache[i -10];
            term = pos - neg;
        }
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
