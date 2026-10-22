/*
 * A348663  Number of tree polyominoes with width 3 and height n.
 * https://oeis.org/A348663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) -21*a(n-2) +15*a(n-3) +34*a(n-4) -101*a(n-5)
 *            +120*a(n-6) -66*a(n-7) +12*a(n-8) (n ≥ 9) Initial terms: a(1)=1,
 *            a(2)=10, a(3)=55, a(4)=243, a(5)=980, a(6)=3789, a(7)=14357,
 *            a(8)=53832
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;            /* 1‑based array of computed values */
static size_t cache_size = 0;          /* allocated capacity (elements) */
static size_t cache_last  = 0;         /* highest index computed (≥0) */

/* First eight terms of the sequence */
static const seq_t init_vals[8] = {
    (seq_t)1,
    (seq_t)10,
    (seq_t)55,
    (seq_t)243,
    (seq_t)980,
    (seq_t)3789,
    (seq_t)14357,
    (seq_t)53832
};

/* Ensure that cache contains values up to 'target' (inclusive). */
static void ensure_cache_up_to(size_t target) {
    if (target <= cache_last)
        return;                                   /* already available */

    /* Grow the cache if needed. */
    if (target + 1 > cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 16;
        while (new_cap <= target)
            new_cap <<= 1;                       /* double until large enough */

        /* Use a C‑style cast so the code compiles in both C and C++. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                            /* out of memory */

        cache = tmp;
        cache_size = new_cap;
    }

    /* Initialise first eight terms if the cache was previously smaller. */
    if (cache_last < 8) {
        size_t start = cache_last + 1;
        if (start < 1)
            start = 1;
        for (size_t i = start; i <= 8 && i <= target; ++i)
            cache[i] = init_vals[i - 1];
    }

    /* Compute further terms using the recurrence. */
    for (size_t i = cache_last + 1; i <= target; ++i) {
        if (i <= 8)                       /* already set */
            continue;

        /* Perform the recurrence term‑wise.  Use the smallest possible
           intermediate type to avoid overflow in 64‑bit builds.  The
           expression is split into additions and subtractions to minimize
           temporary magnitudes. */
        seq_t val = (seq_t)0;
        val += (seq_t)8   * cache[i - 1];
        val -= (seq_t)21  * cache[i - 2];
        val += (seq_t)15  * cache[i - 3];
        val += (seq_t)34  * cache[i - 4];
        val -= (seq_t)101 * cache[i - 5];
        val += (seq_t)120 * cache[i - 6];
        val -= (seq_t)66  * cache[i - 7];
        val += (seq_t)12  * cache[i - 8];
        cache[i] = val;
    }

    cache_last = target;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;                       /* sequence undefined for n < 1 */

    size_t idx = (size_t)n;                    /* safe conversion */

    if (idx <= 8)
        return init_vals[idx - 1];

    ensure_cache_up_to(idx);
    return cache[idx];
}
