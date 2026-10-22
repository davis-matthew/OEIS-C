/*
 * A373550  a(n) is the parity of the n-th squarefree number.
 * https://oeis.org/A373550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   not_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the parity (mod 2) of the n‑th squarefree number – i.e. the OEIS
   sequence A373550.  The function uses a lazy cache: it keeps the parity
   values of the first k squarefree numbers and extends this cache when a
   larger index is requested.  The implementation works for any build
   where seq_t is an unsigned integer type or an arbitrary‑precision
   integer class, and idx_t is an unsigned 64‑bit integer.  The cache is
   grown by realloc / malloc; cast is used to satisfy C++ strict type
   checking.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/not_squarefree.h"

/* Cache of parities (0 or 1) of the first k squarefree numbers. */
static seq_t *parity_cache = NULL;   /* array of parity values */
static size_t parity_cap   = 0;      /* capacity of the cache */
static idx_t  sqfree_found = 0;      /* how many squarefree numbers stored */
static idx_t  next_cand    = 1;      /* next integer to test */

/* Grow the cache to at least new_cap elements.  A cast to (seq_t *) is
   required to satisfy the C++ type system when seq_t may be a class type. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(parity_cache, new_cap * sizeof(seq_t));
    parity_cache = tmp;
    parity_cap   = new_cap;
}

/* External function required by the evaluator. */
seq_t a(idx_t n)
{
    if (n <= 0) {
        return (seq_t)0;   /* undefined for n < 1 – return 0 */
    }

    /* Extend the cache until we have at least n entries. */
    while (sqfree_found < n) {
        if (!not_squarefree(next_cand)) {
            if (parity_cap == sqfree_found) {
                grow_cache(parity_cap == 0 ? 16 : parity_cap * 2);
            }
            parity_cache[sqfree_found] = (seq_t)(next_cand & 1); /* parity of the squarefree number */
            ++sqfree_found;
        }
        ++next_cand;
    }

    return parity_cache[n - 1];
}
