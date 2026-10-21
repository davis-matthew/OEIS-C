/*
 * A260034  Number of configurations of the general monomer-dimer model for a
 *          4 X 2n square lattice.
 * https://oeis.org/A260034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 163*a(n-1) - 2641*a(n-2) + 12479*a(n-3) - 22577*a(n-4) +
 *            16705*a(n-5) - 5331*a(n-6) + 769*a(n-7) - 47*a(n-8) + a(n-9)
 *            (for n ≥ 9, with initial values a(0)…a(8) given below)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Initial 9 terms of the sequence */
static const seq_t init_vals[9] = {
    (seq_t)1ULL,                   /* a(0) */
    (seq_t)71ULL,                  /* a(1) */
    (seq_t)10012ULL,               /* a(2) */
    (seq_t)1453535ULL,             /* a(3) */
    (seq_t)211351945ULL,           /* a(4) */
    (seq_t)30734932553ULL,         /* a(5) */
    (seq_t)4469527322891ULL,       /* a(6) */
    (seq_t)649966808093412ULL,     /* a(7) */
    (seq_t)94519361817920403ULL    /* a(8) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t cache_size = 0;  /* number of terms stored */

    /* Return the precomputed initial term if requested */
    if (n < 9)
        return init_vals[n];

    /* Grow cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_buf = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_buf) exit(1);                 /* allocation failed */
        cache = new_buf;
        /* Initialize newly allocated space with initial values */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 9)
                cache[i] = init_vals[i];
        }
        cache_size = new_size;

        /* Compute terms from old_size to n using the recurrence */
        for (idx_t i = 9; i < cache_size; ++i) {
            seq_t val = (seq_t)163    * cache[i-1];
            val -= (seq_t)2641       * cache[i-2];
            val += (seq_t)12479      * cache[i-3];
            val -= (seq_t)22577      * cache[i-4];
            val += (seq_t)16705      * cache[i-5];
            val -= (seq_t)5331       * cache[i-6];
            val += (seq_t)769        * cache[i-7];
            val -= (seq_t)47         * cache[i-8];
            val +=                 cache[i-9];
            cache[i] = val;
        }
    }

    return cache[n];
}
