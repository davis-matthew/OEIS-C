/*
 * A283606  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 670", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) for n > 17 Initial values:
 *            a(0) = 1, a(1) = 3, a(2) = 7, a(3) = 15, a(4) = 29, a(5) = 63,
 *            a(6) = 119, a(7) = 255, a(8) = 477, a(9) = 1023, a(10) = 1915,
 *            a(11) = 4095, a(12) = 7667, a(13) = 16383, a(14) = 30715, a(15)
 *            = 65535, a(16) = 122879, a(17) = 262143
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of sequence A283606.
 * The values for n >= 18 are computed by the recurrence
 * a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3).  To avoid recomputation
 * the sequence is cached in a static array that grows as needed. */
#include <stdlib.h>

/* initial values for n = 0 … 17 */
static const seq_t init_vals[18] = {
    (seq_t)1,   (seq_t)3,   (seq_t)7,   (seq_t)15,
    (seq_t)29,  (seq_t)63,  (seq_t)119, (seq_t)255,
    (seq_t)477, (seq_t)1023, (seq_t)1915, (seq_t)4095,
    (seq_t)7667, (seq_t)16383, (seq_t)30715,
    (seq_t)65535, (seq_t)122879, (seq_t)262143
};

/* cached values: cache[0] … cache[max_i] contain computed terms */
static seq_t *cache = NULL;
static idx_t  cap   = 0;  /* allocated size of cache */
static idx_t  max_i = 0;  /* highest index that has been computed */

/* grow the cache to be able to store values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cap)
        return;
    idx_t new_cap = cap ? cap << 1 : 16;
    if (new_cap <= n + 1)
        new_cap = n + 1;
    /* reallocate array – keep the old data if allocation fails */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        return;           /* allocation failed – keep old cache unchanged */
    cache = tmp;
    cap   = new_cap;
}

/* sequence a(n) */
seq_t a(idx_t n)
{
    /* return the pre‑computed value for n ≤ 17 */
    if (n < 18)
        return init_vals[n];

    /* lazily allocate the cache on first use */
    if (!cache) {
        cap   = 18;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* out of memory – return 0 as safe fallback */
        for (idx_t i = 0; i < 18; ++i)
            cache[i] = init_vals[i];
        max_i = 17;
    }

    /* if the value is already known, return it */
    if (n <= max_i)
        return cache[n];

    /* grow the cache to accommodate the requested index */
    ensure_capacity(n);

    /* compute terms that are not yet stored */
    for (idx_t i = max_i + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 4*a(i-2) - 4*a(i-3) for i > 17 */
        cache[i] = cache[i - 1] + (cache[i - 2] - cache[i - 3]) * (seq_t)4;
    }
    max_i = n;
    return cache[n];
}
