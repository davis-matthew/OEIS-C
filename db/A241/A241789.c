/*
 * A241789  Triangular numbers which have one or more occurrences of exactly
 *          six different digits.
 * https://oeis.org/A241789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2500, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Helpers:   tri_has_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/tri_has_property.h"

/* The sequence: a(n) = the n‑th triangular number that uses
   exactly six distinct decimal digits.  Triangular numbers T_k
   are defined by T_k = k*(k+1)/2.  The algorithm simply tests
   each triangular number for the digit‑set condition and
   collects the qualifying values. */

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for offset 1, return 0 gracefully */
        return 0;

    /* Static cache for memoisation. */
    static idx_t last_checked = 0;          /* largest triangular index processed */
    static idx_t terms_found = 0;           /* number of qualifying terms stored */
    static seq_t *cache = NULL;             /* array of found terms */
    static size_t capacity = 0;             /* current allocation size */

    /* Return cached value if already computed. */
    if (n <= terms_found)
        return cache[n - 1];

    /* Allocate cache on first use. */
    if (!cache) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Generate triangular numbers one by one until we have the nth term. */
    for (last_checked++; ; ++last_checked) {
        seq_t tri = (seq_t)last_checked * ((seq_t)last_checked + 1) / 2;
        if (tri_has_property(tri)) {
            /* Store if we found a new qualifying term. */
            if (terms_found == capacity) {
                capacity *= 2;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[terms_found++] = tri;
            if (terms_found >= n)
                return cache[n - 1];
        }
    }
}
