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
 * Range:     u8 n <= 54, u16 n <= 2583, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 71 terms through n = 8482 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper that checks whether a number satisfies the condition.
 * zero is considered valid.
 */
static int valid_bin(seq_t v)
{
    if (v == 0)                     /* zero is allowed */
        return 1;

    /* Determine the index of the most significant set bit (0‑based). */
    idx_t msb = 0;
    seq_t t = v;
    while (t >> 1) {                /* shift until only one bit left */
        t >>= 1;
        msb++;
    }

    if (msb == 0)                   /* single bit 1 (value 1) */
        return 1;

    /* Check that the second most significant bit is not 0. */
    seq_t second = (v >> (msb - 1)) & 1;
    if (!second)
        return 0;

    /* For two‑bit numbers (msb==1) we are done. */
    if (msb == 1)
        return 1;

    /* Scan lower bits for a forbidden '00' pattern. */
    idx_t pos = msb - 2;
    seq_t prev = 1;                  /* the bit at msb is 1 */
    while (1) {
        seq_t curr = (v >> pos) & 1;
        if (!prev && !curr)          /* two consecutive zeros found */
            return 0;
        if (pos == 0)                /* reached the least significant bit */
            break;
        prev = curr;
        pos--;
    }

    return 1;
}

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
        cache = mem_new(seq_t, cap);
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
            seq_t *ptr = mem_resize(seq_t, cache, cap);
            if (!ptr)        /* allocation failed – program aborts */
                abort();
            cache = ptr;
        }

        cache[size++] = cand;            /* store the new term */
    }

    return cache[n];
}
