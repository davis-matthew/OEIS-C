/*
 * A338110  Number of spanning trees in the join of the disjoint union of two
 *          complete graphs each on n vertices with the empty graph on n
 *          vertices.
 * https://oeis.org/A338110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n)^(3*(n-1))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

/* Cached values: 1‑based index, cache[0] unused */
static seq_t *cache   = NULL;   /* array of computed terms       */
static unsigned char *valid = NULL; /* 1 if cache[i] is valid      */
static size_t cap    = 0;       /* current capacity of the arrays */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n=0 – return 0 so callers can ignore */
        return (seq_t)0;
    }

    /* Ensure the cache can hold the n-th term */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;   /* +1 because 1‑based */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        unsigned char *new_valid = (unsigned char *)realloc(valid, new_cap);
        if (!new_cache || !new_valid) {
            abort();   /* allocation failure */
        }
        /* Zero‑initialize any new flag entries */
        memset(new_valid + cap, 0,
               (new_cap - cap) * sizeof(unsigned char));
        cache = new_cache;
        valid = new_valid;
        cap   = new_cap;
    }

    if (!valid[n]) {
        /* Compute a(n) = n * (2*n)^(3*(n-1)) */
        seq_t base = ((seq_t)2 * (seq_t)n);
        seq_t exponent = ((seq_t)3 * (seq_t)(n - 1));
        seq_t pow_val = big_pow(base, exponent);
        seq_t result = (seq_t)n * pow_val;
        cache[n] = result;
        valid[n] = 1;
    }

    return cache[n];
}
