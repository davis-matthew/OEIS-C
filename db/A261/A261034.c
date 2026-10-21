/*
 * A261034  Numbers m such that 3*m is squarefree.
 * https://oeis.org/A261034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer m such that 3*m is squarefree
 *            (i.e., m is squarefree and not divisible by 3). Computed by
 *            incremental search with memoisation
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_squarefree.h"

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated capacity */
static size_t len   = 0;   /* number of values stored */

/* Next integer to test for membership. */
static idx_t next = 1;

/* Grow the cache to at least new_cap elements. */
static void grow_cache(void)
{
    size_t new_cap = cap ? cap * 2 : 4;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* allocation failure */
    cache = tmp;
    cap   = new_cap;
}

/* Return the n‑th term of A261034.  n >= 1; behaviour for n==0 is
   undefined but returns 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below offset – undefined behaviour */

    size_t target = (size_t)n;        /* target number of terms */

    /* Return cached value if already computed. */
    if (len >= target)
        return cache[target - 1];

    /* Generate terms until we have at least 'target' of them. */
    while (len < target) {
        /* Find next candidate that is not divisible by 3 and squarefree. */
        while (1) {
            if (next % 3 != 0 && is_squarefree((seq_t)next))
                break;
            ++next;
        }

        /* Ensure space for the new term. */
        if (len >= cap)
            grow_cache();

        /* Store the found value and advance to the next integer to test. */
        cache[len++] = (seq_t)next;
        ++next;
    }

    /* Return the requested term. */
    return cache[target - 1];
}
