/*
 * A275863  Floor(r*a(n-1)) + floor(r*a(n-2)), where r = 5/2, a(0) = 1, a(1) =
 *          1.
 * https://oeis.org/A275863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5/2)*a(n-1)) + floor((5/2)*a(n-2)), with a(0) = 1,
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static size_t seq_cap   = 0;       /* current allocation size */
static idx_t seq_max    = (idx_t)-1;   /* last index that has been filled
                                         (sentinel when not yet initialized) */

static void seq_grow(idx_t n_needed) {
    if ((size_t)n_needed < seq_cap)
        return;                 /* already large enough */
    size_t new_cap = seq_cap ? seq_cap * 2 : 2;
    while (new_cap <= (size_t)n_needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                /* unrecoverable allocation failure */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

seq_t a(idx_t n) {
    /* Initialise the first two terms on the first call */
    if (seq_max == (idx_t)-1) {
        seq_grow(1);                    /* ensures indices 0 and 1 are valid */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_max = 1;
    }

    /* If the term is already computed, return it immediately */
    if (n <= seq_max)
        return seq_cache[n];

    /* Otherwise compute all missing terms up to n */
    seq_grow(n);
    for (idx_t i = seq_max + 1; i <= n; ++i) {
        seq_t t1 = ((seq_t)5 * seq_cache[i - 1]) / (seq_t)2;
        seq_t t2 = ((seq_t)5 * seq_cache[i - 2]) / (seq_t)2;
        seq_cache[i] = t1 + t2;
    }
    seq_max = n;
    return seq_cache[n];
}
