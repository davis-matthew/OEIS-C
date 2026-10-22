/*
 * A317378  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4, a(2) = 5, a(3) = 18, a(4) = 36, a(5) = 94, a(6) = 270,
 *            a(7) = 731, a(8) = 1973, a(9) = 5388, a(10) = 14732. For n > 10:
 *            a(n) = a(n-1) + 3*a(n-2) + 5*a(n-3) + 2*a(n-4) - 3*a(n-5) -
 *            10*a(n-6) - 8*a(n-7)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* size of allocated array (>= index of last element) */
static idx_t max_computed = 0; /* highest index we have computed so far */

/* Ensure that cache is large enough to hold index n (1‑based). */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort program (should not happen in tests). */
        abort();
    }
    /* Zero the newly allocated part so that we can check uninitialized
       values by a simple comparison to 0 (all terms are positive). */
    if (cache_size < new_size)
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
    cache = tmp;
    cache_size = new_size;
}

/* Initial values for 1 <= n <= 10. */
static const seq_t init_values[10] = {
    4,    /* a(1) */
    5,    /* a(2) */
    18,   /* a(3) */
    36,   /* a(4) */
    94,   /* a(5) */
    270,  /* a(6) */
    731,  /* a(7) */
    1973, /* a(8) */
    5388, /* a(9) */
    14732 /* a(10) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined for n = 0; return 0 for safety */

    /* Return already computed value. */
    if (n <= max_computed && cache[n] != 0)
        return cache[n];

    ensure_cache(n);

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init_values[i - 1];  /* init_values is 0‑based */
        } else {
            cache[i] = cache[i - 1]
                     + 3 * cache[i - 2]
                     + 5 * cache[i - 3]
                     + 2 * cache[i - 4]
                     - 3 * cache[i - 5]
                     - 10 * cache[i - 6]
                     - 8 * cache[i - 7];
        }
    }

    max_computed = n;
    return cache[n];
}
