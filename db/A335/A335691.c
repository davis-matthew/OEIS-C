/*
 * A335691  A000166(n)^2.
 * https://oeis.org/A335691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (!n)^2, where !n is the number of derangements
 *            (subfactorial)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A335691. */

#include <stdlib.h>

static seq_t *subfact = NULL;    /* cache of subfactorials */
static size_t cache_cap = 0;     /* allocated capacity in the cache */
static idx_t cache_size = 0;     /* number of computed subfactorials (subfact[0..cache_size-1]) */

/* Return the value of the sequence at index n. */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold subfact[n] */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(subfact, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – not expected */
        subfact = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base cases if the cache was empty */
    if (cache_size == 0) {
        subfact[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        subfact[1] = (seq_t)0;
        cache_size = 2;
    }

    /* Compute missing subfactorials using the recurrence:
     * !k = (k-1)(!k-1 + !k-2)
     */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = subfact[i - 1] + subfact[i - 2];
        subfact[i] = ((seq_t)(i - 1)) * sum;
    }
    cache_size = (idx_t)(n + 1);

    /* Return the square of the subfactorial. */
    return subfact[n] * subfact[n];
}
