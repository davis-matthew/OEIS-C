/*
 * A280873  Numbers whose binary expansion does not begin 10 and does not
 *          contain 2 adjacent 0's; Ahnentafel numbers of X-chromosome
 *          inheritance of a male.
 * https://oeis.org/A280873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose binary expansion does not begin 10 * and does not
 *            contain two consecutive 0's (A280873)
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 8482, u64 n <= 8482,
 *            u128 n <= 8482, big any n
 * Helpers:   valid_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/valid_bin.h"

/* Sequence generator.  The sequence starts with a(0)=0. */
seq_t a(idx_t n)
{
    /* Static cache of generated terms. */
    static seq_t *cache   = NULL;   /* stored values */
    static size_t cap     = 0;      /* allocated capacity */
    static size_t size    = 0;      /* number of terms stored (== 1 initially) */
    static seq_t cand     = 0;      /* next integer to test, starts at 0 */

    if (!cache) {                   /* first call: allocate and seed with 0 */
        cap  = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0;                /* a(0) = 0 */
        size = 1;
    }

    /* Generate new terms until we have at least n+1 elements. */
    while (size <= n) {
        /* Find the next integer that satisfies the condition. */
        while (1) {
            cand++;                       /* skip 0 on the first iteration */
            if (valid_bin(cand))
                break;
        }

        /* Grow cache if necessary. */
        if (size >= cap) {
            cap *= 2;
            seq_t *ptr = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!ptr)        /* allocation failed – program aborts */
                abort();
            cache = ptr;
        }

        cache[size++] = cand;            /* store the new term */
    }

    return cache[n];
}
