/*
 * A386536  Exponent of the highest power of 2 dividing the n-th number that
 *          is cubefree but not squarefree.
 * https://oeis.org/A386536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exponent_of_2, is_cubefree_not_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386536: exponent of 2 dividing the n‑th integer that is cubefree but not squarefree.
   It is computed by a linear search over the natural numbers, caching results so that
   repeated calls are fast.  No closed form is known. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/exponent_of_2.h"
#include "../utils/is_cubefree_not_squarefree.h"

/* --------------------------------------------------------------------- */
/* Cache for computed values                                         */
/* --------------------------------------------------------------------- */
static seq_t *cache   = NULL;   /* array of a(k) */
static size_t cache_cnt = 0;    /* number of stored values */
static size_t cache_cap = 0;    /* allocated capacity */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Ensure cache has room for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Next candidate integer to test */
static idx_t next_to_check = 1;

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n==0 but avoid UB */
        return 0;

    /* Extend cache until we have at least n entries */
    while ((size_t)n > cache_cnt) {
        if (is_cubefree_not_squarefree(next_to_check)) {
            ensure_cache_cap(cache_cnt + 1);
            cache[cache_cnt++] = exponent_of_2(next_to_check);
        }
        ++next_to_check;
    }
    return cache[n - 1];
}
