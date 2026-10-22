/*
 * A371668  Number of binary necklaces of length n which have more 00 than 01
 *          substrings.
 * https://oeis.org/A371668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*(n-7)*a(n-7) + 4*(29-5*n)*a(n-6) + (26*n-110)*a(n-5) +
 *            (77-23*n)*a(n-4) + (15*n-46)*a(n-3) + (22-10*n)*a(n-2) +
 *            5*(n-1)*a(n-1))/n Initial values: a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=5, a(5)=11, a(6)=19
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"  // provides seq_t mul_small(seq_t, unsigned int)

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = (idx_t)-1;   /* index of last computed value */

/* Resize cache so that it can hold indices up to new_cap-1.
   Existing values are preserved. */
static void ensure_cache(idx_t new_cap) {
    if (new_cap <= cache_cap) return;
    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    /* copy existing values */
    for (idx_t i = 0; i < cache_cap; ++i) new_mem[i] = cache[i];
    /* initialise extra slots with 0 */
    for (idx_t i = cache_cap; i < new_cap; ++i) new_mem[i] = 0;
    free(cache);
    cache = new_mem;
    cache_cap = new_cap;
}

/* initialise first 7 terms, if cache is empty or not large enough */
static void init_cache(void) {
    ensure_cache(7);
    seq_t init_vals[7] = {0, 1, 1, 1, 5, 11, 19};
    for (idx_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
    last_computed = 6;
}

seq_t a(idx_t n) {
    if (cache == NULL) init_cache();

    /* extend cache if necessary */
    if (n >= cache_cap) ensure_cache(n + 1);

    /* if value already computed, return it */
    if (n <= last_computed) return cache[n];

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 7) { /* should already be initialised */
            continue;
        }

        seq_t sum = 0;   /* will hold the numerator */

        /* Term for a(i-7): 8*(i-7) * a[i-7] (always positive) */
        unsigned int coeff = (unsigned int)(8 * (i - 7));
        sum += mul_small(cache[i - 7], coeff);

        /* Term for a(i-6): 4*(29-5*i) * a[i-6] */
        long long coeff64 = 4LL * (29 - 5LL * (long long)i);
        if (coeff64 >= 0) {
            sum += mul_small(cache[i - 6], (unsigned int)coeff64);
        } else {
            sum -= mul_small(cache[i - 6], (unsigned int)(-coeff64));
        }

        /* Term for a(i-5): (26*i-110) * a[i-5] (positive for i>=5) */
        coeff64 = 26LL * (long long)i - 110;
        sum += mul_small(cache[i - 5], (unsigned int)coeff64);

        /* Term for a(i-4): (77-23*i) * a[i-4] */
        coeff64 = 77 - 23LL * (long long)i;
        if (coeff64 >= 0) {
            sum += mul_small(cache[i - 4], (unsigned int)coeff64);
        } else {
            sum -= mul_small(cache[i - 4], (unsigned int)(-coeff64));
        }

        /* Term for a(i-3): (15*i-46) * a[i-3] (positive for i>=4) */
        coeff64 = 15LL * (long long)i - 46;
        sum += mul_small(cache[i - 3], (unsigned int)coeff64);

        /* Term for a(i-2): (22-10*i) * a[i-2] */
        coeff64 = 22 - 10LL * (long long)i;
        if (coeff64 >= 0) {
            sum += mul_small(cache[i - 2], (unsigned int)coeff64);
        } else {
            sum -= mul_small(cache[i - 2], (unsigned int)(-coeff64));
        }

        /* Term for a(i-1): 5*(i-1) * a[i-1] (positive for i>=1) */
        coeff = (unsigned int)(5 * (i - 1));
        sum += mul_small(cache[i - 1], coeff);

        /* Divide by i to get a[i] */
        cache[i] = sum / (seq_t)i;
    }

    last_computed = n;
    return cache[n];
}
