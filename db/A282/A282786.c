/*
 * A282786  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4·a(n-1) + 8·a(n-2) – 8·a(n-3) – 78·a(n-4) – 72·a(n-5) –
 *            16·a(n-6) + 64·a(n-7) – 33·a(n-8) + 52·a(n-9) – 24·a(n-10) +
 *            8·a(n-11) – 4·a(n-12) Initial values: a(1) = 1, a(2) = 8, a(3) =
 *            73, a(4) = 318, a(5) = 1747, a(6) = 8216, a(7) = 38027, a(8) =
 *            173722, a(9) = 773529, a(10) = 3412416, a(11...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Computes a(n) for sequence A282786, 1‑based. */
seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* largest index computed */
    static idx_t cache_cap = 0;  /* allocated size */

    if (n < 1) return (seq_t)0;  /* outside the defined range */

    /* ----- initialise cache on first call ----- */
    if (!cache) {
        /* allocate space for indices 0..12 at least */
        cache_cap = (n > 12) ? (n + 1) : 13;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */

        /* initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)73;
        cache[4] = (seq_t)318;
        cache[5] = (seq_t)1747;
        cache[6] = (seq_t)8216;
        cache[7] = (seq_t)38027;
        cache[8] = (seq_t)173722;
        cache[9] = (seq_t)773529;
        cache[10] = (seq_t)3412416;
        cache[11] = (seq_t)14880845;
        cache[12] = (seq_t)64319686;

        /* zero the rest of the allocated slots */
        for (size_t i = 13; i < cache_cap; ++i) cache[i] = (seq_t)0;
        cached_n = 12;
    }

    /* ----- grow cache if needed ----- */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* realloc failed */
        cache = tmp;
        /* initialise newly allocated entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* ----- return cached value if already computed ----- */
    if (n <= cached_n) return cache[(size_t)n];

    /* ----- compute missing values sequentially ----- */
    for (idx_t idx = cached_n + 1; idx <= n; ++idx) {
        size_t i = (size_t)idx;          /* safe conversion for indexing */

        if (i <= 12) {
            /* values 1..12 already stored during initialization */
            continue;
        }

        /* compute using the empirical recurrence, separating positive
           and negative contributions to avoid unsigned underflow */
        seq_t pos =  (cache[i-1] * (seq_t)4) + (cache[i-2] * (seq_t)8)
                    + (cache[i-7] * (seq_t)64) + (cache[i-9] * (seq_t)52)
                    + (cache[i-11] * (seq_t)8);

        seq_t neg =  (cache[i-3] * (seq_t)8) + (cache[i-4] * (seq_t)78)
                    + (cache[i-5] * (seq_t)72) + (cache[i-6] * (seq_t)16)
                    + (cache[i-8] * (seq_t)33) + (cache[i-10] * (seq_t)24)
                    + (cache[i-12] * (seq_t)4);

        cache[i] = pos - neg;
    }

    cached_n = n;
    return cache[(size_t)n];
}
